# Suspension Design

KinetiForge suspension is more than a simple raycast spring. The solver uses a
two-stage process:

1. Assume the previous or current approximate suspension posture.
2. Cast against the ground to estimate available strut travel.
3. Integrate strut length.
4. Recompute the wheel hub posture from suspension geometry.
5. Compute spring, damping, jacking, and effective mass values.

The raycast is used to estimate contact and strut travel. It is not the final
wheel pose.

## Update Pipeline

For independent suspension, `FVehicleSuspensionSolver::UpdateSuspension`
roughly follows this order:

```text
Previous suspension state
  -> prepare chassis/wheel transforms
  -> compute raycast start/end from approximate posture
  -> raycast or sweep
  -> estimate hit distance
  -> update strut length
  -> solve suspension geometry
  -> compute anti-pitch / anti-roll geometry
  -> compute suspension force and effective mass
  -> publish suspension state
```

For solid axle suspension, the work is split between wheel components and
`VehicleAxleAssemblyComponent`:

```text
Left wheel StartUpdateSolidAxle
Right wheel StartUpdateSolidAxle
  -> each side raycasts and estimates strut length

AxleAssembly
  -> solves solid axle posture from left/right strut lengths

Left wheel FinalizeUpdateSolidAxle
Right wheel FinalizeUpdateSolidAxle
  -> each side computes final hub posture and force
```

This split is necessary because a solid axle posture depends on both wheels at
the same time.

## Raycast Purpose

The raycast is intentionally approximate.

The solver first assumes the wheel posture from the previous state or from a
direct kinematic estimate. It then chooses a trace line along the wheel
component's local Z direction. The exact ray location is not meant to be a full
contact patch model. Its purpose is to estimate how much suspension travel is
available before the wheel touches the ground.

The trace position is chosen from suspension geometry:

- X/Y placement is based on the lower ball joint area.
- Z placement is based on the compressible part of the strut.
- Trace length is based on the projection of the compressible strut travel onto
  the trace direction.

After the trace, the result is converted back into strut travel. The solver then
recomputes wheel posture from geometry.

## Trace Modes

The suspension supports several trace modes:

- `LineTrace`: fastest and simplest.
- `SphereTrace`: sweeps a sphere with wheel-radius scale and may refine bad
  hits.
- `BoxTrace`: sweeps a box-shaped approximation.
- `SphereTraceNoRefinement`: uses a smaller sphere without refinement.
- `MultiSphereTrace`: tries additional checks when the first trace appears to
  hit an invalid area.

These modes mostly affect contact detection robustness and performance. They do
not change the high-level suspension model.

## Hit Distance

The solver keeps two useful interpretations of hit distance:

- `HitDistance`: the approximate distance from the trace start to the ground as
  if the result were interpreted like a single ray.
- `HitDistanceNoBias`: the hit distance with wheel-radius bias removed, similar
  to the distance between sphere centers when tracing a sphere with wheel
  radius.

`HitDistanceNoBias` is projected back onto the strut travel range to estimate
the maximum current compressible strut length. This is the value used by
`UpdateStrutLength` to decide how far the suspension can extend before the wheel
loses contact.

## Strut Length Integration

If `SuspensionAndBrakeMass` is greater than zero, the solver simulates a virtual
unsprung mass. This includes estimated wheel mass plus suspension/brake mass.

In this mode, strut length is integrated through substeps. The solver considers:

- Spring force.
- Compression and rebound damping.
- Spring preload.
- Swaybar force.
- Gravity along the strut direction.
- Fictitious force from chassis acceleration at the mount point.
- Ground-imposed maximum strut extension from the trace result.

The integration uses an implicit-style denominator for damping and spring
stiffness. This helps avoid oscillation when the virtual unsprung mass and
spring are stiff.

If `SuspensionAndBrakeMass` is zero or negative, virtual unsprung mass simulation
is disabled. The strut length follows the raycast-derived maximum length
directly. This is usually more stable and easier to tune, but less physically
detailed.

## Hub Offset

`HubOffsetFromLowerJoint` defines the rigid offset from the lower ball joint to
the wheel hub.

This offset is important. It means the contact patch and hub do not have to sit
directly on the lower ball joint. When camber, steering, or axle posture changes,
the offset can naturally move the hub and contact patch in a way that affects
the chassis.

One visible result is that steering with camber can lift or push the chassis
through geometry. This is an emergent result of the current kinematic model
rather than a separate hard-coded jacking effect.

## Independent Suspension Types

### StraightLine

`StraightLine` moves along the strut direction. It is useful for simple
fork-like suspension, such as motorcycle-style fork motion or simple vertical
wheel travel.

The wheel rotation can still receive camber, toe, and caster offsets from lookup
tables and static spindle rotation.

### MacPherson

The simplified MacPherson model uses one lower control arm and one strut. The
lower ball joint is solved from:

- Lower arm pivot.
- Lower arm axis and length.
- Top mount.
- Current strut length.

The steer axis is approximated by the strut direction. Hub rotation is then
derived from steering angle, strut axis, lookup-table rotation, and static
spindle rotation.

### Double Wishbone

The simplified double wishbone model uses:

- One lower control arm.
- One upper control arm.
- A knuckle length between lower and upper ball joints.
- A strut connected to the lower ball joint.

The strut connection point is currently the lower ball joint. It cannot be moved
to another point on the lower arm or chassis-side linkage.

