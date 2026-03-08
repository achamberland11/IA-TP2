//
// Created by Antoine on 2026-03-05.
//

#include <cstdlib>
#include <ctime>

#include "Game/Map/Map.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Network/SocketHandle.hpp"
#include "SFML/System/Vector2.hpp"

int main() {
    srand(time(nullptr));

    try {
        sf::Vector2u windowSize(1280, 736);
        sf::RenderWindow window(sf::VideoMode(windowSize), "TP AI");
        window.setFramerateLimit(120);

        GMap Map(windowSize.x, windowSize.y);

        Map.LoadMap("Assets/map.csv");

        while (window.isOpen()) {
            // Draw
            window.clear(sf::Color::Blue);

            Map.Display(window);

            window.display();
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
