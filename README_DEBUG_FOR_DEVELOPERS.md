
# README_DEBUG_FOR_DEVELOPERS.md

## 🐜 Developer-Focused Overview

This document provides technical details for developers actively working on and debugging the Ant Simulation project. It expands on the standard `README.md` with a focus on the build system, dependency management, and debugging environments. The project's purpose is to simulate an ant colony in search of food using SFML for graphics.

#### Key Technical Stack
* **Language**: C++17
* **Build System**: CMake (version 3.28 or newer)
* **Graphics**: SFML 2.6 (managed via CMake's `FetchContent`)
* **Includes**:  Vcpkg package manager for easy open-source library importing via `vcpkg.json`
* **Author**: Logan Herrera

---

## 🛠️ Deep Dive: Build System & Dependencies

This project uses a cross-platform CMake configuration designed for consistency across Windows and Linux.

### CMake Configuration

* **`CMakePresets.json`**: This file is the primary driver for configuration. It ensures that all developers use the same settings. The `default-windows` preset is configured for Visual Studio 2022. Visual Studio automatically detects and uses this preset when you open the project folder.
* **`FetchContent` for SFML**: The project does **not** require a manual installation of SFML. CMake's `FetchContent` module automatically downloads and builds SFML version 2.6 from source during the CMake configuration step.
    * On Linux, you still need to install SFML's own dependencies (e.g., `libgl1-mesa-dev`, `libopenal-dev`, etc.). A helper script, `setup_linux_dependencies.sh`, is provided to do this.
* **Resource Management**: To simplify debugging and local execution, essential resources are copied directly into the top-level `build/` directory during the build process. This includes:
    * `Vertiky.ttf` (font)
    * `build_verification.txt`
* **Build Verification**: A `build_verification.txt` file is generated upon a successful build. This is used to ensure code integrity, especially for contributions. It is part of the automated build verification hash generation feature in Github Actions Workflow.

### Dependency Management

* **Vcpkg (Windows)**:
    * Vcpkg is used to manage dependencies on Windows. It must be bootstrapped and integrated with Visual Studio (`vcpkg integrate install`) for the project to build correctly.
    * The `vcpkg.json` file is included to automatically install any listed libraries when CMake runs. While currently empty, it is set up for future dependencies. The mention of `openssl` in the documentation is purely an example and is not a project requirement.
* **System Libraries (Linux)**:
    * On Linux, dependencies required by SFML must be present on the system. The manual installation commands for Debian/Ubuntu, Fedora, and Arch are provided for this purpose.

---

## 🚀 Debugging Guide

Correctly configuring the debugger's working directory is crucial for the application to find its resources.

### Working Directory
The application executable expects its working directory to be the **top-level `build/` folder**. Both the font (`Vertiky.ttf`) and verification file are copied here, and the executable is designed to look in this location.

### Windows (Visual Studio 2022)
* **Configuration**: When using the `Windows Default Build` preset, Visual Studio automatically sets the correct working directory for debugging.
* **Startup Project**: Ensure the `main` target is set as the "Startup Project" in the Solution Explorer.
* **Running**: You can run the simulation directly by pressing the green Play button (Start Debugging).
* **Executable Location**: The compiled binary, `AntSimulation.exe`, is located in `build/bin/Debug/` or `build/bin/Release/`.

### Linux (GDB / VS Code / CLion)
* **Executable Location**: The executable, `main`, is located in `build/bin/`.
* **Running from Terminal**: To run correctly, you must launch the executable from within the `build` directory:
    ```bash
    cd build/
    ./bin/main
    ```
* **IDE Configuration (VS Code Example)**: When setting up a `launch.json` file for debugging in an IDE like VS Code, you must explicitly set the Current Working Directory (`cwd`).
    ```json
    {
        "version": "0.2.0",
        "configurations": [
            {
                "name": "(gdb) Launch",
                "type": "cppdbg",
                "request": "launch",
                "program": "${workspaceFolder}/build/bin/main",
                "args": [],
                "stopAtEntry": false,
                "cwd": "${workspaceFolder}/build",
                "environment": [],
                "externalConsole": false,
                "MIMode": "gdb",
                "setupCommands": [
                    {
                        "description": "Enable pretty-printing for gdb",
                        "text": "-enable-pretty-printing",
                        "ignoreFailures": true
                    }
                ]
            }
        ]
    }
    ```

---

## 🤝 Developer Contribution Workflow

Contributions are welcome. The workflow is designed to be straightforward while maintaining code integrity.
> [!IMPORTANT]  
> Read the CONTRIBUTING.md for full details.



1.  **Fork & Clone**: Fork the repository on GitHub and clone your fork locally.
2.  **Make Changes**: Implement your features or bug fixes. Ideas for contributions include improving pheromone mechanics, implementing inter-colony interactions, or adding UI enhancements.
3.  **Build & Test**: Ensure the project builds successfully on your platform. This will generate/update the `build_verification.txt` file.
4.  **Commit & Push**: Commit your changes, including any modifications to the verification file.
5.  **Submit Pull Request**: Open a pull request against the main repository for review. The CI pipeline, which runs on GitHub Actions, will automatically test the build on Windows and Linux platforms.
