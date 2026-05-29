# KinetiForge-Vehicle-System

High-performance, modular vehicle physics system for Unreal Engine 5, fully implemented in C++ with real-time tunable parameters.

* Demonstration videos:
  - [Key features](https://www.bilibili.com/video/BV1HFnAzCEKU/)
  - [Installation guide](https://www.bilibili.com/video/BV1SxJkzPEcA)
  - [How to configure vehicles](https://www.bilibili.com/video/BV1h2JCzzEF3)

---

## ✨ Key Features

**Tires**  
- Physics-based force model following a slip‑friction curve (Pacejka‑style)  
- Automatic combined slip behavior similar to TMeasy  
- Relaxation length simulation and non‑linear tire load sensitivity  
- Quickly tunable longitudinal/lateral force distribution and handling feel parameters  

**Suspension**  
- Supports: **Straight‑axle** (like a mountain bike fork), MacPherson, double wishbone, solid axle  
- Suspension geometry is **fully solved in real time** – ideal for suspension tuning systems  
- Caster / camber / toe changes affect tire forces correctly  
- Anti‑dive / anti‑squat geometry naturally supported  
- Lookup tables available for all but solid axle (for quick tuning without detailed linkage setup)  

**Drivetrain (1D physics, implicit integration)**  
- Driveshaft torsional stiffness simulated  
- Internal physical sub-steps of the drivetrain and tires

**Engine**  
- Models starter motor, spark plugs, fuel injectors, turbocharger, etc.  
- Engine can stall and **reverse**; can be push‑started  
- Internal engine friction provides braking or holding torque  
- Turbo supports simple anti‑lag  
- Exhaust temperature and backfire effects (simple simulation)  

**Clutch**  
- Torsen‑spring style model – simulates torsional stiffness of both clutch input shaft and main driveshaft  

**Transmission**  
- Simple manual transmission (MT) with support for sequential and AMT logic  

**Differential**  
- Includes limited‑slip differential (LSD), also acts as transfer case / center differential  

**Electronics & Assistance**  
- Basic traction control, ABS, automatic gearbox logic  
- Open APIs for P1/P2/P3/P4 electric motors and ESP (not yet implemented)  

**Known Limitations**  
- No camber thrust  
- No angular momentum conservation → cannot properly simulate motorcycles or bicycles  

---

## 🚀 Migration Guide

> [!IMPORTANT]
> **If you are upgrading from an older version, be sure to follow the steps below; otherwise, all your Blueprint variables will be lost and cannot be recovered!**

> Because I occasionally rename classes/variables, follow these steps to prevent Blueprint data loss.

1. **CoreRedirects** – Manually copy the entire `[CoreRedirects]` section from the plugin’s `Config/DefaultEngine.ini` into your **project’s** `Config/DefaultEngine.ini`.  
   - Plugin file: `(YourProject)/Plugins/KinetiForge-Vehicle-System/Config/DefaultEngine.ini`  
   - Project file: `(YourProject)/Config/DefaultEngine.ini` (create if missing)

2. **Verify and finalize** – Open your project in Unreal Editor. **Make sure no errors appear**. If errors occur, double‑check that you copied `[CoreRedirects]` exactly (including all brackets and indentation). If the problem persists, please open an issue.  
   Once everything looks correct, **save any asset** (a Level or Blueprint) – this writes the redirects into the asset registry permanently.

3. **Variable access** – Many config variables are now `protected`. If a Blueprint error occurs, use the corresponding `SetXXXX` function instead.

---

## 💡 Philosophy: Anything Can Be a Vehicle

This system is highly decoupled and component‑based. You do not have to inherit from example blueprints:  
- Simply attach a `DriveAssembly` component under any primitive component of any Actor.

---

## 🚀 Getting Started (C++ Project)

1. Create a new **empty C++ project**.
2. Close Unreal Editor and Visual Studio (or your IDE).
3. In your project folder, create a new folder named `Plugins`.
4. Delete the `.sln` file, then right‑click your `.uproject` file and select **“Generate Visual Studio project files”**.
5. Double‑click the `.sln` file to open your IDE, then **rebuild** the project.
6. Follow the **Migration Guide** above to set up CoreRedirects.
7. Open the Unreal project.
8. In **Project Settings → Physics**, enable **Async Physics**.
9. Set your **physics timestep** to **≤ 1/60 sec** (recommended: 1/90 ~ 1/120 sec).  
   > ⚠️ Never go below 30 Hz (≥ 1/30 sec) – the simulation may become unstable or crash.
10. **Prefer Async Physics over sub‑stepping** – A fixed, slightly larger timestep (with Async Physics) is more stable than a variable smaller timestep. Async Physics > Physics Sub‑stepping.
11. Open the example map and start tuning your vehicles!

---

## 📖 How to Configure a New Vehicle

Detailed video tutorial: [How to configure vehicles](https://www.bilibili.com/video/BV1h2JCzzEF3)

### Basic Setup (Simplest Way)
1. Create a new Actor.  
   - For quick start, inherit from **`DefaultVehicle`** (provided by this plugin – sorry for the generic name).  
   - If you want to handle async physics in Blueprint, inherit from **`AsyncTickPawn`** (from the AsyncTickPhysics plugin).  
2. Add a primitive component (e.g., `StaticMeshComponent`) as the root.  
3. Add a **`DriveAssembly`** component and attach it under the root primitive.  
   > `DefaultVehicle` already does this for you.  
4. Inside `DriveAssembly`, assign:
   - Engine, Clutch, Gearbox, Transfer Case, Axle(s)  
5. Inside each **Axle**, assign:
   - Wheels, Differential  
6. Inside each **Wheel Component**, tune:
   - Tire parameters, suspension geometry (or lookup tables)  
7. **Collision & inertia** – Ensure your vehicle’s collision body **covers the suspension top mounts**. Too small a moment of inertia may cause the vehicle to fly away or behave erratically.

### Important Tuning Tips
- **LSD lock ratio** has a huge impact on handling – experiment with it.  
- **Multiple DriveAssemblies** – You can have several drive assemblies in one Actor. For example, place one assembly for front wheels and another for rear wheels if they are powered by different engines.  
- **Low‑level wheel/axle components** – `WheelComponent` and `AxleComponent` can be used standalone, without a `DriveAssembly`. In that case you must manually call `UpdatePhysics()` on them every tick, and they **must** run on the physics thread (e.g., triggered by `AsyncPhysicsTick`).

---

## 🏎️ Driving Feel & Tuning Tips

- **No aerodynamics in this system** – If a high‑powered rear‑wheel drive car feels uncontrollable, it’s **not** your tuning fault.  
  - Add **angular damping that increases with speed** (e.g., in the vehicle’s movement component).  
  - Apply **downforce that grows with speed** (manually or via a simple blueprint curve).  
  These two changes dramatically improve high‑speed stability and traction.

- **Input smoothing matters** – The rate at which you apply steering and throttle has a huge impact on handling. Use input curves or low‑pass filters to avoid instant, jerky inputs.

---

## 🤝 Participation & Feedback

All questions, suggestions, and bug reports are welcome!

- Open an issue or discussion in **Issues**
- Feel free to submit **Pull Requests**

---

## 🙏 Acknowledgements

This project is inspired by the following open‑source projects / tutorials:

- [Vehicle Components](https://www.youtube.com/watch?v=BaE3l4rNzJQ&list=PLsxE2KdYcv6vvrJFtRqN14xjrPt-v5fpd) – Helped me get started with UE5 Blueprint vehicle systems  
- [AsyncTickPhysics](https://github.com/Mr-Craig/AsyncTickPhysics) – Used for async physics implementation  

Special thanks to everyone who has provided suggestions and feedback!

---

## 📜 License

MIT License

---

# 中文版

✨ 特性亮点

**轮胎**  
- 基于滑移‑摩擦系数的力学曲线（Pacejka 风格），自动实现类似 TMeasy 的联合滑移  
- 模拟松弛长度、轮胎载荷的非线性影响  
- 提供可快速调节纵向/侧向力分配和操控手感的参数  

**悬挂**  
- 支持：直轴式（如山地车前叉）、麦弗逊、双叉臂、整体桥  
- 悬挂几何完全实时解算 → 完美支持悬挂改装系统  
- Caster / Camber / Toe 变化会真实影响轮胎受力  
- 自带抗俯仰/抗下沉几何特性  
- 除整体桥外，均可使用查表方式快速调参（无需详细连杆数据）  

**传动系统**  
- 1D 物理，隐式积分，模拟传动轴扭转刚度  
- 传动系统和轮胎的内部物理子步

**引擎**  
- 模拟启动机、火花塞、喷油嘴、涡轮增压器等  
- 可熄火、可反转、可推车启动  
- 引擎内部阻力可用于减速或停车  
- 涡轮支持简单 Anti‑Lag，排气管支持简单的温度和回火模拟  

**离合器**  
- 托森弹簧模型，同时模拟离合器输入轴和主传动轴的扭转刚度  

**变速箱**  
- 手动变速箱（MT），支持序列式变速箱和 AMT 逻辑  

**差速器**  
- 支持限滑差速器（LSD），同时承担分动箱/中央差速器角色  

**电控与辅助**  
- 基础的牵引力控制、ABS、自动变速箱逻辑  
- 开放 P1/P2/P3/P4 电机 API 以及 ESP 预留接口（尚未实现）  

**已知不足**  
- 不支持 Camber Thrust  
- 未模拟角动量守恒 → 无法正确模拟摩托车或自行车  

---

🚀 迁移指南

> [!超级无敌重要]
> **如果你正在从旧版本升级，请务必执行以下步骤，否则你的蓝图变量将全部丢失且无法恢复！**
> 我偶尔会重命名类或变量，请务必按以下步骤操作，防止蓝图数据丢失。

1. **配置重定向（CoreRedirects）**  
   将插件目录下 `Config/DefaultEngine.ini` 中的 **整个 `[CoreRedirects]` 段落**，手动复制到你项目的 `Config/DefaultEngine.ini` 中。  
   - 插件文件位置：`(你的项目)/Plugins/KinetiForge-Vehicle-System/Config/DefaultEngine.ini`  
   - 项目文件位置：`(你的项目)/Config/DefaultEngine.ini`（若没有则新建）

2. **验证并生效**  
   复制完成后，打开虚幻编辑器，**确保没有报错**。如果出现错误，检查 `[CoreRedirects]` 是否一字不落地复制了（包括所有括号和缩进）。若仍有问题，欢迎提 Issue。  
   确认无误后，**保存任意资产**（关卡或蓝图），这会将重定向写入项目的资产注册表，永久生效。

3. **变量权限变更**  
   很多配置变量已改为 `protected`。如果蓝图报错，请改用对应的 `SetXXXX` 函数。

---

💡 核心理念：万物皆可为车

本系统采用高度解耦的组件化设计，你**不需要**继承示例蓝图：  
- 只需在任意 Actor 的根组件下挂载 `DriveAssembly` 组件即可。

---

🚀 快速开始（C++ 项目）

1. 创建一个**新的空 C++ 项目**。  
2. 关闭虚幻编辑器与 Visual Studio（或其它 IDE）。  
3. 在项目根目录下新建文件夹，命名为 `Plugins`。  
4. 删除项目目录下的 `.sln` 文件。  
5. 右键点击 `.uproject` 文件 → 选择 **“Generate Visual Studio project files”**。  
6. 双击生成的 `.sln` 文件打开 IDE，然后**重新生成（Rebuild）** 整个项目。  
7. 按照上面的**迁移指南**配置 CoreRedirects。  
8. 打开虚幻项目。  
9. 在 **项目设置 → 物理** 中，启用 **异步物理（Async Physics）**。  
10. 将 **物理时间步长** 设置为 ≤ 1/60 秒（推荐 1/90 ~ 1/120 秒）。  
    > ⚠️ 切勿低于 30 Hz（≥ 1/30 秒），否则物理模拟可能不稳定或崩溃。  
11. **异步物理优于物理子步**：固定步长（即使略大）比不固定的小步长更稳定。稳定性排序：异步物理 > 物理子步。  
12. 打开示例地图，开始调校你的载具！

---

📖 如何配置一辆新车

详细视频教程：[如何创建载具](https://www.bilibili.com/video/BV1h2JCzzEF3)

### 基础配置（最简单的方式）
1. 新建一个 Actor。  
   - 快速上手：继承自 **`DefaultVehicle`**（插件自带的，名字起得随意，见谅）。  
   - 如果想在蓝图中处理异步物理，可以继承自 **`AsyncTickPawn`**（来自 AsyncTickPhysics 插件）。  
2. 添加一个基础组件（比如 `StaticMeshComponent`）作为根。  
3. 添加一个 **`DriveAssembly`** 组件，并将其附加到根基础组件下。  
   > `DefaultVehicle` 已经帮你做了这一步。  
4. 在 `DriveAssembly` 中指定：
   - 引擎、离合器、变速箱、分动箱、车轴  
5. 在每个 **车轴（Axle）** 中指定：
   - 车轮、差速器  
6. 在每个 **车轮组件（WheelComponent）** 中调校：
   - 轮胎参数、悬挂几何（或查表）  
7. **碰撞体与转动惯量**：确保车身碰撞体**包裹住悬挂塔顶**。转动惯量太小，车辆可能直接飞掉或行为异常。

### 重要调校提示
- **限滑差速器（LSD）的锁止率** 对手感影响巨大 – 建议多试试不同的数值。  
- **多个 DriveAssembly** – 一个 Actor 里可以有多个传动系统。例如：前轮用一个发动机驱动，后轮用另一个发动机驱动，就分别建立两个 DriveAssembly 并分配给前后轴。  
- **单独使用车轮/车轴组件** – `WheelComponent` 和 `AxleComponent` 可以不挂载到 `DriveAssembly` 下单独使用。这种情况下你需要每帧**手动调用 `UpdatePhysics()`**，并且它们**必须在物理线程上运行**（例如通过 `AsyncPhysicsTick` 触发）。

---

🏎️ 驾驶手感调校技巧

- **本系统未模拟空气动力学** – 如果大马力后驱车感觉很难开，**不是你调教得不好**。  
  - 给车辆添加**随速度增大的角阻尼**（例如在 MovementComponent 中设置）。  
  - 施加**随速度增大的下压力**（手动控制或简单的蓝图曲线）。  
  这两项改动会极大改善高速稳定性和后轮牵引力。

- **输入平滑很重要** – 转向和油门的施加速度对手感影响巨大。请使用输入曲线或低通滤波器，避免瞬间、跳跃式的输入变化。

---

🤝 参与与反馈

欢迎大家提出问题、建议或 Bug 反馈！

- 在 Issues 提交问题或建议  
- 欢迎提交 Pull Requests

---

🙏 致谢

本项目受到以下开源项目 / 教程的启发：

- [Vehicle Components](https://www.youtube.com/watch?v=BaE3l4rNzJQ&list=PLsxE2KdYcv6vvrJFtRqN14xjrPt-v5fpd)  
  帮助我入门 UE5 蓝图载具系统开发  
- [AsyncTickPhysics](https://github.com/Mr-Craig/AsyncTickPhysics)  
  用于实现异步物理

感谢所有提供建议和反馈的开发者们！

---

📜 许可证

MIT License