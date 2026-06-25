# Tire And Wheel Design

`VehicleWheelComponent` is the bridge between three systems:

- The drivetrain, which sends drive torque, brake torque, and reflected inertia.
- The suspension, which provides contact state, normal load, effective sprung
  mass, and wheel pose.
- The tire solver, which turns slip and load into contact force.

The tire model is built around one important idea:

> Constraint force is the stability foundation. Tire curves limit that force.

In other words, the solver first computes the force that would constrain the
wheel and chassis motion in a stable way. Then the tire curves decide how much
of that force the tire is allowed to keep.

## Wheel Update Overview

A wheel update is split into three phases:

```text
PreStepWheel
  -> read suspension/contact state
  -> compute force directions and local ground velocity
  -> update available grip and wheel load

SubStepWheel
  -> receive drive/brake torque and reflected inertia
  -> solve tire force
  -> integrate wheel angular velocity

PostStepApplyForce
  -> apply tire and suspension impulses to the chassis
  -> apply equal and opposite impulse to the hit component when possible
```

When a wheel belongs to an axle assembly, the axle calls these phases in the
correct drivetrain order. Standalone wheels can still be used, but they must be
updated manually from a physics-safe callback.

## Camber And Force Directions

The suspension solver provides the wheel hub rotation and the world-space wheel
right vector. The wheel solver uses that vector to build tire force directions
on the contact surface.

The lateral force direction is the wheel right vector projected onto the ground
plane. The longitudinal force direction is computed from the lateral direction
and the impact normal. In code, this comes from the wheel right vector and the
ground normal through projection and cross product.

The length of the projected direction is also used as a force scale:

- If the wheel is upright relative to the ground, the projected directions keep
  most of their length.
- If the wheel has strong camber relative to the ground normal, the projection
  length becomes smaller, so available longitudinal and lateral tire force is
  reduced.

This gives camber a rough influence on tire force direction and magnitude.
However, KinetiForge does not currently simulate explicit camber thrust. Camber
changes the contact-plane force basis and scaling, but it does not generate a
separate lateral force purely from camber angle.

## Wheel Angular Velocity

The wheel has its own angular velocity and effective inertia. Effective inertia
includes the wheel's intrinsic inertia plus drivetrain inertia reflected down to
the wheel.

When integrating wheel speed, the solver considers ground-relative motion. The
longitudinal tire reaction is clamped so it cannot flip the sign of the relative
velocity between the wheel surface and the ground in one step. This prevents the
tire reaction from overshooting and creating artificial oscillation.

Brake torque is handled after the non-braking wheel response is estimated. If
braking would reverse the wheel's angular velocity, the wheel is treated as
locked and angular velocity is set to zero.

## Constraint Tire Force

Constraint tire force is the fallback and stability layer of the tire model.
It does not require a well-authored tire curve.

The solver computes longitudinal and lateral constraint forces separately.

### Lateral Constraint

The lateral constraint attempts to cancel lateral contact velocity within the
current macro step.

The impulse is limited by the lateral effective sprung mass. Conceptually, the
lateral tire impulse should not exceed the lateral momentum available at the
contact patch.

This gives the tire a stable fallback even when the lateral tire curve is
missing or unusable.

### Longitudinal Constraint

The longitudinal constraint combines several effects:

- Drive torque divided by wheel radius.
- Ground interaction torque that would synchronize wheel angular velocity with
  ground speed in one step.
- Brake force, applied after the current wheel/ground motion direction is known.

Brake impulse is limited so it cannot exceed the longitudinal momentum that can
be removed from the effective sprung mass. This prevents braking from
numerically pulling the wheel/chassis system past zero velocity in one step.

On slopes, longitudinal braking has special handling. A locked wheel can receive
gravity compensation along the slope, but that compensation is limited by the
available brake force. As a result, braking on steep slopes is intentionally
less perfect than lateral holding and may not be able to stabilize a parked
vehicle on every incline.

## Tire Curves As Force Limits

After computing constraint force, the tire curve model limits it.

`FVehicleTireConfig` provides two optional curves:

