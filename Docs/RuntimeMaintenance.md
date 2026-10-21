# Vehicle runtime maintenance guide

This UE5.3 plugin simulates a configured drivetrain assembled from existing vehicle components. Components remain the asset, Blueprint, configuration and editor-facing API. The normal Drive simulation runs through ordinary C++ runtimes. Existing assets do not need a new drivetrain graph.

Start with [VehicleDriveRuntime.cpp](../Source/KinetiForge/Private/VehicleDriveRuntime.cpp). Its `Update` function is the complete vehicle physics sequence. Read the layers below when changing a particular behavior; the development audit is not required reading.

## One physics step

```text
Chaos callback -> scene schedule -> admitted physics step -> DriveRuntime::Update
  validate all required bodies; consume power/input settings and commands
  axle PreStep: consume wheel/axle inputs, prepare suspension and wheels
  drivetrain substeps:
    input assistance -> engine/output torque -> transfer case -> axles/wheels
    -> clutch feedback for the next substep
  axle PostStep: finalize wheels, apply chassis and ground reactions, publish results
  aggregate vehicle telemetry; publish power-unit results
```

Suspension preparation/integration and tire/drivetrain integration remain separate stages. Paired independent suspension integrates both unsprung masses together through local substeps; a single-wheel fallback remains available. WheelCoordinator manages wheels associated with a body but does not advance the drivetrain. Drive owns that update, including bindings to more than one body.

## Where to make changes

| Change | Start here |
| --- | --- |
| Vehicle assembly, component discovery, editor preview, binding | [VehicleDriveAssemblyComponent.cpp](../Source/KinetiForge/Private/VehicleDriveAssemblyComponent.cpp) |
| Whole-vehicle ordering and transfer-case distribution | [VehicleDriveRuntime.cpp](../Source/KinetiForge/Private/VehicleDriveRuntime.cpp) |
| Engine, clutch and gear-path composition | [VehiclePowerUnitRuntime.cpp](../Source/KinetiForge/Private/VehiclePowerUnitRuntime.cpp), then the corresponding solver |
| Default input assistance | [VehicleInputSolver.cpp](../Source/KinetiForge/Private/VehicleInputSolver.cpp) |
| Axle stages and feedback | [VehicleAxlePhysicsAssembly.cpp](../Source/KinetiForge/Private/VehicleAxlePhysicsAssembly.cpp), then VehicleAxleSolver |
| Suspension, limits and anti-roll coupling | [VehicleSuspensionSolver.cpp](../Source/KinetiForge/Private/VehicleSuspensionSolver.cpp) |
| Wheel/tire integration and suspension handoff | [VehicleWheelRuntime.cpp](../Source/KinetiForge/Private/VehicleWheelRuntime.cpp), [VehicleWheelPhysics.cpp](../Source/KinetiForge/Private/VehicleWheelPhysics.cpp) |
| UE5.3 queries and contact reactions | [VehicleSuspensionContactBridge.cpp](../Source/KinetiForge/Private/VehicleSuspensionContactBridge.cpp) |
| Configuration, command and result transfer | VehiclePowerUnitBridge, VehicleAxleBridge, VehicleWheelBridge |
| Callback admission, teardown and body identities | VehiclePhysicsScene, VehiclePhysicsSchedule, VehiclePhysicsStep, VehiclePhysicsBodyBinding |

## State and thread ownership

The game thread owns component configuration, Blueprint interaction, preview and display caches. Bridges transfer value snapshots and commands to a single simulation consumer, then publish result copies back. Shared ownership keeps a bridge alive; it does not permit simultaneous arbitrary access to its runtime. Physics integration and queries execute outside transfer locks.

Default Drive updates run on the worker callback. The existing thread selection also supports the actual UE Frozen GameThread callback. Aero and spring-arm helpers use Frozen GT. ATP remains available for existing Blueprint dependencies.

Configuration setters submit snapshots; physics consumes them at a step boundary. GT Tick also captures settings to preserve existing direct configuration changes. Curve/LUT regeneration follows the component's existing cache update rules. Do not replace this with setter-only dirty flags without accounting for those other mutation paths.

