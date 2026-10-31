# Physics model overview

This page is for readers who want to understand or modify the vehicle calculations. It is an implementation map, not a complete derivation or a claim that the models reproduce every real tire, suspension, or powertrain. For exact behavior, follow the linked solver code and configuration structs.

## How the main models connect

```mermaid
flowchart LR
    Ground[Ground query] --> Contact[Contact and suspension geometry]
    Contact --> Suspension[Suspension force]
    Suspension --> Chassis[Chassis]
    Suspension --> Load[Wheel load]
    Load --> Tire[Tire slip and force]
    Chassis --> Hub[Hub velocity]
    Hub --> Tire
    Engine[Engine] --> Clutch[Clutch]
    Clutch --> Gearbox[Gearbox]
    Gearbox --> TransferCase[Transfer case]
    TransferCase --> AxleDiff[Axle differential]
    AxleDiff --> Wheel[Wheel rotation]
    Wheel --> Tire
    Tire --> Wheel
    Tire --> Chassis
```

The wheel-to-tire and tire-to-wheel links show the coupled wheel-speed and tire-force calculation, not separate solver stages.

## Tire forces

The wheel solver derives longitudinal slip from wheel peripheral speed and hub velocity. It derives slip angle from wheel-local velocity, with a low-speed blend. A camber-to-drift curve can add lateral contact-patch drift; this changes the slip input and is not a direct camber-thrust force term.

The `Fx` curve maps slip ratio to a longitudinal friction coefficient, and `Fy` maps slip angle in degrees to a lateral coefficient. The solver applies tire/load scaling and `MaxFx`/`MaxFy`, then couples longitudinal and lateral demand using the combined-slip settings. `RelaxationLength` makes the slip response build over time instead of changing instantly. The exact coupling and force limits are in the solver; tuning curves should be treated as part of the model, not just visual presets.

Useful entry points:

- Configuration and telemetry: [VehicleWheelStructs.h](../Source/KinetiForge/Public/VehicleWheelStructs.h)
- Slip, relaxation, and tire-force calculations: [VehicleWheelSolver.cpp](../Source/KinetiForge/Private/VehicleWheelSolver.cpp)
- Wheel integration and handoff between suspension and tire stages: [VehicleWheelRuntime.cpp](../Source/KinetiForge/Private/VehicleWheelRuntime.cpp), [VehicleWheelPhysics.cpp](../Source/KinetiForge/Private/VehicleWheelPhysics.cpp)

## Suspension and contact

The suspension stage queries the world for a contact using one of the configured line, sphere, box, or multi-sphere modes. It solves the selected linkage geometry to find hub position and orientation, then computes spring, damping, and optional anti-dive, anti-squat, and anti-roll effects. The resulting load contributes to chassis forces and to the load used by the tire calculation. Optional unsprung mass adds a separate suspension integration path.

The supported independent geometries are straight-line, MacPherson, and double wishbone; generated axle setups also support solid axle. These are runtime geometry calculations, not rendered-link simulations. Lookup curves can replace some detailed geometry inputs. For the exact stages and assumptions, see:

- Configuration: [VehicleSuspensionStructs.h](../Source/KinetiForge/Public/VehicleSuspensionStructs.h)
- Geometry, force, and unsprung-mass integration: [VehicleSuspensionSolver.cpp](../Source/KinetiForge/Private/VehicleSuspensionSolver.cpp)
- Contact queries and reactions: [VehicleSuspensionContactBridge.cpp](../Source/KinetiForge/Private/VehicleSuspensionContactBridge.cpp)

The contact-query bridge has an engine-version-specific threading constraint. This plugin began on UE 5.3 and currently uses the older `UWorld` physics-query path, whose `FHitResult` carries UObject references. The runtime copies the needed contact data for numeric consumers, but worker-thread code must not dereference the hit component or physical material references. Newer Unreal versions provide alternate physics-thread query and handle APIs that may allow this boundary to be replaced when integrated correctly; this code has not been migrated. See [Runtime Maintenance](RuntimeMaintenance.md) before changing this boundary.

## Engine and drivetrain

The power unit is a one-dimensional rotational model. The engine solver advances engine state using its torque curve, throttle, inertia, and internal losses. The clutch and gear path transfer torque between engine and gearbox; the gearbox applies the selected ratio. The transfer case distributes output among connected axles, and each axle differential distributes torque between that axle's left and right wheels. Torsional stiffness and damping are represented in the clutch/power path rather than by a 3D driveshaft mesh.

Useful entry points:

- Engine configuration and drivetrain values: [VehicleDrivetrainStructs.h](../Source/KinetiForge/Public/VehicleDrivetrainStructs.h)
- Engine state and torque: [VehicleEngineSolver.cpp](../Source/KinetiForge/Private/VehicleEngineSolver.cpp)
- Clutch and gear-path calculations: [VehicleTransmissionSolver.cpp](../Source/KinetiForge/Private/VehicleTransmissionSolver.cpp), [VehicleGearboxSolver.cpp](../Source/KinetiForge/Private/VehicleGearboxSolver.cpp)
- Differential and axle distribution: [VehicleDifferentialSolver.cpp](../Source/KinetiForge/Private/VehicleDifferentialSolver.cpp), [VehicleAxleSolver.cpp](../Source/KinetiForge/Private/VehicleAxleSolver.cpp)
- Whole-vehicle order and internal substeps: [VehicleDriveRuntime.cpp](../Source/KinetiForge/Private/VehicleDriveRuntime.cpp), [VehiclePowerUnitRuntime.cpp](../Source/KinetiForge/Private/VehiclePowerUnitRuntime.cpp)

The current power unit is single-clutch. More complex topologies such as a dual-clutch transmission require additional shared engine-load and output-composition work; see the extension boundaries in [Runtime Maintenance](RuntimeMaintenance.md).

## A practical way to change a model

1. Start from the relevant configuration struct and the corresponding solver listed above.
2. Trace where the solver's inputs are prepared and where its outputs are consumed using the [Source Guide](SourceGuide.md).
3. Change one behavior at a time and compare it in a controlled vehicle setup. The existing automation tests under `Source/KinetiForge/Private/Tests/` can show how specific behavior is checked, but they do not replace driving and tuning the affected setup in Unreal Editor.

For callback, thread, and lifetime constraints that apply across these models, read [Runtime Maintenance](RuntimeMaintenance.md).
