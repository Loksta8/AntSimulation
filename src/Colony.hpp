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

#ifndef COLONY_HPP
#define COLONY_HPP

#include "Environment.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

class Ant; // Forward declare the Ant class

class Colony {
public:
    int homeX, homeY;
    unsigned long long peakPopulation;
    std::vector<Ant> ants;
    sf::Color colonyColor;

    // Unique identifier for each colony
    int id;
    static int nextColonyID; // Static counter for unique IDs
    
    // --- Food storage and spawn cost ---
    unsigned int foodStored; // Tracks accumulated food
    static constexpr unsigned int FOOD_REQUIRED_PER_ANT_SPAWN = 8; // Cost to spawn one ant

    // --- Counter for total ants that have died from this colony ---
    unsigned long long totalAntsDied; // Using unsigned long long for large numbers

    // --- Pheromone grids owned by the colony ---
    std::vector<std::vector<float>> foodPheromones; // "Food Trail" pheromones
    std::vector<std::vector<float>> returnHomePheromones; // "Home trail" pheromones

    // --- Pheromone constants for this colony ---
	static constexpr float PHEROMONE_DECAY_RATE = 0.98f; // How quickly pheromones fade over time
    static constexpr float MAX_PHEROMONE_LEVEL = 500.0f; // A cap for pheromone levels

    // Constructor
    Colony(int colonyX, int colonyY, int initialNumAnts, float antsCellSize, const sf::Color& color, int id, const sf::Texture& antTexture);


    Colony(const Colony&) = delete;
    Colony& operator=(const Colony&) = delete;
    Colony(Colony&&) noexcept = default;
    Colony& operator=(Colony&&) noexcept = default;

    // Destructor
    ~Colony();

    // Main update method for the colony
    void update(Environment& env, const std::vector<Colony>& allColonies);

    
    // Adds a unit of food to the colony's stored supply
    void addFood(unsigned int amount = 1);

    // --- Pheromone management methods for this colony ---
    void addFoodPheromone(int gridX, int gridY, float amount);
    float getFoodPheromoneLevel(int gridX, int gridY) const;
    void addReturnHomePheromone(int gridX, int gridY, float amount);
    float getReturnHomePheromoneLevel(int gridX, int gridY) const;
    void updatePheromones(); // Method to handle decay for this colony's pheromones

private:
    float m_antsCellSize;
    int m_antsToSpawnThisTurn;
    void spawnAnts(int numAntsToSpawn);
	const sf::Texture& m_antTexture; // Texture for the ants
};

#endif // COLONY_HPP