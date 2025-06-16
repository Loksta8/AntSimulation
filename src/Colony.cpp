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

#include "Colony.hpp"
#include "Ant.hpp"
#include "Environment.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

// Definition and initialization of the static member to keep track of the next colony ID
// This static member is used to assign unique IDs to each colony and is local to this class's scope
int Colony::nextColonyID = 0;

// Constructor
Colony::Colony(int colonyX, int colonyY, int initialNumAnts, float antsCellSize, const sf::Color& color, int id, const sf::Texture& antTexture)
    : homeX(colonyX),
    homeY(colonyY),
    peakPopulation(initialNumAnts),
    m_antsCellSize(antsCellSize),
    m_antsToSpawnThisTurn(0),
    colonyColor(color),
    id(id),
    foodStored(0),
    totalAntsDied(0),
    foodPheromones(Environment::GRID_SIZE, std::vector<float>(Environment::GRID_SIZE, 0.0f)),
    returnHomePheromones(Environment::GRID_SIZE, std::vector<float>(Environment::GRID_SIZE, 0.0f)),
    m_antTexture(antTexture) // <<< INITIALIZE the texture reference
{
    ants.reserve(initialNumAnts + 100);
    spawnAnts(initialNumAnts);
}

//Destructor
Colony::~Colony() {
    ants.clear();
}

// Increment the number of ants to spawn this turn by 1
//void Colony::requestAntSpawn() {
//    m_antsToSpawnThisTurn++;
//}
// Old: requestAntSpawn. NEW: Adds a unit of food to the colony's stored supply
void Colony::addFood(unsigned int amount) {
    foodStored += amount; // Increment foodStored
}

// Used for deferred spawning
void Colony::spawnAnts(int numAntsToSpawn) {
    for (int i = 0; i < numAntsToSpawn; i++) {
        // Pass the colony's color, its pheromone grids, ID, AND the ant texture
        ants.emplace_back(homeX, homeY, homeX, homeY, m_antsCellSize, this->colonyColor,
            foodPheromones, returnHomePheromones, this->id, m_antTexture); // <<< PASS texture
    }
}

void Colony::update(Environment& env, const std::vector<Colony>& allColonies) {
   // m_antsToSpawnThisTurn = 0;

    for (size_t i = 0; i < ants.size(); ++i) {
        ants[i].updateSelf(env, *this);
    }

    size_t antsBeforeErase = ants.size(); // Get count before erase
    //lambda to rearrange elements so that all ants that satisfy the condition (ant.isDead() == true) are moved to the end of the vector.
	//std::remove_if - removes those elements from the vector, effectively erasing them with ants.erase.
    ants.erase(std::remove_if(ants.begin(), ants.end(),
        [](const Ant& ant) { return ant.isDead(); }),
        ants.end());
    size_t antsAfterErase = ants.size(); // Get count after erase

    totalAntsDied += (antsBeforeErase - antsAfterErase); // Increment totalAntsDied


    // Check if enough food is stored to spawn new ants
    while (foodStored >= FOOD_REQUIRED_PER_ANT_SPAWN) { // While loop to spawn multiple if enough food
        spawnAnts(1); // Spawn one ant
        foodStored -= FOOD_REQUIRED_PER_ANT_SPAWN; // Consume the food
    }

	// Update peak population if the current number of ants exceeds it
    if (ants.size() > peakPopulation) {
        peakPopulation = ants.size();
    }
	// Update pheromones after all ants have been updated
    updatePheromones();
}

// Colony Pheromone management methods
void Colony::addFoodPheromone(int gridX, int gridY, float amount) {
    if (gridX >= 0 && gridX < Environment::GRID_SIZE && gridY >= 0 && gridY < Environment::GRID_SIZE) {
        foodPheromones[gridX][gridY] += amount; // Access using std::vector syntax
        if (foodPheromones[gridX][gridY] > MAX_PHEROMONE_LEVEL) {
            foodPheromones[gridX][gridY] = MAX_PHEROMONE_LEVEL;
        }
        if (foodPheromones[gridX][gridY] < 0) {
            foodPheromones[gridX][gridY] = 0.0f;
        }
    }
}

float Colony::getFoodPheromoneLevel(int gridX, int gridY) const {
    if (gridX >= 0 && gridX < Environment::GRID_SIZE && gridY >= 0 && gridY < Environment::GRID_SIZE) {
        return foodPheromones[gridX][gridY];
    }
    return 0.0f;
}

void Colony::addReturnHomePheromone(int gridX, int gridY, float amount) {
    if (gridX >= 0 && gridX < Environment::GRID_SIZE && gridY >= 0 && gridY < Environment::GRID_SIZE) {
        returnHomePheromones[gridX][gridY] += amount;
        if (returnHomePheromones[gridX][gridY] > MAX_PHEROMONE_LEVEL) {
            returnHomePheromones[gridX][gridY] = MAX_PHEROMONE_LEVEL;
        }
        if (returnHomePheromones[gridX][gridY] < 0.0f) {
            returnHomePheromones[gridX][gridY] = 0.0f;
        }
    }
}

float Colony::getReturnHomePheromoneLevel(int gridX, int gridY) const {
    if (gridX >= 0 && gridX < Environment::GRID_SIZE && gridY >= 0 && gridY < Environment::GRID_SIZE) {
        return returnHomePheromones[gridX][gridY];
    }
    return 0.0f;
}

void Colony::updatePheromones() {
    for (int i = 0; i < Environment::GRID_SIZE; ++i) {
        for (int j = 0; j < Environment::GRID_SIZE; ++j) {
            if (foodPheromones[i][j] > 0.001f) {
                foodPheromones[i][j] *= PHEROMONE_DECAY_RATE;
                if (foodPheromones[i][j] < 0.001f) foodPheromones[i][j] = 0.0f;
            }
            else {
                foodPheromones[i][j] = 0.0f;
            }

            if (returnHomePheromones[i][j] > 0.001f) {
                returnHomePheromones[i][j] *= PHEROMONE_DECAY_RATE;
                if (returnHomePheromones[i][j] < 0.001f) returnHomePheromones[i][j] = 0.0f;
            }
            else {
                returnHomePheromones[i][j] = 0.0f;
            }
        }
    }
}