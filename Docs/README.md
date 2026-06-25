# KinetiForge Documentation

This folder contains design and usage notes for KinetiForge Vehicle System.

## Design Notes

- [Drivetrain Design](DrivetrainDesign.md): drivetrain update order, torque flow,
  reflected inertia, and clutch spring model.
- [WheelCoordinator Design](WheelCoordinator.md): chassis-level shared data,
  sprung mass, wheelbase, and aero coordination.
- [Assembly Components](AssemblyComponents.md): generated vs existing
  components, ownership, and update hierarchy.
- [Tire And Wheel Design](TireAndWheel.md): constraint tire force, tire curves,
  combined slip, wheel speed integration, and ABS behavior.
- [Suspension Design](SuspensionDesign.md): raycast-to-geometry suspension
  solving, strut integration, solid axle coordination, and effective mass.

## Planned Documents

- `TuningGuide.md`: practical tuning workflow for vehicle behavior.
