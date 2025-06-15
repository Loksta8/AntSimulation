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


#include <SFML/Graphics.hpp>
#include "Ant.hpp"
#include "Colony.hpp"
#include "Environment.hpp"
#include "RandomUtils.hpp" // For RandomUtils::getGenerator()
#include <random>          // For std::uniform_int_distribution
#include <iostream>
#include <algorithm> // For std::min, std::max
#include <string>
#include <vector>
#include <memory>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

// For key-based zoom control
const float KEY_ZOOM_FACTOR = 1.1f; // Pressing key zooms in/out by 10%
// For mouse wheel zoom control
const float MOUSE_WHEEL_ZOOM_FACTOR = 1.1f; // Scrolling zooms in/out by 10%
// For panning control
const float PAN_SPEED_FACTOR = 0.05f; // Panning speed relative to view size (for keys)

// --- Simulation States ---
enum SimulationState {
    RUNNING,          // Simulation is actively running
    WAITING_FOR_RESET // Conditions met, waiting for 3-second delay
};

// Forward declaration for the reset function
void resetSimulation(std::vector<Colony>& colonies, Environment& env, sf::Clock& gameClock, float cellSize, sf::View& view, float initialZoom);


constexpr float CELL_SIZE = static_cast<float>(WINDOW_WIDTH) / Environment::GRID_SIZE;
const float INITIAL_DEFAULT_ZOOM_OUT = 1.4f; // Default zoom level

