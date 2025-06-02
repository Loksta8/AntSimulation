# Ant Simulation

![Ant Simulation](https://github.com/Loksta8/AntSimulation/blob/master/AntSim.gif?raw=true)

**PURPOSE**:
To simulate an ant colony in search of food, collect the food, take the food back to their colony,
and grow their colony population.


**BACKGROUND**:
To use SFML graphically, to simulate ants visually.

## CMAKE Steps to build project

1. Download `CMAKE`.

2. A file named `CMakeLists.txt` is inside the root directory of this project.

3. Open `CMAKE` and in the `Where is the source code:` navigate to the root `CMakeLists.txt`.

4. In the `Where to build the binaries:` create a `build` folder preferably outside of the repo and point to that folder.

5. Assuming you already have `CMAKE` configured correctly with `Visual Studio 17 2022` click `Generate` and once completed then open the project.

6. Inside the IDE right click on `main` and click `Set as Startup Project`.

7. Build the project (If it errors you might have to click on `Show all Files` and delete `Debug`).

8. Click `Play` (Debug) and watch the ants find their way randomly to food.


**CMake version used**: 
cmake_minimum_required(VERSION 3.29.2)

**Compiler**: 
C++17