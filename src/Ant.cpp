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
#include "Ant.hpp"
#include "Colony.hpp"
#include "Environment.hpp"
#include "RandomUtils.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <deque>

// HOME PROXIMITY
static constexpr float HOME_PROXIMITY_THRESHOLD = 8.0f; // Distance threshold for direct homing

// generateRand: Generates a random integer between 0 and maxValue (inclusive)
int Ant::generateRand(int maxValue) {
    std::uniform_int_distribution<> distrib(0, maxValue);
    return distrib(RandomUtils::getGenerator());
}

// Constructor
Ant::Ant(int startX, int startY, int colonyX, int colonyY, float antCellSize, const sf::Color& colonyColor,
    std::vector<std::vector<float>>& foodPheromones,
    std::vector<std::vector<float>>& returnHomePheromones,
    int colonyID)
    : x(startX),
    y(startY),
    prevX(startX),
    prevY(startY),
    direction(Ant::generateRand(7)),
    hasFood(false),
    pheromoneStrength(100.0f),
    homeX(colonyX),
    homeY(colonyY),
    m_cellSize(antCellSize),
    lifespan(MAX_LIFESPAN),
    memoryLength(10),
    movesWhileReturningHome(0),
    m_colonyColor(colonyColor),
    m_colonyID(colonyID),
    m_foodPheromones(foodPheromones),
    m_returnHomePheromones(returnHomePheromones)
{

    shape = std::make_unique<sf::RectangleShape>(sf::Vector2f(m_cellSize, m_cellSize));
    shape->setPosition(static_cast<float>(x * m_cellSize), static_cast<float>(y * m_cellSize));
    recentPositions.assign(1, std::make_pair(x, y));
}

// Manually defined move constructor was having trouble with unique_ptr and deque
// needed to define it explicitly
Ant::Ant(Ant&& other) noexcept
    : x(other.x),
    y(other.y),
    prevX(other.prevX),
    prevY(other.prevY),
    direction(other.direction),
    hasFood(other.hasFood),
    pheromoneStrength(other.pheromoneStrength),
    homeX(other.homeX),
    homeY(other.homeY),
    m_cellSize(other.m_cellSize),
    lifespan(other.lifespan),
    memoryLength(other.memoryLength),
    movesWhileReturningHome(other.movesWhileReturningHome),
    m_colonyColor(other.m_colonyColor),
    m_colonyID(other.m_colonyID),
    m_foodPheromones(other.m_foodPheromones),
    m_returnHomePheromones(other.m_returnHomePheromones),
    shape(std::move(other.shape)),
    recentPositions(std::move(other.recentPositions))
{
}

// Manually defined move assignment operator was having trouble with unique_ptr and deque
// same as move constructor, but with assignment logic
Ant& Ant::operator=(Ant&& other) noexcept {
    if (this != &other) {
        x = other.x;
        y = other.y;
        prevX = other.prevX;
        prevY = other.prevY;
        direction = other.direction;
        hasFood = other.hasFood;
        pheromoneStrength = other.pheromoneStrength;
        homeX = other.homeX;
        homeY = other.homeY;
        m_cellSize = other.m_cellSize;
        lifespan = other.lifespan;
        memoryLength = other.memoryLength;
        movesWhileReturningHome = other.movesWhileReturningHome;
        m_colonyColor = other.m_colonyColor;
        m_colonyID = other.m_colonyID;

        // References cannot be reassigned. They must refer to the same object throughout their lifetime.
        // Move the unique_ptr content
        shape = std::move(other.shape);
        // Move the deque content
        recentPositions = std::move(other.recentPositions);

        // Clear out the moved-from object's data where appropriate (optional, but good practice)
		other.x = 0; other.y = 0; // Or other default values
    }
    return *this;
}


Ant::~Ant()
{
    // Destructor content if needed. unique_ptr will automatically delete the shape.
}

