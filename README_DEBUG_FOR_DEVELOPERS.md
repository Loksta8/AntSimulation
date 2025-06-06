# 🐜 Ant Simulation - Debug/Development Version

![Cross-Platform](https://img.shields.io/badge/Cross%20Platform-Windows%20%7C%20Linux-success)
![SFML](https://img.shields.io/badge/Dependency-SFML%202.6-green)
![OpenSSL](https://img.shields.io/badge/Dependency-OpenSSL-green)
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
3.  Run the simulation via `./bin/main` (Linux) or `AntSimulation.exe` (Windows).

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
    * Windows: Visual Studio 2017 or newer (MSVC), or MinGW-w64.
    * Linux: GCC (g++) or Clang.
    * macOS: Clang (Xcode Command Line Tools).
* **Git**: For cloning the repository.

### 🪟 Windows (Visual Studio)

These steps assume you are using Visual Studio.

1.  **Clone the Repository ✨**:
    ```bash
    git clone [https://github.com/Loksta8/AntSimulation](https://github.com/Loksta8/AntSimulation)
    cd AntSimulation
    ```

2.  **Configure and Generate Project Files**:

    You can use either the CMake GUI or the command line.

    **Option A: Using CMake GUI ⚙️**
    * Open CMake GUI.
    * In "Where is the source code:", navigate to the root of the cloned repository (e.g., `C:/path/to/AntSimulation`).
    * In "Where to build the binaries:", create a `build` folder (e.g., `C:/path/to/AntSimulation/build`) and point to that folder.
    * Click "Configure".
    * Choose your generator from the dropdown list (e.g., "Visual Studio 17 2022"). Ensure the "Platform for generator" is set correctly (e.g., x64 if you're building a 64-bit application).
    * Click "Finish".
    * Click "Generate".

    **Option B: Using Command Line (Developer Command Prompt for VS) 💻**
    * Open a "Developer Command Prompt for VS 2022" (or your corresponding Visual Studio version). This ensures that CMake can find your C++ compiler and other build tools. You can find this in your Start Menu.
    * Navigate to the root directory of your cloned repository:
        ```batch
        cd C:\path\to\AntSimulation
        ```
    * Create a build directory and navigate into it:
        ```batch
        mkdir build
        cd build
        ```
    * Run CMake to configure and generate the Visual Studio solution. Replace `"Visual Studio 17 2022"` if you are using a different version. For a 64-bit build, you can specify the architecture with `-A x64`.
        ```batch
        cmake -G "Visual Studio 17 2022" -A x64 ..
        ```
        * `-G "Visual Studio 17 2022"`: Specifies the generator.
        * `-A x64`: Specifies the target architecture as 64-bit. For 32-bit, you might use `-A Win32` (though 64-bit is more common now). If you omit `-A`, CMake might pick a default based on your VS installation.
        * `..`: Points to the parent directory (which is the source directory containing your root `CMakeLists.txt`).

3.  **Open and Build in Visual Studio 🏗️**:
    * After either GUI or command-line generation, navigate to your `build` directory. You will find a Visual Studio Solution file (`.sln`), for example, `AntSimulation.sln`.
    * Open this `.sln` file in Visual Studio.
    * Inside Visual Studio, right-click on the `main` target in the Solution Explorer and select "Set as Startup Project".
    * Build the project (e.g., Build > Build Solution).
    * Run the `main` project (e.g., Debug > Start Debugging, or click the green Play button ▶️).

### 🐧 Linux

1.  **Install Prerequisites ✅**:
    * Open your terminal.
    * **CMake, C++ Compiler (g++), Git**:
        ```bash
        # For Debian/Ubuntu based systems
        sudo apt-get update
        sudo apt-get install build-essential cmake git
        
        # For Fedora based systems
        sudo dnf groupinstall "Development Tools" "Development Libraries" 
        sudo dnf install cmake git
        
        # For Arch Linux based systems
        sudo pacman -Syu base-devel cmake git
        ```
    * **SFML and OpenSSL Dependencies**: The `CMakeLists.txt` uses `FetchContent` to download and build SFML from source. However, SFML itself relies on several system libraries, and the project also requires OpenSSL. You need to install their development packages. I included a shell script `setup_linux_dependencies.sh` that does the below for you or you can run the commands manually as below:
    
        **Shell Script (Recommended):**
        ```bash
        chmod +x setup_linux_dependencies.sh && ./setup_linux_dependencies.sh
        ```

        **Manual Installation:**

        **Ubuntu/Debian:**
        ```bash
        sudo apt-get install libgl1-mesa-dev libxrandr-dev libxcursor-dev libudev-dev libopenal-dev libflac-dev libvorbis-dev libfreetype6-dev libssl-dev libsfml-dev
        ```

        **Fedora:**
        ```bash
        sudo dnf install mesa-libGL-devel libXrandr-devel libXcursor-devel systemd-devel openal-soft-devel libflac-devel libvorbis-devel freetype-devel openssl-devel SFML-devel
        ```

        **Arch Linux:**
        ```bash
        sudo pacman -Syu glu libxrandr libxcursor systemd openal flac libvorbis freetype2 openssl sfml
        ```

2.  **Clone the Repository (if not already done) ✨**:
    ```bash
    git clone [https://github.com/Loksta8/AntSimulation](https://github.com/Loksta8/AntSimulation)
    cd AntSimulation
    ```

3.  **Configure and Build with CMake ⚙️**:
    ```bash
    mkdir build
    cd build
    cmake .. 
    make -j$(nproc) # -j$(nproc) uses all available processor cores for faster compilation
    ```

4.  **Run the Simulation ▶️**:
    The executable will be in `build/bin/` with the font copied over.
    ```bash
    ./bin/main
    ```

## ℹ️ Project Details

## 🌍 Cross-Platform Compatibility  
This project runs **seamlessly** on both **Windows** and **Linux**, ensuring accessibility across different operating systems!  
✅ **Windows** – Tested on Windows 11  
✅ **Linux** – Tested on Debian 12 and Ubuntu 22.04  

**CMake version used**:
`VERSION 3.28`

**Compiler Standard**:  
`C++17`  

**Dependencies**:  
`SFML 2.6`, `OpenSSL`, `Vertiky.ttf`  

**Font Requirement**:  
Vertiky.ttf is bundled with the project and automatically placed next to the executable during the build process. No manual setup is needed.

**Build System Features**:
- Cross-platform CMake configuration with platform-specific optimizations
- Automatic dependency fetching for SFML via CMake FetchContent
- Smart resource copying that works with both single-config (Make/Ninja) and multi-config (Visual Studio) generators
- RPATH configuration on Linux for better shared library handling

## 📜 License  
[![GPLv3 License](https://www.gnu.org/graphics/gplv3-88x31.png)](https://www.gnu.org/licenses/gpl-3.0.html)
This project is licensed under the **GNU GPL v3**. See the [LICENSE](LICENSE) file for details.

## 🤝 Contributing

This project is open for learning and exploration! Here are a few ways to get involved:  
We welcome contributions to the Ant Simulation project! To ensure code integrity across platforms, we've implemented an automated verification system.

Getting Started

    Clone the repository
    Run the included setup script:

    python setup_hooks.py

    This configures the necessary git hooks for the verification system.

How the Verification System Works

Our verification system maintains code integrity through the following process:

    Pre-commit Hook: Automatically detects changes to source files
    Hash Generation: Updates the hash value in verification.txt when changes are detected
    Automatic Inclusion: Adds the updated verification.txt to your commit
    Build Integration: When building with CMake, the system places the current verification.txt alongside your executable and generates a validation_history_log.txt in your build directory

This seamless process ensures all contributors maintain consistent code verification without manual intervention. The verification files remain in your build directory and don't need to be manually managed.
Contribution Workflow

    Make your code changes
    Commit your changes (the hook handles verification automatically)
    Push your changes (including the updated verification hash)
    Submit a pull request

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