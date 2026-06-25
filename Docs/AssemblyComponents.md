# Assembly Components

Assembly components are higher-level components that organize smaller vehicle
physics components into usable systems.

The main assembly components are:

- `VehicleDriveAssemblyComponent`
- `VehicleAxleAssemblyComponent`

They are designed to support two workflows:

- Quick setup, where the assembly generates child components from classes.
- Manual setup, where the assembly binds to components already placed in a
  Blueprint.

This lets the same plugin support simple vehicles, custom Blueprint layouts,
recordable level-sequence components, and unusual drivetrain layouts.

## Naming Convention

Most runtime components follow this pattern:

```text
Vehicle + role name + Component
```

Examples:

- `VehicleEngineComponent`
- `VehicleClutchComponent`
- `VehicleGearboxComponent`
- `VehicleWheelComponent`

Assembly components add `Assembly` to indicate that the component owns or
coordinates downstream parts:

```text
Vehicle + role name + Assembly + Component
```

Examples:

- `VehicleDriveAssemblyComponent`
- `VehicleAxleAssemblyComponent`

In this naming scheme, an assembly is not just a data container. It also defines
ownership, binding, update order, or torque distribution for the components
below it.

## DriveAssembly

One `VehicleDriveAssemblyComponent` represents one drivetrain tree.

A typical drive assembly contains:

- One engine
- One clutch
- One gearbox
- One transfer case or center differential
- Any number of axle assemblies

The usual structure is:

```text
VehicleDriveAssemblyComponent
  -> VehicleEngineComponent
  -> VehicleClutchComponent
  -> VehicleGearboxComponent
  -> VehicleDifferentialComponent as transfer case
  -> VehicleAxleAssemblyComponent[]
```

The drive assembly owns the drivetrain update order. It is responsible for
running the engine, clutch, gearbox, transfer case, axles, differentials, and
wheels in a sequence that preserves torque flow, angular velocity feedback, and
reflected inertia.

For more detail, see [Drivetrain Design](DrivetrainDesign.md).

## AxleAssembly

One `VehicleAxleAssemblyComponent` represents one axle-level drivetrain and
suspension group.

An axle assembly normally contains:

- One differential
- One or two wheels
- Steering logic
- Brake and handbrake torque distribution
- Traction control logic
- Independent or solid axle suspension coordination

The usual structure is:

```text
VehicleAxleAssemblyComponent
  -> VehicleDifferentialComponent
  -> VehicleWheelComponent left
  -> VehicleWheelComponent right
```

An axle can also be configured with only one wheel. This can be useful for
unusual vehicles or non-car use cases.

## Generated Components

Assemblies can create missing child components automatically.

For example, a drive assembly can generate:

- Engine
- Clutch
- Gearbox
- Transfer case
- Axle assemblies

An axle assembly can generate:

- Left and right wheels
- Differential

Generated components are tracked by the assembly. When the assembly is
destroyed, generated components are destroyed with it.

This is useful for quick setup, because a user can configure component classes
and let the assembly build the structure.

## Existing Components

Assemblies can also bind to components that already exist on the Actor.

This is useful when:

- You want full manual control of component placement.
- You need specific component names in Blueprint.
- You want level sequences to recognize and record component animation.
- You want multiple assemblies to reference explicitly configured components.
- You are building a custom layout that does not match the default generator.

Existing components are not treated as owned by the assembly. Destroying the
assembly should not imply that manually placed Blueprint components are part of
the assembly's generated-component cleanup.

## Component Attachment

Wheels, axles, drive assemblies, and coordinators generally look for the closest
physical parent component. In normal vehicle setups this is the chassis or car
body primitive component.

This physical parent is important because:

- Suspension forces are applied to the chassis.
- Wheel positions are interpreted relative to the chassis.
- Sprung mass is computed from chassis mass and wheel positions.
- Aero forces are applied to the chassis.
- The coordinator is shared by components attached to the same chassis.

For best results, attach vehicle physics components under the primitive
component that represents the vehicle body.

## Update Ownership

Assembly ownership is also update ownership.

If a wheel is part of an axle assembly, the axle calls the wheel update
functions. If an axle is part of a drive assembly, the drive assembly calls the
axle update functions.

The usual automatic update path is:

```text
VehicleAsyncTickComponent
  -> VehicleDriveAssemblyComponent::UpdatePhysics
  -> VehicleAxleAssemblyComponent::PreStepAxle
  -> VehicleAxleAssemblyComponent::SubstepAxle
  -> VehicleWheelComponent::SubStepWheel
  -> VehicleAxleAssemblyComponent::PostStepAxle
```

Standalone wheels and standalone axles are still supported, but they must be
updated manually from an async physics callback or another physics-safe update
path.

## Choosing A Workflow

Use generated components when:

- You want the fastest setup.
- The default component hierarchy is acceptable.
- You prefer editing classes and config templates.
- You do not need every child component manually placed in the Blueprint.

Use existing components when:

- You need precise Blueprint hierarchy control.
- You want to animate or record specific components.
- You want to reuse manually configured components.
- You are building a non-standard drivetrain or vehicle layout.

Both workflows can be mixed. For example, a drive assembly can bind to an
existing axle while generating its engine, clutch, gearbox, and transfer case.

## Recommended Hybrid Workflow

A practical workflow is to place the main drivetrain components manually, while
letting each axle generate its wheels from a wheel subclass.

Recommended setup:

- Place `VehicleDriveAssemblyComponent` in the Blueprint.
- Place or bind existing engine, clutch, gearbox, transfer case, and axle
  assembly components.
- Configure each axle assembly to generate its left and right wheels from a
  `VehicleWheelComponent` subclass.
- Store tire, suspension, wheel mesh, and brake mesh settings in that wheel
  subclass.

This keeps the high-level drivetrain readable in the Blueprint while avoiding
the repetitive work of manually placing and aligning every wheel component.

Wheel generation is especially useful because wheels are usually owned by an
axle-level layout. The axle already knows whether it has two wheels, one left
wheel, or one right wheel, and it can position generated wheel components from
track width and axle position.

Other drivetrain components are usually fewer in number and often benefit from
being visible in the Blueprint hierarchy. Manually placing them can make custom
layouts easier to inspect and debug.

## Default Components

If a component class is not specified, the assembly attempts to create a default
component type.

This makes quick setup easier because a minimal vehicle can be assembled without
creating a subclass for every part. For serious tuning, explicit subclasses are
recommended so each vehicle can carry its own wheel, tire, suspension, engine,
gearbox, and differential settings.

## Practical Notes

- A `DriveAssembly` corresponds to one drivetrain tree, not necessarily one
  whole vehicle.
- A vehicle Actor may contain multiple drive assemblies.
- An `AxleAssembly` can be used without a drive assembly, but it must be updated
  manually.
- A `WheelComponent` can be used without an axle assembly, but it must be
  updated manually.
- Generated components are convenient, but existing components are better when
  editor visibility, sequencing, or custom layout matters.