// Main update logic for the ant for a single turn
void Ant::updateSelf(Environment& env, Colony& colony) {

    // Decrement lifespan at the start of each update cycle
    if (lifespan > 0) {
        lifespan--;
    }

    // This is the combined FORAGER/EXPLORER behavior for the single ant type
    if (hasFood) { // Ant has food - returning home
        bool atHome = (this->x == this->homeX && this->y == this->homeY);

        if (atHome) {
			storeFood(colony); // This sets hasFood = false and resets movesWhileReturningHome counter
			lifespan++; // Reset lifespan when returning home with food
        }
        else {
            // Not at home, but has food. Try to navigate home.
            int oldX = this->x;
            int oldY = this->y;
            float distToHomeBeforeMove = std::sqrt(static_cast<float>(std::pow(this->x - this->homeX, 2) + std::pow(this->y - this->homeY, 2)));

            // --- Prioritize direct homing for food-carrying ants ---
            if (distToHomeBeforeMove <= HOME_PROXIMITY_THRESHOLD) { // If very close, force direct homing
                goHome(colony, env);
            }
            else if (this->movesWhileReturningHome < MAX_TOTAL_RETURN_ATTEMPTS) { // Use MAX_TOTAL_RETURN_ATTEMPTS as the limit for direct goHome attempts
                goHome(colony, env);
            }
            else {
                // This is the fallback for when an ant has been trying to go home for an extremely long time
                // (exceeding MAX_TOTAL_RETURN_ATTEMPTS), suggesting it's genuinely stuck or off-map.
                direction = Ant::generateRand(7); // Allow random wander if truly lost
                move(env);
            }

            // Increment counter for every turn spent trying to return home and not yet there.
            if (!(this->x == this->homeX && this->y == this->homeY)) {
                this->movesWhileReturningHome++;
            }

            // Sanity Check:
            bool movedToNewCell = (this->x != oldX || this->y != oldY);
            float distToHomeAfterMove = std::sqrt(static_cast<float>(std::pow(this->x - this->homeX, 2) + std::pow(this->y - this->homeY, 2)));

            // If it didn't move OR didn't make progress towards home (and not already in direct homing zone)
            if ((!movedToNewCell || distToHomeAfterMove >= distToHomeBeforeMove - 0.1f) &&
                distToHomeBeforeMove > HOME_PROXIMITY_THRESHOLD) {
                this->movesWhileReturningHome++;
            }

            // After attempting to move, check again if now at home
            if (this->x == this->homeX && this->y == this->homeY) {
                storeFood(colony);
            }
            // Only deposit food pheromones if not in the "truly lost" wandering phase
            else if (hasFood && this->movesWhileReturningHome < MAX_TOTAL_RETURN_ATTEMPTS) {
                depositFoodPheromones(env);
            }
        }
    }
    else { // Ant does not have food - searching
        // Special behavior for ants that just dropped off food
        if (this->movesWhileReturningHome == 0 && this->prevX == this->homeX && this->prevY == this->homeY) {
            if (Ant::generateRand(99) < 3) { // 3% chance to wander
                wander(env); // Explore randomly
            }
            else { // 95% chance to follow food trails
                followFoodPheromones(env); // Directly try to follow food trails
            }
            // An ant leaving the nest, regardless of its path, always drops home pheromones
            depositHomePheromones(env);
        }
        else { // Normal searching behavior for ants that have been out for a while
            searchForFood(env);
            if (!hasFood) {
                depositHomePheromones(env); // Drop the To Home Pheromone exploration trail
                followFoodPheromones(env); // This will wander if no food trails are found
            }
            else { // Food was just found by searchForFood()!
                this->pheromoneStrength = 20.0f;
                depositFoodPheromones(env);
                this->movesWhileReturningHome = 0;
            }
        }
    }
}

