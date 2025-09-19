# KinetiForge-Vehicle-System
High-performance, modular vehicle physics system for Unreal Engine 5, fully implemented in C++ with real-time tunable parameters.

* Demonstration video:

✨ Key Features

  * High-Performance C++ Core – Written from scratch, runs much faster than Blueprint-based systems, supports async physics with excellent stability

  * Full Blueprint Support – All core functions are exposed to Blueprints, allowing you to build a complete custom drivetrain or extend features purely in Blueprints

  * Modular Architecture – Multiple wheels, multiple drivetrains, even complex multi-body vehicles in a single Actor

  * Highly Configurable – Almost every parameter can be adjusted at runtime; supports both static meshes and skeletal meshes (no skeleton binding required)

  * Physically-Driven Simulation

    - Tire forces based on force analysis, curve-tunable, supporting camber/toe effects, friction ellipse, combined slip

    - Suspension supports Telescopic Fork, MacPherson, double wishbone, trailing arm, with adjustable stiffness, damping, preload

    - Engine, clutch, gearbox, and differential are fully simulated with inertia, turbo, stall/reverse support, LSD

  * Driver Assistance Systems – Automatic gearbox, traction control, ABS, rev-matching, auto clutch, turbo anti-lag, auto-hold

🚀 Getting Started

  * Copy the plugin to your Unreal project’s Plugins/ folder

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
      – Helped me get started with UE5 vehicle system development
    - [AsyncTickPhysics](https://github.com/Mr-Craig/AsyncTickPhysics)
      – Used for implementing async physics

Special thanks to all developers who have provided suggestions and feedback!

📜 License

  * Released under MIT License – free for commercial use, attribution required.
