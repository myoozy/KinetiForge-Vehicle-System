# Getting Started

This guide takes you from installing the plugin to opening its example vehicle. The plugin contains both C++ runtime modules and Unreal content, so copy the complete plugin folder and keep its directory structure intact.

## Requirements

- An Unreal Engine **C++ project**.
- **Engine versions:** UE 5.3 is the development baseline. UE 5.7 was recently re-tested and worked on the maintainer's setup. UE 5.5, 5.6, and 5.8 passed earlier tests but have not been recently re-verified; UE 5.4 has not been confirmed. The plugin is not tested against every engine version, so other versions may need adjustments.
- **Async Physics** enabled in **Project Settings → Physics**. The normal `DriveAssembly` path schedules its work through the plugin's physics integration and expects the physics callback execution path.
- A C++ build toolchain supported by your installed Unreal Engine. On Windows, this normally means Visual Studio with the C++ game development tools installed. If Unreal reports a missing compiler or SDK, install the version required by that engine installation.

## Get the plugin files

On the repository's GitHub page, choose **Code → Download ZIP** (the same source archive shown in the installation video), then extract it. The archive contains the repository; use its `KinetiForgeVehicles` plugin folder. If the extracted folder has a different name, it is fine to rename it, as long as `KinetiForge.uplugin`, `Source/`, `Content/`, and `Config/` are directly inside the folder you copy into your project's `Plugins/` directory. The repository does not require a separate Release package for this installation method.

## Install and open the example

1. Close Unreal Editor and your IDE.
2. Create `<YourProject>/Plugins/` if it does not exist.
3. Copy the complete `KinetiForgeVehicles` folder to `<YourProject>/Plugins/KinetiForgeVehicles/`. At that destination, `KinetiForge.uplugin`, `Source/`, `Content/`, and `Config/` should be directly inside the folder. Copying only `Source/` is not sufficient.
4. Regenerate project files for your `.uproject`, then build the project's Editor target with your installed Unreal Engine version.
5. Open the project and enable **Async Physics** in **Project Settings → Physics**.
6. In the Content Browser settings, enable **Show Plugin Content**.
7. Open `/KinetiForge/Template/Maps/KinetiForgeExampleMap` (file: `Content/Template/Maps/KinetiForgeExampleMap.umap`) and run the level. It is a simple demonstration scene with a few speed bumps; use the included vehicle and controller/input setup to try the vehicle and suspension over them.

The example includes `DefaultVehicle` and other vehicle examples. `KFVehicleGameMode` and `KFVehicleController` in `Content/Template/` demonstrate one way to connect a game mode and controller; they are not required architecture for your own game. Example assets are demonstrations, not guaranteed best-practice configurations.