// Movement Logic: Ant moves one step based on its current direction (8 directions)
void Ant::move(Environment& env) {
    //set previous positions before moving
    this->prevX = this->x;
    this->prevY = this->y;

    //direction to move
    switch (direction) {
    case 0: y -= 1; break;             // North
    case 1: x += 1; y -= 1; break;    // Northeast
    case 2: x += 1; break;             // East
    case 3: x += 1; y += 1; break;    // Southeast
    case 4: y += 1; break;             // South
    case 5: x -= 1; y += 1; break;    // Southwest
    case 6: x -= 1; break;             // West
    case 7: x -= 1; y -= 1; break;    // Northwest
    }

    //boundary check
    bool hitBoundary = false;
    if (this->x < 0) {
        this->x = 0;
        hitBoundary = true;
    }
    else if (this->x >= Environment::GRID_SIZE) {
        this->x = Environment::GRID_SIZE - 1;
        hitBoundary = true;
    }
    if (this->y < 0) {
        this->y = 0;
        hitBoundary = true;
    }
    else if (this->y >= Environment::GRID_SIZE) {
        this->y = Environment::GRID_SIZE - 1;
        hitBoundary = true;
    }

    // When a boundary is hit, force a significant turn.
    if (hitBoundary) {
        // This makes the ant turn roughly 90-135 degrees left or right, or turn around.
        int turnDecision = generateRand(2); // 0 for left-ish, 1 for right-ish, 2 for turnaround
        int baseTurnAmount = 2; // For ~90 degrees (2 * 45 deg segment)

        if (turnDecision == 0) { // Turn "left-ish"
            direction = (this->direction - baseTurnAmount - generateRand(1) + 8) % 8; // +8 to handle negative results before modulo
        }
        else if (turnDecision == 1) { // Turn "right-ish"
            direction = (this->direction + baseTurnAmount + generateRand(1)) % 8;
        }
        else { // Turn around
            direction = (this->direction + 4) % 8;
        }
    }

    // Reflect the movement changes graphically
    updateGraphics();

    // Update recent positions ants short term memory
    recentPositions.push_back(std::make_pair(x, y));
    if (recentPositions.size() > memoryLength) {
        recentPositions.pop_front(); // O(1) pop_front
    }
}

// Wander function for 8 directions
void Ant::wander(Environment& env) {
    const int dx[] = { 0, 1, 1, 1, 0, -1, -1, -1 }; // N, NE, E, SE, S, SW, W, NW
    const int dy[] = { -1, -1, 0, 1, 1, 1, 0, -1 };

    bool decidedToContinueCurrentDir = false;

    // --- Start of Directional Inertia Logic (70% chance to try) ---
    if (generateRand(100) < 70) {
        int currentDir = this->direction; // The direction the ant is already facing
        int nextX = this->x + dx[currentDir];
        int nextY = this->y + dy[currentDir];

        bool canContinue = true;

        // Check if continuing in the current direction is valid
        if (nextX < 0 || nextX >= Environment::GRID_SIZE ||
            nextY < 0 || nextY >= Environment::GRID_SIZE) {
            canContinue = false; // Would hit a boundary
        }
        else if (nextX == this->prevX && nextY == this->prevY) {
            canContinue = false; // Would be going directly back
        }
        else {
            // Check short-term memory
            for (const auto& pos : recentPositions) {
                if (pos.first == nextX && pos.second == nextY) { // 'Remembering' where it's previously been
                    canContinue = false; // Would be a recently visited spot
                    break;
                }
            }
        }

        if (canContinue) {
            // The current direction is fine to continue with.
            decidedToContinueCurrentDir = true;
        }
        else {
        }
    }
    // --- End of Directional Inertia Logic ---

    // If the ant didn't decide to continue its current path (either by chance or because it was bad)
    // then use the existing logic to find a new good direction.
    if (!decidedToContinueCurrentDir) {
        std::vector<int> potentialGoodDirections;

        for (int i = 0; i < 8; ++i) {
            int testDir = i;

            int neighborX = this->x + dx[testDir];
            int neighborY = this->y + dy[testDir];

            //check within bounds
            if (neighborX < 0 || neighborX >= Environment::GRID_SIZE ||
                neighborY < 0 || neighborY >= Environment::GRID_SIZE) {
                continue;
            }
            // check previous x,y
            if (neighborX == this->prevX && neighborY == this->prevY) {
                continue;
            }

            bool wasRecentlyVisited = false;
            //check short term memory
            for (const auto& pos : recentPositions) {
                if (pos.first == neighborX && pos.second == neighborY) {
                    wasRecentlyVisited = true;
                    break;
                }
            }
            // if ant has not been there before add direction to potential 'good' directions vector
            if (!wasRecentlyVisited) {
                potentialGoodDirections.push_back(testDir);
            }
        }
        // if the good directions vector is not empty pick randomly from the good directions vector
        if (!potentialGoodDirections.empty()) {
            this->direction = potentialGoodDirections[generateRand(potentialGoodDirections.size() - 1)];
        }
        else {
            // All neighbors are either previous cell or recently visited or out of bounds.
            // Fallback: pick a completely random direction.
            this->direction = generateRand(7);
        }
    }

    move(env); // ALWAYS attempt to move when wandering to ensure exploration
}