int main() {

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Ant Colony Simulation");
    window.setFramerateLimit(60);

    // --- Load a Font ---
    sf::Font font;
    bool fontLoaded = false;

    // Try multiple locations for the font
    std::vector<std::string> fontPaths = {
        "Vertiky.ttf",                    // Current working directory
        "resources/Vertiky.ttf",          // Resources subdirectory
        "../resources/Vertiky.ttf",       // One level up resources directory
        "Fonts/Vertiky.ttf",              // Fonts subdirectory
        "../Fonts/Vertiky.ttf"            // One level up fonts directory
    };

    for (const auto& path : fontPaths) {
        if (font.loadFromFile(path)) {
            //std::cout << "Successfully loaded font from: " << path << std::endl;
            fontLoaded = true;
            break;
        }
    }

	// If custom font couldn't be loaded, try system fonts as fallback so that the application can still run
    if (!fontLoaded) {
        std::cerr << "Warning: Could not load Vertiky.ttf, trying system fonts..." << std::endl;

        std::vector<std::string> systemFontPaths;

#ifdef _WIN32
        systemFontPaths = {
            "C:\\Windows\\Fonts\\arial.ttf",
            "C:\\Windows\\Fonts\\verdana.ttf"
        };
#elif defined(__APPLE__)
        systemFontPaths = {
            "/System/Library/Fonts/Helvetica.ttc",
            "/Library/Fonts/Arial.ttf"
        };
#else // Linux
        systemFontPaths = {
            "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
            "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
            "/usr/share/fonts/TTF/Arial.ttf"
        };
#endif

        for (const auto& path : systemFontPaths) {
            if (font.loadFromFile(path)) {
                std::cout << "Using system font: " << path << std::endl;
                fontLoaded = true;
                break;
            }
        }
    }

    if (!fontLoaded) {
        std::cerr << "Error: Could not load any font! Application will exit." << std::endl;
        return -1; // Exit if no font could be loaded
    }

	// --end Load Font---

    // --- Create sf::Text objects for displaying metrics ---
    sf::Text populationText;
    populationText.setFont(font);
    populationText.setCharacterSize(28);
    populationText.setFillColor(sf::Color::Black);
    populationText.setPosition(10.f, 10.f);

    sf::Text deathText;
    deathText.setFont(font);
    deathText.setCharacterSize(28);
    deathText.setFillColor(sf::Color::Red);
    deathText.setPosition(10.f, 70.f);

    sf::Text foodText;
    foodText.setFont(font);
    foodText.setCharacterSize(28);
    foodText.setFillColor(sf::Color::Blue);
    foodText.setPosition(10.f, 110.f);

	// Text for reset timer
    sf::Text resetTimerText;
    resetTimerText.setFont(font);
    resetTimerText.setCharacterSize(50);
    resetTimerText.setFillColor(sf::Color::Magenta);
    resetTimerText.setPosition(WINDOW_WIDTH / 2.f - 150.f, WINDOW_HEIGHT / 2.f - 20.f);


    // --- Initial Simulation Setup ---
    Environment env(CELL_SIZE);
    std::vector<Colony> colonies;
    colonies.reserve(3);

    std::uniform_int_distribution<> grid_distrib(0, Environment::GRID_SIZE - 1);

    sf::Color colony1Color = sf::Color::Black;
    sf::Color colony2Color = sf::Color::Red;
    sf::Color colony3Color = sf::Color::Blue;

    colonies.emplace_back(grid_distrib(RandomUtils::getGenerator()), grid_distrib(RandomUtils::getGenerator()), 5, CELL_SIZE, colony1Color, Colony::nextColonyID++);
    colonies.emplace_back(grid_distrib(RandomUtils::getGenerator()), grid_distrib(RandomUtils::getGenerator()), 5, CELL_SIZE, colony2Color, Colony::nextColonyID++);
    colonies.emplace_back(grid_distrib(RandomUtils::getGenerator()), grid_distrib(RandomUtils::getGenerator()), 5, CELL_SIZE, colony3Color, Colony::nextColonyID++);
    // --- End Initial Simulation Setup ---


    // --- View Setup ---
    sf::View view;
    float gridWorldDimension = static_cast<float>(Environment::GRID_SIZE) * CELL_SIZE;
    view.setSize(gridWorldDimension, gridWorldDimension);
    view.setCenter(gridWorldDimension / 2.0f, gridWorldDimension / 2.0f);
    view.zoom(INITIAL_DEFAULT_ZOOM_OUT);
    window.setView(view);
    // --- End of View Setup ---

    // Variables for mouse panning
    bool isPanning = false;
    sf::Vector2i lastMousePos;

	// Clock for simulation timing
    sf::Clock clock;
	//currently half a second per update, can be adjusted
	float simulationSpeed = 0.05f; // Speed of simulation in seconds per update

    SimulationState currentSimulationState = RUNNING;
	sf::Clock resetTimerClock; // Clock to track reset delay
    const float RESET_DELAY_SECONDS = 3.0f;


    while (window.isOpen()) {
        sf::Event event;
		// Process events
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::Resized) {
                float windowRatio = static_cast<float>(event.size.width) / static_cast<float>(event.size.height);
                float viewAspectRatio = view.getSize().x / view.getSize().y;
                float viewportWidth = 1.f, viewportHeight = 1.f, viewportX = 0.f, viewportY = 0.f;

                if (windowRatio > viewAspectRatio) {
                    viewportHeight = viewAspectRatio / windowRatio;
                    viewportY = (1.f - viewportHeight) / 2.f;
                }
                else if (windowRatio < viewAspectRatio) {
                    viewportWidth = windowRatio / viewAspectRatio;
                    viewportX = (1.f - viewportWidth) / 2.f;
                }
                view.setViewport(sf::FloatRect(viewportX, viewportY, viewportWidth, viewportHeight));
            }

            // --- Handle Mouse Button for Panning Start/End ---
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) { // Or sf::Mouse::Middle, etc.
                    isPanning = true;
                    lastMousePos = sf::Mouse::getPosition(window);
                }
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    isPanning = false;
                }
            }

            // --- Handle Mouse Movement for Panning ---
            if (event.type == sf::Event::MouseMoved) {
                if (isPanning) {
                    sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);
                    // Calculate the difference in pixel coordinates
                    sf::Vector2f deltaPixel = sf::Vector2f(lastMousePos - currentMousePos);

                    // Convert pixel delta to world delta.
                    // The amount the view moves in world coordinates should be proportional
                    // to how much of the world is visible.
                    // One way, is to scale the pixel delta by (world_view_size / window_pixel_size)
                    // For a square view and potentially non-square window, we can approximate
                    // or use the current view's size.
                    // view.getSize() gives world units. window.getSize() gives pixels.
                    // Need to map pixel movement on the screen to world movement.
                    // The view's current size (in world units) divided by the window's viewport size (in pixels)
                    // gives a ratio of world units per pixel.

                    // Get the actual viewport dimensions in pixels
                    sf::FloatRect viewport = view.getViewport();
                    sf::Vector2f viewportSizePixels(window.getSize().x * viewport.width, window.getSize().y * viewport.height);

                    float worldUnitsPerPixelX = view.getSize().x / viewportSizePixels.x;
                    float worldUnitsPerPixelY = view.getSize().y / viewportSizePixels.y;

                    sf::Vector2f worldDelta(deltaPixel.x * worldUnitsPerPixelX, deltaPixel.y * worldUnitsPerPixelY);

                    view.move(worldDelta);
                    lastMousePos = currentMousePos;
                    window.setView(view); // Apply the pan
                }
            }


            // --- Handle Mouse Wheel Scroll for Zooming ---
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPosBeforeZoom = window.mapPixelToCoords(pixelPos, view);

                    float zoomAmount = 1.0f;
                    if (event.mouseWheelScroll.delta > 0) {
                        zoomAmount /= MOUSE_WHEEL_ZOOM_FACTOR;
                    }
                    else if (event.mouseWheelScroll.delta < 0) {
                        zoomAmount *= MOUSE_WHEEL_ZOOM_FACTOR;
                    }

                    view.zoom(zoomAmount);
                    sf::Vector2f worldPosAfterZoom = window.mapPixelToCoords(pixelPos, view);
                    sf::Vector2f offset = worldPosBeforeZoom - worldPosAfterZoom;
                    view.move(offset);

                    window.setView(view);
                }
            }

            // Handle key pressed events
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                else if (event.key.code == sf::Keyboard::Up) {
                    view.zoom(1.0f / KEY_ZOOM_FACTOR); // Zoom in
                    window.setView(view);
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    view.zoom(KEY_ZOOM_FACTOR); // Zoom out
                    window.setView(view);
                }
                else if (event.key.code == sf::Keyboard::Left) {
                    float panAmount = view.getSize().x * PAN_SPEED_FACTOR;
					view.move(-panAmount, 0.f); // Pan Left
                    window.setView(view);
                }
                else if (event.key.code == sf::Keyboard::Right) {
                    float panAmount = view.getSize().x * PAN_SPEED_FACTOR;
                    view.move(panAmount, 0.f); // Pan Right
                    window.setView(view);
                }
                // Add W, A, S, D for panning as well
                else if (event.key.code == sf::Keyboard::W) { // Pan Up
                    float panAmount = view.getSize().y * PAN_SPEED_FACTOR;
                    view.move(0.f, -panAmount);
                    window.setView(view);
                }
                else if (event.key.code == sf::Keyboard::S) { // Pan Down
                    float panAmount = view.getSize().y * PAN_SPEED_FACTOR;
                    view.move(0.f, panAmount);
                    window.setView(view);
                }
                else if (event.key.code == sf::Keyboard::A) { // Pan Left (same as Left Arrow)
                    float panAmount = view.getSize().x * PAN_SPEED_FACTOR;
                    view.move(-panAmount, 0.f);
                    window.setView(view);
                }
                else if (event.key.code == sf::Keyboard::D) { // Pan Right (same as Right Arrow)
                    float panAmount = view.getSize().x * PAN_SPEED_FACTOR;
                    view.move(panAmount, 0.f);
                    window.setView(view);
                }
                // Reset the simulation with 'R' key
                else if (event.key.code == sf::Keyboard::R) {
                    resetSimulation(colonies, env, clock, CELL_SIZE, view, INITIAL_DEFAULT_ZOOM_OUT);
                    currentSimulationState = RUNNING;
                    std::cout << "Simulation reset.\n";
				}
            }
        }

        // --- Update Logic ---
        if (currentSimulationState == RUNNING) {
            sf::Time elapsed = clock.getElapsedTime();
            if (elapsed.asMilliseconds() > static_cast<sf::Int32>(simulationSpeed * 1000.0f)) {
                for (auto& colony : colonies) {
                    colony.update(env, colonies);
                }
                clock.restart();

                long long totalLiveAnts = 0;
                for (const auto& colony : colonies) {
                    totalLiveAnts += colony.ants.size();
                }

                if (env.totalFoodSources == 0 || totalLiveAnts == 0) {
                    currentSimulationState = WAITING_FOR_RESET;
                    resetTimerClock.restart();
                    std::cout << "Reset condition met. Restarting in " << RESET_DELAY_SECONDS << " seconds...\n";
                }
            }
        }
        else if (currentSimulationState == WAITING_FOR_RESET) {
            float timeRemaining = RESET_DELAY_SECONDS - resetTimerClock.getElapsedTime().asSeconds();
            if (timeRemaining <= 0) {
                resetSimulation(colonies, env, clock, CELL_SIZE, view, INITIAL_DEFAULT_ZOOM_OUT);
                currentSimulationState = RUNNING;
                std::cout << "Simulation restarted.\n";
            }
            resetTimerText.setString("Restarting in " + std::to_string(static_cast<int>(std::max(0.0f, timeRemaining))) + "s");
        }


        // --- Update Text ---
        long long totalLiveAnts = 0, totalPeakPopulation = 0, totalDeaths = 0;
        for (const auto& colony : colonies) {
            totalLiveAnts += colony.ants.size();
            totalPeakPopulation += colony.peakPopulation;
            totalDeaths += colony.totalAntsDied;
        }
        populationText.setString("Total Live Ants: " + std::to_string(totalLiveAnts) +
            "\nPeak Population: " + std::to_string(totalPeakPopulation));
        deathText.setString("Total Deaths: " + std::to_string(totalDeaths));
        foodText.setString("Food Sources: " + std::to_string(env.totalFoodSources));


        // --- Drawing ---
        window.clear(sf::Color::White);
        window.setView(view); // Apply the main view for simulation elements

        for (const auto& colony : colonies) {
            sf::CircleShape colonyHomeShape(CELL_SIZE * 1.5f);
            colonyHomeShape.setFillColor(colony.colonyColor);
            colonyHomeShape.setOrigin(colonyHomeShape.getRadius(), colonyHomeShape.getRadius());
            colonyHomeShape.setPosition((static_cast<float>(colony.homeX) + 0.5f) * CELL_SIZE, (static_cast<float>(colony.homeY) + 0.5f) * CELL_SIZE);
            window.draw(colonyHomeShape);
        }

        for (const auto& colony : colonies) {
            for (int i = 0; i < Environment::GRID_SIZE; i++) {
                for (int j = 0; j < Environment::GRID_SIZE; j++) {
                    float homePheromoneValue = colony.returnHomePheromones[i][j];
                    if (homePheromoneValue > 0.01f) {
                        sf::RectangleShape pheromoneShape(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                        pheromoneShape.setPosition(static_cast<float>(i * CELL_SIZE), static_cast<float>(j * CELL_SIZE));
                        sf::Color baseColor = colony.colonyColor;
                        sf::Uint8 alpha = static_cast<sf::Uint8>(std::min(255.0f, homePheromoneValue * 2.5f));
                        pheromoneShape.setFillColor(sf::Color(std::min(255, baseColor.r + 50), std::min(255, baseColor.g + 50), std::min(255, baseColor.b + 50), alpha));
                        window.draw(pheromoneShape);
                    }
                    float foodPheromoneValue = colony.foodPheromones[i][j];
                    if (foodPheromoneValue > 0.01f) {
                        sf::RectangleShape pheromoneShape(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                        pheromoneShape.setPosition(static_cast<float>(i * CELL_SIZE), static_cast<float>(j * CELL_SIZE));
                        sf::Uint8 alpha = static_cast<sf::Uint8>(std::min(255.0f, foodPheromoneValue * 4.0f));
                        pheromoneShape.setFillColor(sf::Color(255, 215, 0, alpha)); // Gold
                        window.draw(pheromoneShape);
                    }
                }
            }
        }

        for (const auto& colony : colonies) {
            for (const auto& ant : colony.ants) {
                sf::RectangleShape antShapeToDraw(*(ant.shape));
                antShapeToDraw.setPosition(static_cast<float>(ant.x * CELL_SIZE), static_cast<float>(ant.y * CELL_SIZE));
                sf::Color antColor = ant.getColonyColor();
                if (ant.hasFood) antColor = sf::Color::Green;
                if (ant.lifespan < 50) {
                    int fade = static_cast<int>(ant.lifespan * 5.1f);
                    antColor = sf::Color(std::max(0, antColor.r - (255 - fade)), std::max(0, antColor.g - (255 - fade)), std::max(0, antColor.b - (255 - fade)), static_cast<sf::Uint8>(fade));
                }
                antShapeToDraw.setFillColor(antColor);
                window.draw(antShapeToDraw);
            }
        }
        env.renderFood(window);

        // Switch to default view for UI elements
        window.setView(window.getDefaultView());
        window.draw(populationText);
        window.draw(deathText);
        window.draw(foodText);
        if (currentSimulationState == WAITING_FOR_RESET) {
            window.draw(resetTimerText);
        }
        window.display();
    }
    return 0;
}
// resetSimulation function to reset and reinitialize the simulation state
void resetSimulation(std::vector<Colony>& colonies, Environment& env, sf::Clock& gameClock, float cellSize, sf::View& view, float initialZoom) {
    colonies.clear();
    env.generateFood();
    Colony::nextColonyID = 0;

    std::uniform_int_distribution<> grid_distrib(0, Environment::GRID_SIZE - 1);

    sf::Color colony1Color = sf::Color::Black;
    sf::Color colony2Color = sf::Color::Red;
    sf::Color colony3Color = sf::Color::Blue;

    colonies.emplace_back(grid_distrib(RandomUtils::getGenerator()), grid_distrib(RandomUtils::getGenerator()), 5, cellSize, colony1Color, Colony::nextColonyID++);
    colonies.emplace_back(grid_distrib(RandomUtils::getGenerator()), grid_distrib(RandomUtils::getGenerator()), 5, cellSize, colony2Color, Colony::nextColonyID++);
    colonies.emplace_back(grid_distrib(RandomUtils::getGenerator()), grid_distrib(RandomUtils::getGenerator()), 5, cellSize, colony3Color, Colony::nextColonyID++);

    float gridWorldDimension = static_cast<float>(Environment::GRID_SIZE) * cellSize;
    view.setSize(gridWorldDimension, gridWorldDimension);
    view.setCenter(gridWorldDimension / 2.0f, gridWorldDimension / 2.0f);
    view.zoom(initialZoom);

    gameClock.restart();
    std::cout << "Simulation data reset. New colonies created. View reset.\n";
}