- `Fx`: longitudinal friction curve, indexed by slip ratio.
- `Fy`: lateral friction curve, indexed by slip angle in degrees.

The curves are converted into fixed 64-sample LUTs. The LUTs avoid repeatedly
reading curve data during physics updates and reduce the risk of data-race
problems around `TArray`-backed curve data.

If a curve is missing or does not provide usable linear stiffness, that
direction falls back to constraint force limited by maximum available grip.

## Available Grip

Available grip is computed from:

- Tire friction multiplier.
- Physical material friction, depending on `TireFrictionCombineMode`.
- Static sprung mass.
- Current wheel load.
- `WheelLoadInfluenceFactor`.

This means tire force is not only a function of slip. Load transfer and
suspension force can change how much grip the tire has available.

## Combined Slip

The combined slip model is inspired by TMeasy-style behavior.

The simplified idea is:

1. Read the linear-region stiffness from the `Fx` and `Fy` LUTs.
2. Update transient slip using relaxation length.
3. Scale longitudinal and lateral transient slip by their linear stiffness.
4. Combine them into one slip magnitude.
5. Recover a direction from the longitudinal/lateral slip ratio.
6. Use the tire LUTs to compute force limits in that direction.
7. Clamp the constraint force by those limits.

If `Fx` and `Fy` are the same shape, this behaves similarly to a TMeasy-like
combined slip tire. If the curves differ, the model can bias longitudinal and
lateral behavior independently.

The final force remains aligned with the slip direction and is constrained by
the available combined grip. When both `Fx` and `Fy` are valid, the model does
not need a separate explicit friction-circle pass for normal operation because
the combined-slip lookup already couples the two directions.

If one or both curves are missing or invalid, the solver applies a fallback
friction ellipse to prevent force overshoot.

## Relaxation Length And Transient Slip

The tire does not have to respond instantly to raw slip.

`RelaxationLength` filters slip into transient slip. Higher values make tire
response softer and laggier. This can be useful for reducing jitter or giving
heavy vehicles a less immediate tire response.

Transient slip is used by the tire curve model, while raw slip ratio and slip
angle remain useful for debugging, ABS, TCS, skid effects, and telemetry.

## Low-Speed Behavior

Low-speed tire behavior is treated carefully because ordinary slip ratio and
slip angle formulas become unstable near zero speed.

The solver:

- Uses a minimum denominator for slip ratio.
- Blends toward a low-speed lateral slip approximation.
- Uses constraint force as the primary stability layer.
- Prevents tire reaction torque from reversing wheel/ground relative velocity in
  one step.

This is why the vehicle can remain stable at low speed even when tire curves are
not perfect.

## ABS And Braking

ABS predicts the next slip ratio from current angular acceleration and target
brake torque. It only activates when:

- ABS is enabled.
- The wheel is on ground.
- The vehicle is above activation speed.
- Predicted slip exceeds the target slip.
- Slip direction indicates braking lockup behavior.

When ABS triggers, it scales brake torque down according to sensitivity.

## Practical Tuning Notes

- Treat constraint force as the safety net, not the desired tire feel.
- Shape `Fx` for acceleration/braking behavior.
- Shape `Fy` for cornering behavior.
- Increase `FrictionMultiplier`, `MaxFx`, or `MaxFy` for more available grip.
- Increase `RelaxationLength` for slower, softer tire response.
- Use `CombinedSlipBias` to bias grip sharing between longitudinal and lateral
  directions.
- Use the interference parameters for more arcade or more coupled behavior.
- If the vehicle is unstable at low speed, inspect wheel load, effective sprung
  mass, relaxation length, and tire curve linear stiffness.

## Related Components

- `VehicleWheelComponent`: owns wheel, tire, ABS, mesh, and suspension config.
- `VehicleWheelSolver`: solves wheel speed, slip, tire force, and ABS behavior.
- `VehicleSuspensionSolver`: provides contact state, normal force, and
  direction-specific effective sprung mass.
- `VehicleAxleAssemblyComponent`: distributes drive/brake torque and calls wheel
  update functions in drivetrain order.