// Food Search Logic: Ant checks current cell and 8 surrounding cells for food
void Ant::searchForFood(Environment& env) {
    if (env.checkForFood(x, y)) { // check current cell for food has food when env.checkForFood returns true
        hasFood = true; // set hasFood to true
        env.removeFood(x, y); // remove the food source from the environment because the ant picked it up
        shape->setFillColor(sf::Color::Green); // Ants with food turn Green colored when returning home
        return;
    }

    const int dx[] = { 0, 1, 1, 1, 0, -1, -1, -1 }; // N, NE, E, SE, S, SW, W, NW
    const int dy[] = { -1, -1, 0, 1, 1, 1, 0, -1 };

    // check the 8 directions around the ant for food
    for (int i = 0; i < 8; ++i) {
        int checkX = this->x + dx[i];
        int checkY = this->y + dy[i];

        if (checkX < 0 || checkX >= Environment::GRID_SIZE || checkY < 0 || checkY >= Environment::GRID_SIZE) {
            continue;
        }

        if (env.checkForFood(checkX, checkY)) {
            this->prevX = this->x; // Store current before moving
            this->prevY = this->y;
            this->x = checkX;      // Move to adjacent food cell
            this->y = checkY;
            hasFood = true; // cell checkX,checkY has food set to true
            env.removeFood(checkX, checkY);
            shape->setFillColor(sf::Color::Green); // Turn ant green since it has food
            updateGraphics(); // Reflect changes graphically
            return;
        }
    }
}

