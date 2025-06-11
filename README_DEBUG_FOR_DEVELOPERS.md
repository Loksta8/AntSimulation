# 🐜 Ant Simulation - Debug/Development Version

[![CMake on multiple platforms](https://github.com/Loksta8/AntSimulation/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/Loksta8/AntSimulation/actions/workflows/cmake-multi-platform.yml)
![Cross-Platform](https://img.shields.io/badge/Cross%20Platform-Windows%20%7C%20Linux-success)
![SFML](https://img.shields.io/badge/Dependency-SFML%202.6-green)
![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg) 

**🎯 PURPOSE**:
To simulate an ant colony in search of food using SFML for graphics.

**💡 BACKGROUND**:
- This project utilizes SFML for graphical rendering to create a realistic simulation of ant colony behavior. The goal is to model key characteristics such as foraging, pheromone trails, colony growth, and environmental interactions. I (Logan Herrera) am the original author of this project.

The current simulation includes the following features:
* **Variable Food Amounts**: Food sources are depleted gradually as ants find the food and take it back to their colony.
* **Ant Lifespan & Death**: Ants have a finite lifespan and are removed upon death. Gradually the ants die off.
* **Multiple Colonies**: The simulation supports multiple independent ant colonies, each with distinct colors. They currently do not interact yet.
* **Separate Pheromone Grids**: Each colony lays and follows its own distinct pheromone trails.
* **Controlled Colony Growth**: Colonies require specific food amounts to spawn new ants.
* **Enhanced Metrics Display**: You can monitor Total Live Ants, Peak Population, Total Deaths, and remaining Food Sources.
* **Optimized Grid Scaling**: The simulation can handle larger grid sizes (controlled by `Environment::GRID_SIZE`). The grid data structures were moved from stack to heap (switched from C-style arrays to `std::vector`).
* **Refined Ant Foraging Behavior**: Ants now return home more directly and efficiently after finding food.
* **Automatic & Manual Simulation Reset**: The simulation will restart after a set delay if all food is gone or all ants die, or if the user presses the 'R' key.
* **Mouse & Keyboard Functionality**: Zooming in and out, and panning work via mouse scroll wheel, mouse click-hold-and-drag, and arrow keys (Up/Down for zoom, Left/Right/A/D/W/S for pan).

![Ant Simulation GIF](https://raw.githubusercontent.com/Loksta8/AntSimulation/main/AntSim.gif)

## 🚀 Getting Started

1.  Clone the repository: `git clone https://github.com/Loksta8/AntSimulation`
2.  Follow the platform-specific build instructions below.
3.  Run the simulation from your `build/` directory using `./bin/main` (Linux) or `./AntSimulation.exe` (Windows Debug/Release).

## 🎮 Simulation Controls (Detailed)

This section explains all controls for navigating and interacting with the Ant Simulation.

* **Zoom In/Out**:
    * **Mouse Scroll Wheel**: Scroll up to zoom in, scroll down to zoom out.
    * **Up Arrow Key**: Zoom in.
    * **Down Arrow Key**: Zoom out.
* **Pan View (Move Camera)**:
    * **Mouse Click-Hold-and-Drag**: Click and hold the left mouse button, then drag the mouse to move the view.
    * **Left Arrow Key**: Pan left.
    * **Right Arrow Key**: Pan right.
    * **'A' Key**: Pan left.
    * **'D' Key**: Pan right.
    * **'W' Key**: Pan up.
    * **'S' Key**: Pan down.
* **Reset Simulation**:
    * **'R' Key**: Manually reset the simulation to its initial state. The simulation also auto-resets after a delay if all food is gone or all ants die.

## 🛠️ Building the Project

### ✅ Prerequisites (All Platforms)

* **CMake**: Version 3.28 or newer. Download from [cmake.org](https://cmake.org/download/).
* **C++17 Compiler**:
    * Windows: Visual Studio 2022 (MSVC).
    * Linux: GCC (g++) or Clang.
* **Git**: For cloning the repository.
* **Python 3**: Required for setting up Git hooks (see **Contributing** section).

### 🪟 Windows (Visual Studio)

These steps assume you are using Visual Studio 2022.

1.  **Clone the Repository ✨**:
    ```bash
    git clone [https://github.com/Loksta8/AntSimulation](https://github.com/Loksta8/AntSimulation)
    cd AntSimulation
    ```

2.  **Set up Vcpkg for Dependencies**:
    * **Clone Vcpkg**: Open an **Administrator Command Prompt** or **PowerShell** and navigate to your desired installation location (e.g., `C:\`).
        ```bash
        cd C:\
        git clone [https://github.com/microsoft/vcpkg.git](https://github.com/microsoft/vcpkg.git)
        cd vcpkg
        ```
    * **Bootstrap Vcpkg**:
        ```bash
        .\bootstrap-vcpkg.bat
        ```
    * **Integrate Vcpkg with Visual Studio**:
        ```bash
        .\vcpkg integrate install
        ```
    * **Install Dependencies**: Vcpkg manages OpenSSL. Your project's `vcpkg.json` automatically lists `openssl` for installation when CMake runs.
        ```bash
        # This command is technically optional as CMake will trigger it, but useful to know.
        .\vcpkg.exe install openssl:x64-windows
        ```
        *Note: If you encounter permissions errors, ensure your Command Prompt/PowerShell is running as Administrator. If you face persistent issues, try reinstalling vcpkg to a simpler path like `C:\vcpkg` instead of `C:\Program Files\vcpkg`.*

3.  **Configure CMake with `CMakePresets.json` ⚙️**:
    This project uses `CMakePresets.json` for easy configuration across environments.
    * **Open Visual Studio**: Launch Visual Studio 2022.
    * **Open the Project**: Go to `File > Open > CMake...` and select the root directory of your cloned repository (e.g., `C:\path\to\AntSimulation`).
    * **Select a Preset**: Visual Studio will automatically detect `CMakePresets.json`. In the toolbar, select the `Windows Default Build` preset (or "default-windows") from the "Presets" dropdown.
    * **Configure**: Visual Studio will automatically configure CMake using the preset. You can monitor the "Output" window.
    * *(Optional: If you prefer command line, open a "Developer Command Prompt for VS 2022", navigate to your project root, and run: `cmake --preset default-windows`)*

4.  **Build in Visual Studio 🏗️**:
    * Once CMake configuration is complete, your project should appear in the Solution Explorer.
    * Right-click on the `main` target in the Solution Explorer and select "Set as Startup Project".
    * Build the project (e.g., `Build > Build Solution` or `Ctrl+Shift+B`).

5.  **Run the Simulation ▶️**:
    * Run the `main` project directly from Visual Studio (e.g., `Debug > Start Debugging`, or click the green Play button ▶️).
    * The executable (`AntSimulation.exe`) will be in `build/bin/Debug/` (or `Release/`).
    * **For easier local running**, the `Vertiky.ttf` font and `verification.txt` are copied directly to your `build/` directory. So, when running from VS (which sets the working directory to `build/`), the program will find these resources correctly.

### 🐧 Linux

1.  **Install Prerequisites ✅**:
    * Open your terminal.
    * **CMake, C++ Compiler (g++ or Clang), Git**:
        ```bash
        # For Debian/Ubuntu based systems
        sudo apt-get update
        sudo apt-get install build-essential cmake git
        ```
        *(For Fedora/Arch, you'd use their respective package managers as in your original README.)*
    * **Dependencies (OpenSSL & SFML System Libraries)**:
        * SFML is built from source by CMake's `FetchContent`.
        * OpenSSL and other SFML system dependencies need to be installed.
        * **Shell Script (Recommended):** I included a shell script `setup_linux_dependencies.sh` that does the below for you.
            ```bash
            chmod +x setup_linux_dependencies.sh && ./setup_linux_dependencies.sh
            ```
        * **Manual Installation (Ubuntu/Debian):**
            ```bash
            sudo apt-get install libgl1-mesa-dev libxrandr-dev libxcursor-dev libudev-dev libopenal-dev libflac-dev libvorbis-dev libfreetype6-dev libssl-dev
            ```
            *(Removed `libsfml-dev` as SFML is built via FetchContent. Added `libssl-dev` for OpenSSL.)*

2.  **Clone the Repository (if not already done) ✨**:
    ```bash
    git clone [https://github.com/Loksta8/AntSimulation](https://github.com/Loksta8/AntSimulation)
    cd AntSimulation
    ```

3.  **Configure and Build with CMake ⚙️**:
    ```bash
    mkdir build
    cd build
    cmake .. # Configures CMake using the default options. FetchContent will download and build SFML.
    make -j$(nproc) # -j$(nproc) uses all available processor cores for faster compilation
    ```

4.  **Run the Simulation ▶️**:
    The executable (`main`) will be in `build/bin/`.
    The `Vertiky.ttf` font and `verification.txt` are copied directly to your `build/` directory for convenience.
    To run the simulation from the `build` directory:
    ```bash
    cd build
    ./bin/main
    ```

## ℹ️ Project Details

## 🌍 Cross-Platform Compatibility  
This project runs **seamlessly** on both **Windows** and **Linux**, ensuring accessibility across different operating systems!  
✅ **Windows** – Tested on Windows 11  
✅ **Linux** – Tested on Debian 12 and Ubuntu 22.04  

**Continuous Integration (CI)**:
[![CMake on multiple platforms](https://github.com/Loksta8/AntSimulation/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/Loksta8/AntSimulation/actions/workflows/cmake-multi-platform.yml)
The project utilizes GitHub Actions for automated cross-platform builds and testing on every push and pull request. Check the CI badge above for the latest build status.

**CMake version used**:
`VERSION 3.28`

**Compiler Standard**:  
`C++17`  

**Dependencies**:  
`SFML 2.6 (via FetchContent)`, `OpenSSL (via Vcpkg)`, `Vertiky.ttf`  

**Font Requirement**:  
`Vertiky.ttf` is bundled with the project and automatically placed in the top-level build directory (`build/`) during the build process, ensuring it's in the debugger's working directory.

**Build System Features**:
- Cross-platform CMake configuration with platform-specific optimizations
- Automatic dependency fetching for SFML via CMake FetchContent
- **OpenSSL dependency management via Vcpkg**
- Smart resource copying that places assets in the top-level build directory for consistent debugger/runtime behavior.
- RPATH configuration on Linux for better shared library handling
- **Automated build verification hash generation** (for `build_verification.txt`)

## 📜 License  
[![GPLv3 License](https://www.gnu.org/graphics/gplv3-88x31.png)](https://www.gnu.org/licenses/gpl-3.0.html)
This project is licensed under the **GNU GPL v3**. See the [LICENSE](LICENSE) file for details.

## 🤝 Contributing

This project is open for learning and exploration! Here are a few ways to get involved:  
We welcome contributions to the Ant Simulation project! To ensure code integrity across platforms, we've implemented an automated verification system.

**Getting Started for Contributors**

1.  **Clone the repository**:
    ```bash
    git clone [https://github.com/Loksta8/AntSimulation](https://github.com/Loksta8/AntSimulation)
    cd AntSimulation
    ```
2.  **Set up Git Hooks**:
    ```bash
    python setup_hooks.py
    ```
    This configures the necessary git hooks for the verification system.

**How the Verification System Works**

Our verification system helps maintain code integrity through the following processes:

* **Local Source Code Verification (`verification.txt`)**:
    * A **Pre-commit Hook** (set up by `setup_hooks.py`) automatically detects changes to source files.
    * It updates a hash value in `verification.txt` when changes are detected.
    * This updated `verification.txt` is automatically included in your commit. This file tracks changes to the source code itself.
* **Built Executable Verification (`build_verification.txt`)**:
    * When building with CMake, a separate mechanism automatically generates a SHA256 hash of your main executable.
    * This hash is stored in `build_verification.txt` in your build directory.
    * This file provides a verifiable checksum of the built program, ensuring its integrity and consistency with the build process.
    * For Pull Requests, this `build_verification.txt` is uploaded as an artifact in the CI pipeline for easy verification by maintainers.

This seamless process ensures all contributors maintain consistent code verification without manual intervention. The verification files remain in your build directory and don't need to be manually managed after committing.

**Contribution Workflow**

1.  **Make your code changes**.
2.  **Commit your changes** (the hook handles `verification.txt` automatically).
3.  **Push your changes** (including the updated `verification.txt`).
4.  **Submit a pull request**. The CI pipeline will automatically build your changes and generate a `build_verification.txt` artifact for integrity checking.

The verification system will help maintain project integrity across different development environments and platforms.

### 🐜 How to Contribute  
1.  **Fork the repository** and clone your version.  
2.  **Explore the code** and get familiar with how the ant simulation works.  
3.  **Suggest improvements**: Whether it's optimizing algorithms, refining ant behaviors, or adding new features, feel free to propose ideas! I would like to make soldier ants eventually!
4.  **Submit a pull request** if you make meaningful changes.  

### 🚀 Ideas for Contributors  
-   Improve pheromone decay mechanics for more realistic foraging.  
-   Implement inter-colony interactions or competition.
-   Add UI enhancements for easier metric tracking.  
-   Optimize performance for larger-scale simulations.  
-   Expand documentation with deeper explanations or diagrams.  

If you just want to **ask questions or brainstorm ideas**, opening a discussion or issue is always welcome! Thanks and enjoy the ants. I've been keeping it running as a screen saver haha! 🖥️🐜💾