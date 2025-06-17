
# 🐜 Ant Simulation

  

![Cross-Platform](https://img.shields.io/badge/Cross%20Platform-Windows%20%7C%20Linux-success)

![SFML](https://img.shields.io/badge/Dependency-SFML%202.6-green)

![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)

  

**🎯 PURPOSE**:

To simulate an ant colony in search of food using SFML for graphics.

  

**💡 BACKGROUND**:

- This project utilizes SFML for graphical rendering to create a realistic simulation of ant colony behavior. The goal is to model key characteristics such as foraging, pheromone trails, colony growth, and environmental interactions. I (Logan Herrera) am the original author of this project. There are 3 colonies each with their own unique color (Black, Red, and Blue). As they search for food they leave a trail of pheromones. If they find food, the ant color changes green to show their returning home with food, they also leave a yellowish pheromone trail to alert and lead other ants to the food source that was found. Read more below to find out the simulation controls.

  

The current simulation includes the following features:

*  **Variable Food Amounts**: Food sources are depleted gradually as ants find the food and take it back to their colony.

*  **Ant Lifespan & Death**: Ants have a finite lifespan and are removed upon death. Gradually the ants die off.

*  **Multiple Colonies**: The simulation supports multiple independent ant colonies, each with distinct colors. They currently do not interact yet.

*  **Separate Pheromone Grids**: Each colony lays and follows its own distinct pheromone trails.

*  **Controlled Colony Growth**: Colonies require specific food amounts to spawn new ants.

*  **Enhanced Metrics Display**: You can monitor Total Live Ants, Peak Population, Total Deaths, and remaining Food Sources.

*  **Optimized Grid Scaling**: The simulation can handle larger grid sizes (controlled by `Environment::GRID_SIZE`). The grid data structures were moved from stack to heap (switched from C-style arrays to `std::vector`).

*  **Refined Ant Foraging Behavior**: Ants now return home more directly and efficiently after finding food.

*  **Automatic & Manual Simulation Reset**: The simulation will restart after a set delay if all food is gone or all ants die, or if the user presses the 'R' key.

*  **Mouse & Keyboard Functionality**: Zooming in and out, and panning work via mouse scroll wheel, mouse click-hold-and-drag, and arrow keys (Up/Down for zoom, Left/Right/A/D/W/S for pan).

  

![Ant Simulation GIF](https://raw.githubusercontent.com/Loksta8/AntSimulation/main/AntSim.gif)

  

## 🚀 Getting Started

1. Clone the repository: `git clone https://github.com/Loksta8/AntSimulation`

2. Follow the platform-specific build instructions below.

3. Run the simulation from your `build/` directory using `./bin/main` (Linux) or `./AntSimulation.exe` (Windows Debug/Release).

## :video_game: **Play with the Program Download from Releases**
>[!Note]
> If you do not want to build the project and you just feel like running the program to play around with it, you can download from [Releases](https://github.com/Loksta8/AntSimulation/releases)
> If you are on Windows you'll need Microsoft's Visual C++ Redistributables runtime libraries, you can get them from [here](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170) .


## 🛠️ Building the Project

  

### ✅ Prerequisites (All Platforms)

  

*  **CMake**: Version 3.28 or newer. Download from [cmake.org](https://cmake.org/download/).

*  **C++17 Compiler**:

* Windows: Visual Studio 2022 (MSVC).

* Linux: GCC (g++) or Clang.

*  **Git**: For cloning the repository.


  

### 🪟 Windows (Visual Studio)

  

These steps assume you are using Visual Studio 2022.

  

1.  **Clone the Repository ✨**:

```bash

git clone https://github.com/Loksta8/AntSimulation

cd AntSimulation

```

  

2.  **Set up Vcpkg for Dependencies**:

*  **Clone Vcpkg**: Open an **Administrator Command Prompt** or **PowerShell** and navigate to your desired installation location (e.g., `C:\`).

```bash

cd C:\

git clone https://github.com/microsoft/vcpkg.git

cd vcpkg

```

*  **Bootstrap Vcpkg**:

```bash

.\bootstrap-vcpkg.bat

```

*  **Integrate Vcpkg with Visual Studio**:

```bash

.\vcpkg integrate install

```

*  **Install Dependencies**: Vcpkg manages all libraries for future ease of use. The project's `vcpkg.json` automatically lists `any libraries the project needs in the future` for installation when CMake runs. Currently there aren't any libraries in there but in case other developers feel like using other libraries with the project, the `vcpkg.json` is where you can list them. 


> [!TIP]
> IF you do happen to bring in a new library as mentioned above,
> this command is technically optional as CMake will trigger it.
> Either way it's useful to know, using openssl library as an example. It is not required by the project.
> .\vcpkg.exe install openssl:x64-windows
> 




>[!Note]: 
> If you encounter permissions errors, ensure your Command Prompt/PowerShell is running as 
> Administrator. If you face persistent issues, try reinstalling vcpkg to a simpler path 
> like `C:\vcpkg` instead of `C:\Program Files\vcpkg`. 

  

3.  **Configure CMake with `CMakePresets.json` ⚙️**:

This project uses `CMakePresets.json` for easy configuration across environments.

*  **Open Visual Studio**: Launch Visual Studio 2022.

*  **Open the Project**: Go to `File > Open > CMake...` and select the root directory of your cloned repository (e.g., `C:\path\to\AntSimulation`).

*  **Select a Preset**: Visual Studio will automatically detect `CMakePresets.json`. In the toolbar, select the `Windows Default Build` preset (or "default-windows") from the "Presets" dropdown.

*  **Configure**: Visual Studio will automatically configure CMake using the preset. You can monitor the "Output" window.

*  *(Optional: If you prefer command line, open a "Developer Command Prompt for VS 2022", navigate to your project root, and run: `cmake --preset default-windows`)*

  

4.  **Build in Visual Studio 🏗️**:

* Once CMake configuration is complete, your project should appear in the Solution Explorer.

* Right-click on the `main` target in the Solution Explorer and select "Set as Startup Project".

* Build the project (e.g., `Build > Build Solution` or `Ctrl+Shift+B`).

  

5.  **Run the Simulation ▶️**:

* Run the `main` project directly from Visual Studio (e.g., `Debug > Start Debugging`, or click the green Play button ▶️).

* The executable (`AntSimulation.exe`) will be in `build/bin/Debug/` (or `Release/`).

*  **For easier local running**, the `Vertiky.ttf` font is copied directly to your `build/` directory. So, when running from VS (which sets the working directory to `build/`), the program will find these resources correctly.

  

### 🐧 Linux

  

1.  **Install Prerequisites ✅**:

* Open your terminal.

*  **CMake, C++ Compiler (g++ or Clang), Git**:

```bash

# For Debian/Ubuntu based systems

sudo apt-get update

sudo apt-get install build-essential cmake git

```

  

*  **Dependencies (SFML System Libraries)**:

* SFML is built from source by CMake's `FetchContent`.


*  **Shell Script (Recommended):** I included a shell script `setup_linux_dependencies.sh` that does the below for you.

```bash

chmod +x setup_linux_dependencies.sh && ./setup_linux_dependencies.sh

```


* **Manual Installation (Ubuntu/Debian):**

    ```bash
    sudo apt-get install libgl1-mesa-dev libxrandr-dev libxcursor-dev libudev-dev libopenal-dev libflac-dev libvorbis-dev libfreetype6-dev
    ```

    *(Removed `libsfml-dev` as SFML is built via FetchContent.)*

* **Fedora:**

    ```bash
    sudo dnf install mesa-libGL-devel libXrandr-devel libXcursor-devel systemd-devel openal-soft-devel libflac-devel libvorbis-devel freetype-devel
    ```

* **Arch Linux:**

    ```bash
    sudo pacman -Syu glu libxrandr libxcursor systemd openal flac libvorbis freetype2
    ```

  

2.  **Clone the Repository (if not already done) ✨**:

```bash

git clone https://github.com/Loksta8/AntSimulation

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

The `Vertiky.ttf` font is copied directly to your `build/` directory for convenience.

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

`SFML 2.6 (via FetchContent)`,  `Vertiky.ttf`
In the future other libraries can be brought in via `vcpkg.json`, and will be handled `(via Vcpkg)`,
  

**Font Requirement**:

`Vertiky.ttf` is bundled with the project and automatically placed in the top-level build directory (`build/`) during the build process, ensuring it's in the debugger's working directory.

  

**Build System Features**:

- Cross-platform CMake configuration with platform-specific optimizations

- Automatic dependency fetching for SFML via CMake FetchContent

-  **Dependency management ready via Vcpkg**

- Smart resource copying that places assets in the top-level build directory for consistent debugger/runtime behavior.

- RPATH configuration on Linux for better shared library handling

-  **Automated build verification hash generation** (for `build_verification.txt`)

  

## 📜 License

[![GPLv3 License](https://www.gnu.org/graphics/gplv3-88x31.png)](https://www.gnu.org/licenses/gpl-3.0.html)

This project is licensed under the **GNU GPL v3**. See the [LICENSE](LICENSE) file for details.

  

## 🤝 Contributing

  

This project is open for learning and exploration! Here are a few ways to get involved:

We welcome contributions to the Ant Simulation project! To ensure code integrity a build_verification.txt gets created upon a successful build.

> [!IMPORTANT]  
> Read the [Contributing Guidelines](.github/CONTRIBUTING.md) for full details.



**Getting Started for Contributors**


1.  **Clone the repository**:

```bash

git clone https://github.com/Loksta8/AntSimulation

cd AntSimulation

```


  

**Contribution Workflow**


1.  **Make your code changes**.

2.  **Commit your changes**

3.  **Push your changes**

4.  **Submit a pull request**. 

  


  

### 🐜 How to Contribute

1.  **Fork the repository** and clone your version.

2.  **Explore the code** and get familiar with how the ant simulation works.

3.  **Suggest improvements**: Whether it's optimizing algorithms, refining ant behaviors, or adding new features, feel free to propose ideas! I would like to make soldier ants eventually!

4.  **Submit a pull request** if you make meaningful changes.

  

### 🚀 Ideas for Contributors

- Give pointers on how to make a good workflow to make it as easy as possible to contribute.

- Improve pheromone decay mechanics for more realistic foraging.

- Implement inter-colony interactions or competition.

- Add UI enhancements for easier metric tracking.

- Optimize performance for larger-scale simulations.

- Expand documentation with deeper explanations or diagrams.

  

If you just want to **ask questions or brainstorm ideas**, opening a discussion or issue is always welcome! Thanks and enjoy the ants. I've been keeping it running as a screen saver haha! 🖥️🐜💾