// Pheromone Following for searching ants (!hasFood)
void Ant::followFoodPheromones(Environment& env) { // This is called ONLY when ant does not have food by updateSelf
    // Get "to-food" pheromone level at current ant's cell
    float currentPheromoneOnCell = m_foodPheromones[x][y];

    // If on a very strong "to-food" pheromone spot, small chance to explore locally (wander)
    // This helps prevent ants from clustering too much right on the trail at the end if food is nearby.
    if (currentPheromoneOnCell > 25.0f && generateRand(100) < 5) { // Threshold was 15, prob was 40. Now 25 & 5%
        wander(env); // wander off trail to hopefully find new food
        return;
    }

    const int dx[] = { 0, 1, 1, 1, 0, -1, -1, -1 }; // N, NE, E, SE, S, SW, W, NW
    const int dy[] = { -1, -1, 0, 1, 1, 1, 0, -1 };

    std::vector<float> candidatePheromonesWeights;
    std::vector<int> candidateDirections; // Stores the direction enum (0-7)
    float totalWeightSum = 0.0f;

    // Using Euclidean distance for home (though Manhattan was used before when I only used 4 directions, sqrt is more accurate for 8 directions diagonal bias)
    float currentDistToHome = std::sqrt(static_cast<float>(std::pow(this->x - this->homeX, 2) + std::pow(this->y - this->homeY, 2)));

    for (int i = 0; i < 8; ++i) { // Check all 8 directions
        int potentialNewDir = i;
        int neighborX = this->x + dx[i];
        int neighborY = this->y + dy[i];

        if (neighborX < 0 || neighborX >= Environment::GRID_SIZE ||
            neighborY < 0 || neighborY >= Environment::GRID_SIZE) {
            continue;
        }
        if (neighborX == this->prevX && neighborY == this->prevY) {
            continue;
        }

        bool wasRecentlyVisited = false;
        // short term memory
        for (const auto& pos : recentPositions) {
            if (pos.first == neighborX && pos.second == neighborY) {
                wasRecentlyVisited = true;
                break;
            }
        }

        // Get the pheromone level from colony's grid
        float pheromoneLevel = m_foodPheromones[neighborX][neighborY];

        if (pheromoneLevel > 0) {
            float weight = pheromoneLevel;

            if (wasRecentlyVisited) {
                weight *= 0.5f; // Penalty for recently visited, was 0.1f, now less severe
            }

            // Logic for searching ants without food (!hasFood) to prefer paths away from home
            float neighborDistToHome = std::sqrt(static_cast<float>(std::pow(neighborX - this->homeX, 2) + std::pow(neighborY - this->homeY, 2)));
            if (neighborDistToHome < currentDistToHome) { // Path leads TOWARDS home
                weight *= 0.2f; // Penalize, was 0.1f
            }
            else if (neighborDistToHome > currentDistToHome) { // Path leads AWAY from home
                weight *= 1.2f; // Boost, was 1.5f
            }

            // Diagonal move bonus (directions 1, 3, 5, 7 are diagonal)
            if (potentialNewDir % 2 != 0) {
                weight *= 1.1f;
            }

            if (weight > 0.001f) { // Slightly lower threshold for considering a path
                candidatePheromonesWeights.push_back(weight);
                candidateDirections.push_back(potentialNewDir);
                totalWeightSum += weight;
            }
        }
    }

    if (candidateDirections.empty() || (totalWeightSum <= 0.1f && generateRand(100) < 20)) { // lower sum threshold, lower wander chance
        wander(env);
    }
    else {
        std::uniform_real_distribution<float> distrib(0.0f, totalWeightSum);
        float randomPick = distrib(RandomUtils::getGenerator());
        float currentSum = 0.0f;
        int chosenDir = -1;

        for (size_t j = 0; j < candidatePheromonesWeights.size(); ++j) {
            currentSum += candidatePheromonesWeights[j];
            if (randomPick <= currentSum) {
                chosenDir = candidateDirections[j];
                break;
            }
        }

        if (chosenDir == -1 && !candidateDirections.empty()) { // Fallback logic
            float maxW = 0.0f;
            int bestDir = candidateDirections[0];
            for (size_t k = 0; k < candidateDirections.size(); ++k) {
                if (candidatePheromonesWeights[k] > maxW) {
                    maxW = candidatePheromonesWeights[k];
                    bestDir = candidateDirections[k];
                }
            }
            chosenDir = bestDir;
        }

        if (chosenDir != -1) {
            this->direction = chosenDir;
            move(env);
        }
        else { //if nothing else works, wander
            wander(env);
        }
    }
}


