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

#include "Environment.hpp"
#include "RandomUtils.hpp"
#include <iostream>
#include <random> // For std::uniform_int_distribution, std::uniform_real_distribution
#include <cmath> // For std::sqrt, std::cos, std::sin

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Constructor
Environment::Environment(float cellSizeVal) : cellSize(cellSizeVal), 
totalFoodSources(0),
foodGrid(GRID_SIZE, std::vector<unsigned int>(GRID_SIZE, 0)) {
    generateFood(); // Place initial food sources
}

// Destructor
Environment::~Environment()
{
    
}

// Generate Random Food Sources
void Environment::generateFood() {
    // Clear existing food first
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            foodGrid[i][j] = 0;
        }
    }
    totalFoodSources = 0; // Reset count when regenerating food

    std::uniform_int_distribution<> distrib_coord(0, GRID_SIZE - 1);
    std::uniform_real_distribution<float> distrib_angle(0.0f, 2.0f * static_cast<float>(M_PI));
    std::uniform_real_distribution<float> distrib_radius_factor(0.0f, 1.0f);


    for (int c = 0; c < NUM_CLUMPS; ++c) {
        if (totalFoodSources >= INITIAL_FOOD_SOURCES && INITIAL_FOOD_SOURCES > 0) break;

        int clumpCenterX = distrib_coord(RandomUtils::getGenerator());
        int clumpCenterY = distrib_coord(RandomUtils::getGenerator());

        for (int attempt = 0; attempt < ATTEMPTS_PER_CLUMP; ++attempt) {
            if (totalFoodSources >= INITIAL_FOOD_SOURCES && INITIAL_FOOD_SOURCES > 0) break;

            float angle = distrib_angle(RandomUtils::getGenerator());
            float radius_factor = distrib_radius_factor(RandomUtils::getGenerator());
            float radius = CLUMP_RADIUS * radius_factor * radius_factor;

            int offsetX = static_cast<int>(std::round(radius * std::cos(angle)));
            int offsetY = static_cast<int>(std::round(radius * std::sin(angle)));

            int foodX = clumpCenterX + offsetX;
            int foodY = clumpCenterY + offsetY;

            if (foodX >= 0 && foodX < GRID_SIZE && foodY >= 0 && foodY < GRID_SIZE) {
                if (foodGrid[foodX][foodY] == 0) { // Only place if the cell is empty
                    foodGrid[foodX][foodY] = INITIAL_FOOD_PER_SOURCE; // Set initial food quantity
                    totalFoodSources++;
                }
            }
        }
    }
    //std::cout << "Total distinct food sources placed: " << totalFoodSources << std::endl;
}

// Check if Food Exists at a Given Grid Location (for quantity > 0)
bool Environment::checkForFood(int x, int y) {
    if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) {
        return foodGrid[x][y] > 0; // Food exists if quantity is greater than 0
    }
    return false;
}

// Remove Food When an Ant Takes It from a Given Grid Location (decrements quantity)
void Environment::removeFood(int x, int y) {
    if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) {
        if (foodGrid[x][y] > 0) { // Check if food exists before removing
			foodGrid[x][y]--;   // Decrement food quantity by 1 at the specified location
            if (foodGrid[x][y] == 0) { // If source is fully depleted
                totalFoodSources--; // Decrement count of existing sources
            }
            // std::cout << "Food decremented at (" << x << ", " << y << "). Remaining: " << foodGrid[x][y] << "\n";
        }
    }
}


// Render Food Sources Correctly Using SFML (only renders if food > 0)
void Environment::renderFood(sf::RenderWindow& window) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (foodGrid[i][j] > 0) { // If food is present at grid cell i,j
                sf::RectangleShape foodShape(sf::Vector2f(cellSize, cellSize));
                foodShape.setPosition(static_cast<float>(i * cellSize), static_cast<float>(j * cellSize));
                foodShape.setFillColor(sf::Color::Green);
                window.draw(foodShape);
            }
        }
    }
}


// Utility debug function to print current food positions to the console
void Environment::debugFoodPositions() {
    std::cout << "Current food positions (with quantity):\n";
    bool foodFound = false;
    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            if (foodGrid[x][y] > 0) {
                std::cout << "  Food at (" << x << ", " << y << ") Qty: " << foodGrid[x][y] << "\n";
                foodFound = true;
            }
        }
    }
    if (!foodFound) {
        std::cout << "  No food on the grid.\n";
    }
}