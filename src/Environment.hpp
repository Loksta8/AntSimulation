// AntSimulation - An SFML simulation of an ant colony.
// Copyright (C) 2025 Logan Herrera <jherre36@live.nmhu.edu>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <SFML/Graphics.hpp>
#include <vector>

class Environment {
public:
    float cellSize;
	static const int GRID_SIZE = 200; // Size of the grid (200x200 cells)
    std::vector<std::vector<unsigned int>> foodGrid;         // For Food

    // --- Total count of distinct food sources currently on the grid ---
    unsigned int totalFoodSources;

    // Constants for food generation
    static constexpr unsigned int INITIAL_FOOD_PER_SOURCE = 50; // amount of food at each generated source
    static constexpr int INITIAL_FOOD_SOURCES = 100;
    static constexpr int NUM_CLUMPS = 8;
    static constexpr int ATTEMPTS_PER_CLUMP = (INITIAL_FOOD_SOURCES > 0 && NUM_CLUMPS > 0) ? (INITIAL_FOOD_SOURCES / NUM_CLUMPS) : 20;
    static constexpr float CLUMP_RADIUS = 10.0f;

    // Constructor and destructor
    Environment(float cellSize);
    ~Environment();

    // Food methods
    void generateFood();
    void renderFood(sf::RenderWindow& window);
    bool checkForFood(int x, int y);
    void removeFood(int x, int y);
	// For debugging purposes
    void debugFoodPositions();
};

#endif // ENVIRONMENT_HPP