GT getters read published display state. Supported synchronous manual wheel/axle calls can read the current simulation result for same-step feedback. This compatibility path is not permission to call component getters from arbitrary workers during destruction. Preserve old reflected getters and manual interfaces: external Blueprints may use them.

The normal Drive runtime does not discover components during integration. UE5.3 still requires UObject access for the UWorld query entry and hit component/material handling, including immediate ground reactions. These exceptions are concentrated in contact adaptation; the system does not claim complete UObject isolation. Moving ground reactions to a later GT frame would change the simulation.

Native C++ extensions that explicitly include `VehicleFixedDriveRuntime.h` or refer to `FVehicleFixedDriveRuntime` should update to `VehicleDriveRuntime.h` and `FVehicleDriveRuntime`. Reflected UObject fields and Blueprint interfaces are unchanged.

## Registration and lifetime

OnRegister still prepares editor-visible assembly and preview. Runtime registration waits for valid dependencies and initialized settings, claims ownership, then enables the step. Missing dependencies leave it pending rather than invoking an old physics backend.

Step closures own C++ runtime data instead of capturing the Drive UObject. Unregistering or invalidating a binding closes admission and drains active execution before releasing ownership. Never drain a step from inside its own execution. Rebuilding creates a new executable step; a stopped old step is not reopened.

GT captures body identities. Physics resolves those identities against the current solver. A stack-local resolver shares lookups only within one vehicle's current callback; borrowed Chaos handles must never survive that callback. World/scene cleanup also closes and drains registered work.

## Compatibility and extension limits

The old complete vehicle backend is removed. Deprecated backend-selection fields only preserve serialized compatibility. Standalone wheels and axles retain explicit synchronous manual updates; they do not gain automatic scheduling. Whole-Drive manual updates require the supported physics callback context, not an ordinary GT Tick or an arbitrary native event.

Gear changes retain the component's GT timer and completion event semantics. Physics receives the actual gear index/ratio state. Completion means the GT timer completed, not that PT has acknowledged engagement. Existing delegate replacement/reentrancy limitations remain separate follow-up work.

The current power unit is single-clutch. Separate value-based clutch states and gear-path functions allow a future odd/even DCT implementation without replacing asset components. DCT still needs shared engine-load/inertia handling and output composition; duplicating the entire current power unit is not sufficient. DCT implementation is explicitly deferred. Free topology, DCT control and Blueprint control callbacks are outside the current extension scope.

## Validation when changing code

Build the Editor target and run `Automation RunTests KinetiForge`; the current suite contains 86 tests. Compile Shipping when changing code affected by build guards. Frozen reference data under [Tests/Baselines/VehicleRuntime-v1](../Source/KinetiForge/Private/Tests/Baselines/VehicleRuntime-v1) is read-only evidence, not an output to regenerate when tests fail. Test runtime/lifetime changes in the actual callback path, and exercise affected assets in the editor.

Also check a default Blueprint-only host project: its generated target can use C++17 while a V4 C++ project uses C++20. Keep aggregate initialization compatible with C++17 and put each implementation's matching header first, before profiling or other utility headers. Building only the development project's Editor/Shipping targets does not cover this difference.

User testing has confirmed that the multi-section BUS runs correctly and that a packaged build is drivable; the tested configuration and duration were not specified, so this does not establish coverage for every vehicle or configuration. Existing input replication, recorder keyframe capture and spawn/destroy with normal frame-end garbage collection were also confirmed. A clean World Partition map with an extra-long box road and three simulated, non-player vehicles was tested: the player drove away until those vehicles received EndPlay, then returned, with no issue or edge crash observed so far. The run did not count or identify components during streaming, so it does not establish strict absence of duplicates or missing components. Separately, an old example map has reported corner crashes and loss of ground collision; a UE5.3 Chaos vehicle comparison also lost collision, while UE5.7 did not reproduce it. Available evidence does not establish a single cause. Known issues also include recorded wheel rotation interpolation and a reported 1000Hz PIE exit hang with audio.

Use Insights scopes in the actual runtime. Parent scopes include their children; do not add both. Current input curve capture measurements are small enough to leave the straightforward snapshot path intact. Optimize a measured cost rather than introducing another cache or backend speculatively.
