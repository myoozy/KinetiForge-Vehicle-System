# KinetiForge-Vehicle-System
High-performance, modular vehicle physics system for Unreal Engine 5, fully implemented in C++ with real-time tunable parameters.

一个高性能、模块化、物理自洽的载具系统，完全用 C++ 从零实现，专为 Unreal Engine 5 打造。

* Demonstration video 演示视频:
  - [key features 主要功能展示](https://www.bilibili.com/video/BV1HFnAzCEKU/)
  - [how to install 安装指南](https://www.bilibili.com/video/BV1SxJkzPEcA)
  - [how to configure vehicles 如何创建载具](https://www.bilibili.com/video/BV1h2JCzzEF3)

中文版在下方

✨ Key Features

  * High-Performance C++ Core – Written from scratch, runs much faster than Blueprint-based systems, supports async physics with excellent stability

  * Full Blueprint Support – All core functions are exposed to Blueprints, allowing you to build a complete custom drivetrain or extend features purely in Blueprints

  * Modular Architecture – Multiple wheels, multiple drivetrains, even complex multi-body vehicles in a single Actor

  * Highly Configurable – Almost every parameter can be adjusted at runtime; supports both static meshes and skeletal meshes (no skeleton binding required)

  * Physically-Driven Simulation

    - Tire forces based on force analysis, curve-tunable, supporting camber/toe effects, friction ellipse, combined slip

    - Suspension supports Telescopic Fork, MacPherson, double wishbone, trailing arm, with tunable camber/toe/caster change and anti-pitch/anti-roll geometry

    - Engine, clutch, gearbox, and differential are fully simulated with inertia, turbo, stall/reverse support, LSD

  * Driver Assistance Systems – Automatic gearbox, traction control, ABS, rev-matching, auto clutch, turbo anti-lag, auto-hold

🚀 Getting Started

  * Create a new empty c++ project
  
  * close the unreal editor and the visual studio (or other IDE)

  * find the project folder and create a new folder called 'Plugins'

  * delete the .sln file, right click on the .uproject file, click on "generate visual studio project files"

  * double click the .sln file to open IDE, then rebuild the project

  * Copy [CoreRedirects] in (Name of this plugin folder)/Config/DefaultEngine.ini to (Name of your project)/Config/DefaultEngine.ini

  * open the unreal project

  * Enable Async Physics in Project Settings

  * Set your physics timestep to ≤ 1/60 sec (recommended: 1/120 sec)

  * Open the example map and start tuning your vehicles!

🤝 Participation & Feedback
  * Everyone is welcome to raise questions, suggestions, or bug reports!

  - Submit issues or suggestions in Issues
    
  - Feel free to create Pull Requests (PRs) to improve the system together

🙏 Acknowledgements
  * This project is inspired by the following open-source projects/tutorials:

    - [Vehicle Components](https://www.youtube.com/watch?v=BaE3l4rNzJQ&list=PLsxE2KdYcv6vvrJFtRqN14xjrPt-v5fpd)
      – Helped me get started with UE5 blueprint vehicle system development
    - [AsyncTickPhysics](https://github.com/Mr-Craig/AsyncTickPhysics)
      – Used for implementing async physics

Special thanks to all developers who have provided suggestions and feedback!

📜 License

  * MIT License

以下是中文版：

✨ 特性亮点

 * 高性能 C++ 实现 – 全部从零编写，性能远超蓝图实现，支持异步物理，稳定可靠

 * 蓝图完全支持 – 核心函数全部暴露给蓝图，可用蓝图自行编写完整的传动系统或扩展功能

 * 模块化设计 – 支持多个车轮、多个传动系统，甚至可以在同一个 Actor 内实现多个车身的复杂载具

 * 灵活可扩展 – 几乎所有车辆参数可在游戏中实时修改，支持静态网格体和骨架网格体（无需绑定骨骼）

 * 真实物理驱动

   - 轮胎力基于合理的受力分析，可通过曲线微调，支持 camber/toe 影响、摩擦椭圆、联合滑移

   - 悬挂支持直线、麦弗逊、双叉臂、拖曳臂，参数可调，支持自定义 camber/toe 曲线，支持抗俯仰/侧倾悬挂

   - 引擎、离合器、变速箱、差速器均基于物理模拟，支持涡轮增压、熄火、反转、限滑差速器

   - 驾驶辅助功能 – 自动变速箱、牵引力控制、ABS、降档补油、自动离合、涡轮 AntiLag、AutoHold

🚀 快速开始

  * 创建一个新的空C++项目
  
  * 关闭虚幻编辑器和Visual Studio（或其他IDE）

  * 找到项目文件夹，创建名为'Plugins'的新文件夹

  * 删除.sln文件，右键点击.uproject文件，选择“generate visual studio project files”

  * 双击.sln文件打开IDE，然后重建项目

  * 打开虚幻项目

  * 在项目设置中启用异步物理

  * 将物理时间步长设置为≤1/60秒（推荐：1/120秒）

  * 打开示例地图，开始调试您的载具吧！

🤝 参与与反馈

 * 欢迎大家提出问题、建议或 Bug 反馈！

  * 在 Issues 提交问题或建议

  * 欢迎 PR 一起改进

🙏 致谢

 * 本项目受到以下开源项目/教程启发：

    - [Vehicle Components](https://www.youtube.com/watch?v=BaE3l4rNzJQ&list=PLsxE2KdYcv6vvrJFtRqN14xjrPt-v5fpd)
     – 帮助我入门 UE5 蓝图载具系统开发

    - [AsyncTickPhysics](https://github.com/Mr-Craig/AsyncTickPhysics)
     – 用于实现异步物理

感谢所有提供建议和反馈的开发者们！

📜 许可证

  * MIT License
