# KinetiForge Vehicles

A modular, C++ vehicle physics plugin for Unreal Engine 5. It combines Blueprint-configurable components with a native runtime for drivetrain, suspension, tire, and contact simulation.

**Engine versions:** UE 5.3 is the development baseline. UE 5.7 was recently re-tested and worked on the maintainer's setup. UE 5.5, 5.6, and 5.8 passed earlier tests but have not been recently re-verified; UE 5.4 has not been confirmed. The plugin is not tested against every engine version, so other versions may need adjustments.

## Start here

- **New user:** [Getting Started](Docs/GettingStarted.md) — install the plugin, run the example, and assemble a vehicle.
- **Reading the C++ code:** [Source Guide](Docs/SourceGuide.md) — understand the main layers and follow a physics step.
- **Changing the physics:** [Physics Models](Docs/PhysicsModels.md) — a conceptual map of tire, suspension, and drivetrain calculations with source entry points.
- **Changing runtime code:** [Runtime Maintenance](Docs/RuntimeMaintenance.md) — thread ownership, lifecycle, and the project's existing validation workflow.
- **中文读者：** [快速开始](Docs/GettingStarted.md)（目前文档主体为英文）；组件配置和迁移提示也见下文。

## What it includes

- Tire forces with combined slip, relaxation length, and load sensitivity.
- Runtime suspension geometry for straight axle, MacPherson, double wishbone, and solid axle setups; lookup-table options are available for most types.
- A 1D drivetrain with engine, clutch, gearbox, and differential components.
- Basic traction control, ABS, and automatic gearbox logic.
- Simple airfoil and aerodynamic-volume components.

Wheel spin exchanges angular momentum with the chassis, including acceleration, braking and changes of wheel-axis direction. This uses each wheel's own inertia; upstream drivetrain axes are not modeled. Known limitations include no direct camber-thrust force model and no validated motorcycle or bicycle setup. P1/P2/P3/P4 motor APIs and ESP are not implemented.

## Build a basic vehicle

Any Actor can host a vehicle. Add a physics-enabled primitive body, attach a `DriveAssembly` below it, then configure or generate its engine, clutch, gearbox, transfer case, axles, wheels, and differential. `DefaultVehicle` is a ready-made example, not a required base class.

The assembly updates automatically by default. For standalone wheel or axle components, use their manual update from a physics callback; ordinary Actor Tick is not a physics callback. See [Getting Started](Docs/GettingStarted.md) for setup details and the example map.

## Upgrading from an older version

Class or property names may change between releases. Before opening an upgraded project, back it up and copy the complete `[CoreRedirects]` section from the plugin's `Config/DefaultEngine.ini` into the project's `Config/DefaultEngine.ini` (create the project file if needed). Open the project and resolve any reported asset errors; then inspect and resave affected assets. Many configuration properties are now `protected`; use their corresponding `SetXXXX` functions when Blueprint access requires it.

## Videos

- [Installation (YouTube)](https://youtu.be/Ajve3ndHV04) · [Bilibili](https://www.bilibili.com/video/BV1SxJkzPEcA)
- [Vehicle configuration (YouTube)](https://youtu.be/pHuFyCRVlZM) · [Bilibili](https://www.bilibili.com/video/BV16DjA6XEQL/)
- [Demonstration (YouTube)](https://www.youtube.com/watch?v=iipOBL2fKQ0) · [Bilibili](https://www.bilibili.com/video/BV1HDjA69EGE/)

## 中文简介

KinetiForge Vehicles 是面向 Unreal Engine 5 的模块化 C++ 载具物理插件。新用户请从[快速开始](Docs/GettingStarted.md)入门；希望理解代码结构的读者可看[源码导读](Docs/SourceGuide.md)，想了解轮胎、悬挂和传动计算的开发者可看[物理模型概览](Docs/PhysicsModels.md)，维护运行时的开发者可看[运行时维护指南](Docs/RuntimeMaintenance.md)。

车辆不必继承 `DefaultVehicle`：在任意 Actor 上准备启用物理模拟的基础碰撞体，在其下添加 `DriveAssembly`，再配置或生成动力总成、车轴和车轮。插件默认自动更新整车；单独使用车轮或车轴时，应在物理回调中手动更新。

升级旧项目前请先备份，并将插件 `Config/DefaultEngine.ini` 中完整的 `[CoreRedirects]` 段复制到项目配置中。打开项目后检查报错，并检查、重新保存受影响的资产。蓝图若无法访问受保护的配置变量，请使用对应的 `SetXXXX` 函数。

**引擎版本：** UE 5.3 是开发基线；维护者近期在自己的环境中复测了 UE 5.7，运行正常。UE 5.5、5.6、5.8 以前通过过测试，但近期没有重新验证；UE 5.4 尚未确认。插件没有覆盖所有引擎版本的测试，其他版本可能需要调整。

## Contributing and license

Questions, bug reports, and pull requests are welcome. The plugin is distributed under the MIT License; see the license file in the repository.