// Pheromone Following for ants trying to follow the go home trail (hasFood) or lost
// Basically does the opposite of followFoodPheromones, but with "to-home" pheromones
bool Ant::followHomePheromones(Environment& env) {
    const int dx[] = { 0, 1, 1, 1, 0, -1, -1, -1 }; // N, NE, E, SE, S, SW, W, NW
    const int dy[] = { -1, -1, 0, 1, 1, 1, 0, -1 };

    // --- START: PRIORITY GEOMETRIC HOMING for food-carrying ants ---
    if (this->hasFood) {
        // Check if the ant is directly adjacent (includes diagonals) to its home cell,
        // but not already on it. Help ant find home
        bool isAdjacentToHome = (std::abs(this->x - this->homeX) <= 1 &&
            std::abs(this->y - this->homeY) <= 1) &&
            !(this->x == this->homeX && this->y == this->homeY);

        if (isAdjacentToHome) {
            for (int i = 0; i < 8; ++i) { // Check all 8 potential directions
                int neighborX = this->x + dx[i];
                int neighborY = this->y + dy[i];

                if (neighborX == this->homeX && neighborY == this->homeY) {
                    // This direction 'i' leads straight to the home cell.
                    this->direction = i;
                    move(env);
                    return true; // Successfully made a direct move to home
                }
            }
        }
    }
    // --- END: REFINED PRIORITY GEOMETRIC HOMING ---

    // If geometric homing didn't apply (not adjacent) or didn't result in a move,
    // or if ant does not have food !hasFood, proceed with standard pheromone evaluation:

    float currentHomePheromoneOnCell = m_returnHomePheromones[x][y];
    if (!this->hasFood && currentHomePheromoneOnCell > 30.0f && generateRand(100) < 5) {
        wander(env);
        return false;
    }

    std::vector<float> candidatePheromonesWeights;
    std::vector<int> candidateDirections;
    float totalWeightSum = 0.0f;
    float currentDistToHome = std::sqrt(static_cast<float>(std::pow(this->x - this->homeX, 2) + std::pow(this->y - this->homeY, 2)));

    for (int i = 0; i < 8; ++i) {
        int potentialNewDir = i;
        int neighborX = this->x + dx[i];
        int neighborY = this->y + dy[i];

        if (neighborX < 0 || neighborX >= Environment::GRID_SIZE || neighborY < 0 || neighborY >= Environment::GRID_SIZE) continue;
        if (neighborX == this->prevX && neighborY == this->prevY) continue;

        bool wasRecentlyVisited = false;
        for (const auto& pos : recentPositions) {
            if (pos.first == neighborX && pos.second == neighborY) {
                wasRecentlyVisited = true;
                break;
            }
        }
        //Get pheromone level from colony grid
        float pheromoneLevel = m_returnHomePheromones[neighborX][neighborY];
        if (pheromoneLevel > 0.001f) {
            float weight = pheromoneLevel;
            if (wasRecentlyVisited) {
                // Adaptive penalty for recently visited still applies if not taking the geometric step
                if (this->hasFood && currentDistToHome < 2.5f) weight *= 0.9f;
                else weight *= 0.5f;
            }
            float neighborDistToHome = std::sqrt(static_cast<float>(std::pow(neighborX - this->homeX, 2) + std::pow(neighborY - this->homeY, 2)));
            if (neighborDistToHome < currentDistToHome) weight *= 2.0f;
            else if (neighborDistToHome > currentDistToHome) weight *= 0.1f;
            if (potentialNewDir % 2 != 0) weight *= 1.1f;

            if (weight > 0.001f) {
                candidatePheromonesWeights.push_back(weight);
                candidateDirections.push_back(potentialNewDir);
                totalWeightSum += weight;
            }
        }
    }

    int chosenDir = -1;

    if (candidateDirections.empty()) {
        if (!this->hasFood) {
            wander(env);
        }
        return false;
    }

    if (!this->hasFood && totalWeightSum <= 0.1f && generateRand(100) < 20) {
        wander(env);
        return false;
    }

    if (this->hasFood) {
        float maxW = -1.0f;
        for (size_t k = 0; k < candidateDirections.size(); ++k) {
            if (candidatePheromonesWeights[k] > maxW) {
                maxW = candidatePheromonesWeights[k];
                chosenDir = candidateDirections[k];
            }
        }
        if (chosenDir == -1 && !candidateDirections.empty()) {
            chosenDir = candidateDirections[0];
        }
    }
    else {
        std::uniform_real_distribution<float> distrib(0.0f, totalWeightSum);
        float randomPick = distrib(RandomUtils::getGenerator());
        float currentSum = 0.0f;
        for (size_t j = 0; j < candidatePheromonesWeights.size(); ++j) {
            currentSum += candidatePheromonesWeights[j];
            if (randomPick <= currentSum) {
                chosenDir = candidateDirections[j];
                break;
            }
        }
        if (chosenDir == -1 && !candidateDirections.empty()) {
            float maxW = 0.0f; int bestDir = candidateDirections[0];
            for (size_t k = 0; k < candidateDirections.size(); ++k) {
                if (candidatePheromonesWeights[k] > maxW) {
                    maxW = candidatePheromonesWeights[k]; bestDir = candidateDirections[k];
                }
            }
            chosenDir = bestDir;
        }
    }

    if (chosenDir != -1) {
        this->direction = chosenDir;
        move(env);
        return true;
    }
    else {
        if (!this->hasFood) {
            wander(env);
        }
        return false;
    }
}

