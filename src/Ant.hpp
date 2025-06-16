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

#ifndef ANT_HPP
#define ANT_HPP

#include "Environment.hpp"
#include <SFML/Graphics.hpp>
#include "RandomUtils.hpp"
#include <utility>
#include <deque>
#include <memory>
#include <vector>

// Forward declarations
class Colony;

class Ant {
public:
    // Position & State (Grid coordinates)
    int x, y;
    int prevX, prevY;
    int direction;
    bool hasFood;
    float pheromoneStrength;

    // Colony Coordinates
    int homeX, homeY;

    // SFML Shape for Rendering Ant
    sf::Sprite sprite;

    // Lifespan
    int lifespan;
    static const int MAX_LIFESPAN = 1000;

    //future feature Combat Attributes(health, attackDamage, BASE_HEALTH etc.)

    // Behavior constants
    static const int MAX_PHEROMONE_RETURN_ATTEMPTS = 10;
    static const int MAX_TOTAL_RETURN_ATTEMPTS = 150;
    static constexpr float HOME_PROXIMITY_THRESHOLD = 8.0f; 

    sf::Color getColonyColor() const {
        return m_colonyColor; 
    }

    int getColonyID() const { 
        return m_colonyID;
    }
    // future feature getAntType()
    // future feature getHealth()
    // future feature getAttackDamage()

	// No longer managing unique_ptr for shape directly in Ant class.
    // --- CRITICAL: Manually define move constructor and move assignment operator ---
     // This removes any ambiguity and forces the compiler to use these specific implementations.
	// This is necessary because Ant contains a std::unique_ptr, which cannot be copied but can be moved.
	// If you do not define these, the compiler will generate default move operations that may not work as intended.
	// I ran into issues with the default move constructor and assignment operator not working correctly
    Ant(Ant&& other) noexcept; // Manually defined move constructor
    Ant& operator=(Ant&& other) noexcept; // Manually defined move assignment operator

    // Copy constructor and copy assignment operator implicitly deleted (due to unique_ptr),
    // or explicitly deleted by: Ant(const Ant&) = delete; Ant& operator=(const Ant&) = delete; 

    // Constructor
    Ant(int startX, int startY, int colonyX, int colonyY, float antCellSize, const sf::Color& colonyColor,
        std::vector<std::vector<float>>(&foodPheromones), // Reference to colony's food pheromone grid
        std::vector<std::vector<float>>(&returnHomePheromones),// Reference to colony's home pheromone grid
        int colonyID,
        const sf::Texture& antTexture);


    // Destructor
    ~Ant();

    // Movement & Behavior
    void move(Environment& env);
    void wander(Environment& env);
    void searchForFood(Environment& env);
    void goHome(Colony& colony, Environment& env);
    void storeFood(Colony& colony);

    void updateSelf(Environment& env, Colony& colony);

    // Pheromone Interaction
    void depositFoodPheromones(Environment& env);
    void depositHomePheromones(Environment& env);
    void followFoodPheromones(Environment& env);
    bool followHomePheromones(Environment& env);

    // SFML Graphics Integration
    void updateGraphics();

    // Lifespan & Combat future features
    bool isDead() const;
    // takeDamage(int amount)
    // isAlive()
    // isEnemy(const Ant& other)
    // getDistanceTo(const Ant& other)
private:
    float m_cellSize;
    static int generateRand(int maxValue);
	std::deque<std::pair<int, int>> recentPositions; // Ants shorterm memory of positions to avoid loops
	int memoryLength; // Length of the ants short term memory for recent positions
	int movesWhileReturningHome; // helper variable to track moves while returning home

    //future feature AntType m_antType;
   
    sf::Color m_colonyColor; // Colony Color
    int m_colonyID;
    // --- References to the colony's pheromone grids ---
    std::vector<std::vector<float>>(&m_foodPheromones);
    std::vector<std::vector<float>>(&m_returnHomePheromones);

};

#endif // ANT_HPP