For a plugin upgrade, first back up the project and follow the [migration notes in the README](../README.md#upgrading-from-an-older-version). Check and resave affected assets after verifying the migration.

## How a vehicle is assembled

A vehicle can be any Actor with a physics body. Attach a `DriveAssembly` (`UVehicleDriveAssemblyComponent`) below a primitive component that represents the chassis. You do not need to inherit from `DefaultVehicle`.

The Actor owns the components. Their **scene attachment** and **runtime configuration** are separate:

| Component | Unreal component type | Relationship to the chassis body |
| --- | --- | --- |
| Chassis body | `UPrimitiveComponent` | The physics body used by the vehicle. |
| `DriveAssembly` | `USceneComponent` | Attach it under the chassis primitive. Initialization finds the nearest primitive parent (or falls back to the Actor root); it does not create a drivetrain component hierarchy. |
| Axle component(s) | `USceneComponent` | Initialization attaches each axle directly to the chassis primitive. |
| Wheel component(s) | `USceneComponent` | Initialization attaches each wheel directly to the chassis primitive, including wheels used by an axle. |
| Engine, clutch, gearbox, transfer case, differential | `UActorComponent` | These components have no scene attachment. `DriveAssembly` and axle settings reference/select them. |

In short, the chassis, `DriveAssembly`, axles, and wheels use the same physics body; the axle and wheel components are not nested under one another. The drivetrain's logical order is engine → clutch → gearbox → transfer case → axles/wheels, but that order is not a SceneComponent hierarchy.

This behavior is implemented in [`FindPhysicalParent`](../Source/KinetiForge/Private/VehicleUtilities.cpp), `DriveAssembly::OnRegister` and axle generation ([VehicleDriveAssemblyComponent.cpp](../Source/KinetiForge/Private/VehicleDriveAssemblyComponent.cpp)), axle/wheel initialization ([VehicleAxleAssemblyComponent.cpp](../Source/KinetiForge/Private/VehicleAxleAssemblyComponent.cpp), [VehicleWheelComponent.cpp](../Source/KinetiForge/Private/VehicleWheelComponent.cpp)).

`DriveAssembly` can use existing engine, clutch, gearbox, and transfer-case components on the Actor, or create them from configured class fields. Axle templates select an existing axle or configure a generated axle; each axle can use existing wheel components or create wheels from a wheel class. An axle template's `AxleClass` provides configuration; the generated axle itself is the base `UVehicleAxleAssemblyComponent`. The differential is configured on each axle, and the transfer case is a separate drivetrain component.

### Minimal setup on a new Actor

1. Create an Actor Blueprint with a primitive chassis component as its root. Enable **Simulate Physics** and set up collision for the body.
2. Add a `DriveAssembly` scene component and attach it under the chassis. Leave its existing-component switches off to have it create the default engine, clutch, gearbox, and transfer case.
3. In **DriveAssembly → Axle Templates**, add two entries, one for the front axle and one for the rear. Leave **Use Existing Component** off; leave **Axle Class** and **Wheel Class Override** empty to use the default axle and wheel components. Set each **Axle Position** to the corresponding axle center under your chassis. These are local Unreal coordinates in centimeters; the usual vehicle convention is X forward, Y right, Z up. The default axle track width is 160 cm, so adjust it to match your vehicle using an axle class or **Track Width Override**.
4. Add a camera and input setup as needed. For direct player possession, create the vehicle Blueprint as a `Pawn`; a plain `Actor` can still host the vehicle components, but an existing Pawn or PlayerController must hold a reference to its `DriveAssembly` and forward input calls. Start with the example map's vehicle and controller if you want to compare a working setup before building your own.

This creates a basic four-wheel starting point, not a tuned vehicle or a guarantee of stable, drivable behavior. Align the axle positions and track width with the chassis collision and any wheel meshes you assign, then verify contact, steering, and propulsion in the Editor and tune the wheel, suspension, engine, and gearbox settings for your vehicle. The default wheel radius is 33 cm; use wheel settings that match the scale of your model.

## Chassis and physics setup

- Enable **Simulate Physics** on the primitive chassis body and give it suitable collision. Check that the collision body covers the suspension mount locations.
- The maintainer's suggested Async Physics timestep is no greater than **1/60 s** (at least 60 Hz); **1/90–1/120 s** are suggested tuning values. These are guidance, not guarantees for every project or vehicle.
- Optional unsprung-mass simulation requires **120 Hz Async Physics**, according to the current implementation guidance.
- Too little chassis moment of inertia can make a vehicle unstable or appear to fly away. Review the body's mass and inertia setup if the vehicle behaves erratically.

## Connect input

`DriveAssembly` updates the configured assembly automatically by default. Its public input entry points include `InputThrottle`, `InputBrake`, `InputSteering`, `InputClutch`, and `InputHandbrake`, as well as `ShiftUp`, `ShiftDown`, and engine start/shut methods.

For Enhanced Input, get the vehicle's `DriveAssembly` reference in your controller and pass the action value to the matching function on **Triggered** (or while an axis is active). Throttle, brake, clutch, and handbrake take values from **0 to 1**; steering takes **-1 to 1**. For example, a throttle trigger should call `InputThrottle(Value)` and a steering axis should call `InputSteering(Value)`. On **Completed** or **Canceled**, call the same function with `0` so the input is released; for a digital button, send `1` on **Started** and `0` on release. Map shift buttons to `ShiftUp` / `ShiftDown`, and an engine toggle to the engine start/shut functions. The sign used for left and right steering depends on how the input axis is mapped. The example's Enhanced Input assets are under `Content/Template/Input/`; inspect its controller Blueprint for one implementation.

Smooth steering and throttle changes usually produce more controllable behavior than instant input jumps. Input curves or low-pass filtering can help tune the response.

### Networking scope

The component declares reliable Server and NetMulticast RPCs for throttle, brake, clutch, steering, and handbrake input. Gear changes use server and multicast RPCs, with `ServerCurrentGear` replicated as a correction state. These hooks do not provide complete network vehicle physics: wheel and suspension state replication and a full prediction/reconciliation system are outside this interface. The engine-operation mode property declares an `OnRep` callback, but is not registered in `GetLifetimeReplicatedProps`; do not rely on that property replicating as written. Configure network ownership and actor replication for your project separately.

## Common setup checks

- **The plugin's assets are missing in the Content Browser:** enable **Show Plugin Content**.
- **The plugin does not build or load:** confirm the full plugin folder is under the project's `Plugins/` directory, regenerate project files, and build with an engine version listed under [Requirements](#requirements).
- **The example or vehicle does not respond as expected:** confirm Async Physics is enabled, the chassis primitive has **Simulate Physics** enabled, and the example controller/input wiring is active.
- **The vehicle behaves erratically:** check chassis collision coverage around suspension mounts and review its mass and inertia setup.

## Using wheel or axle components without DriveAssembly

Standalone `WheelComponent` and `AxleAssemblyComponent` do not update physics by themselves. If you use them outside a `DriveAssembly`, call their manual physics update from `AsyncPhysicsTick` or another valid physics callback. Ordinary Actor Tick is not a physics callback. For a manually updated `DriveAssembly`, disable `bUpdatePhysicsAutomatically` and call `UpdatePhysics` only while already executing in that scene's physics callback.

The maintainer's [installation video on YouTube](https://youtu.be/Ajve3ndHV04) and [Bilibili](https://www.bilibili.com/video/BV1SxJkzPEcA) walk through setup. The [vehicle-configuration video on YouTube](https://youtu.be/pHuFyCRVlZM) and [Bilibili](https://www.bilibili.com/video/BV16DjA6XEQL/) show a configuration walkthrough. A parameter-by-parameter tuning video is not currently linked.

For the C++ layer map and physics-step flow, continue to the [source guide](SourceGuide.md). For callback, lifecycle, and thread-ownership constraints, see the [runtime maintenance guide](RuntimeMaintenance.md).