// Storing Food in Colony
void Ant::storeFood(Colony& colony) {
    hasFood = false;
    pheromoneStrength += 10.0f; // Replenish pheromone charge
    shape->setFillColor(sf::Color::Black);
    colony.addFood(1);
}

// Improved goHome function to use 8 directions, takes Environment for move()
void Ant::goHome(Colony& colony, Environment& env) {
    int targetX = homeX;
    int targetY = homeY;

    int deltaX = targetX - x;
    int deltaY = targetY - y;

    // Determine the best direction using 8 directions
    if (deltaX == 0 && deltaY == 0) { // Already home
        if (hasFood) storeFood(colony);
        return;
    }

    // Simplified direction setting towards target
    if (deltaX > 0) { // Need to move East, SE, or NE
        if (deltaY > 0) direction = 3;      // Southeast
        else if (deltaY < 0) direction = 1; // Northeast
        else direction = 2;                 // East
    }
    else if (deltaX < 0) { // Need to move West, SW, or NW
        if (deltaY > 0) direction = 5;      // Southwest
        else if (deltaY < 0) direction = 7; // Northwest
        else direction = 6;                 // West
    }
    else { // deltaX is 0, so N or S
        if (deltaY > 0) direction = 4;      // South
        else direction = 0;                 // North (deltaY must be < 0 if not 0,0)
    }

    move(env); // This also updates prevX, prevY

    // Check if arrived home at the colony after moving
    if (x == homeX && y == homeY && hasFood) {
        storeFood(colony);
    }
}

// Deposit Food Pheromones into the Environment
void Ant::depositFoodPheromones(Environment& env) {
    if (this->hasFood && this->pheromoneStrength > 0.05f) { // Lower threshold slightly
		float amountToDeposit = 60.0f; // Amount of food Pheromones to drop
        // Use colony's specific pheromone grid
        m_foodPheromones[x][y] += amountToDeposit;
        if (m_foodPheromones[x][y] > Colony::MAX_PHEROMONE_LEVEL) {
            m_foodPheromones[x][y] = Colony::MAX_PHEROMONE_LEVEL;
        }
        this->pheromoneStrength -= 0.1f; // CRITICAL: Reduced from 0.5f to 0.1f to match home pheromones
        if (this->pheromoneStrength < 0.0f) this->pheromoneStrength = 0.0f;
    }
}

// Deposit Home Pheromones into the Environment
void Ant::depositHomePheromones(Environment& env) {
    if (!this->hasFood && this->pheromoneStrength > 0.1f) {
        float amountToDeposit = 50.5f; // Amount of home Pheromones to drop
        // Use colony's specific pheromone grid
        m_returnHomePheromones[x][y] += amountToDeposit;
        if (m_returnHomePheromones[x][y] > Colony::MAX_PHEROMONE_LEVEL) {
            m_returnHomePheromones[x][y] = Colony::MAX_PHEROMONE_LEVEL;
        }
        this->pheromoneStrength -= 0.1f;
        if (this->pheromoneStrength < 0.0f) this->pheromoneStrength = 0.0f;
    }
}

// Update SFML Graphics to reflect the ant's current grid position
void Ant::updateGraphics() {
    shape->setPosition(static_cast<float>(x * m_cellSize), static_cast<float>(y * m_cellSize));
}

// Lifespan & Combat future features
bool Ant::isDead() const {
    return lifespan <= 0;
}