The solver first solves the lower ball joint from the lower arm and strut. Then
it solves the upper ball joint from the upper arm and knuckle length. The steer
axis is the line between the lower and upper ball joints.

Camber, toe, and caster changes can also be added through lookup tables.

## Solid Axle

Solid axle suspension is coordinated by `VehicleAxleAssemblyComponent`, because
both wheels define one rigid axle posture.

Each side first raycasts and estimates its strut length. The axle assembly then
passes the left and right strut lengths into a solver that finds the axle center
and axle rotation. Each wheel then finalizes its hub pose from that axle posture.

Important differences from independent suspension:

- Solid axle has no `StaticSpindleRotation` / spindle mount rotation path.
- It does not use upper or lower wishbones as actual control arms.
- It still reuses lower ball joint style data as the connection point for the
  damper and wheel-side axle endpoint.
- Axle width depends on the two wheels' designed hub locations.

In other words, solid axle uses the same wheel/suspension component interface,
but the real posture is solved at axle level.

## Lookup Tables

Suspension lookup tables are used as additional offsets:

- Camber.
- Toe.
- Caster.
- Anti-dive.
- Anti-squat.
- Anti-roll.
- Spring motion ratio.

The lookup-table path is useful when you want fast tuning without fully matching
real suspension linkage geometry. The geometry solver provides the base posture,
and the LUTs add tunable behavior over suspension compression.

For camber, toe, and caster, the LUT rotation is composed on top of the realtime
geometry result. For anti-dive, anti-squat, and anti-roll, the LUT value is added
to the realtime geometry ratio. In other words, LUTs are supplements to the
geometry solver, not a replacement for it.

## Anti-Pitch And Anti-Roll Geometry

The suspension solver estimates anti-pitch and anti-roll behavior from the
current suspension geometry.

For MacPherson and double wishbone suspension, the solver projects the relevant
arm and strut constraints into 2D pitch and roll planes. It then estimates the
swing-arm or instant-center slope from the contact patch. That slope is
converted into an anti ratio using the current contact patch position, center of
mass height, and dynamic lever arm.

The simplified flow is:

```text
current suspension geometry
  -> projected pitch/roll instant-center slope
  -> geometric anti-pitch / anti-roll ratio
  -> add anti-dive / anti-squat / anti-roll LUT values
  -> convert ratio back into vertical jacking force
  -> add jacking force to normal suspension force
```

For pitch, braking uses the anti-dive LUT and acceleration uses the anti-squat
LUT. For roll, the anti-roll LUT is added directly.

Solid axle suspension currently uses the LUT-only path for this calculation.
That is because the solid axle posture is solved at axle level rather than from
the same independent control-arm geometry.

This system is meant to capture the vehicle-dynamics effect of suspension force
paths. It is not a full finite-element or bushing-compliance model.

## Effective Sprung Mass

The suspension solver computes direction-specific effective sprung mass:

- Normal direction.
- Longitudinal tire-force direction.
- Lateral tire-force direction.

These values are important for stability. Tire constraint forces use the
longitudinal and lateral effective sprung masses to limit how much momentum can
be removed in one step. Suspension force uses the normal effective mass to clamp
spring/damper behavior and bottom-out response.

This is why `WheelCoordinatorComponent` computes static sprung mass, while the
suspension solver computes dynamic direction-specific effective masses.

At a high level, effective mass answers this question:

> If an impulse is applied at the contact patch in this direction, how much mass
> does the chassis appear to have?

The value depends on both chassis mass and rotational inertia. A force applied
far from the center of mass can rotate the chassis easily, so the effective mass
in that direction may be lower than the total chassis mass.

The solver computes this from the chassis inverse mass, world inverse inertia
tensor, contact point offset from center of mass, and the target direction:

```text
effective mass = 1 / (linear inverse mass + angular inverse mass along direction)
```

The exact matrix calculation is kept in code, but this conceptual model is
important for tuning. Effective sprung mass is why the same spring or tire force
can feel different at different contact locations and force directions.

## Force Application

Suspension and tire impulses can be applied at:

- The impact point.
- The wheel center.

Applying at the impact point produces more pitch and roll moment from tire and
suspension forces. Applying at the wheel center can be calmer and may be easier
to tune for some vehicles.

## Practical Notes

- The raycast location is an estimate. The final hub transform comes from the
  suspension geometry solver.
- If the vehicle behaves strangely, first verify top mount, lower ball joint,
  hub offset, wheel radius, and stroke.
- For double wishbone suspension, remember that the strut currently connects to
  the lower ball joint.
- For solid axle suspension, tune both wheels together; one side alone does not
  define the axle posture.
- Setting `SuspensionAndBrakeMass` to zero or below disables virtual unsprung
  mass simulation and can make vehicles easier to stabilize.
- Lookup tables are a practical way to tune camber/toe/caster and anti-geometry
  without rebuilding linkage geometry.

## Related Components

- `VehicleSuspensionSolver`: solves trace, strut length, geometry, force, and
  effective mass.
- `VehicleWheelComponent`: owns suspension config and calls the solver.
- `VehicleAxleAssemblyComponent`: coordinates solid axle suspension.
- `VehicleWheelCoordinatorComponent`: provides static sprung mass for each
  wheel.
- `VehicleWheelSolver`: consumes effective sprung mass and contact state for
  tire force calculation.
