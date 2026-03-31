//
// Created by Antoine on 2026-03-05.
//

#include <cstdlib>
#include <ctime>

#include "Game/Game.h"
#include "Game/Map/Map.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Network/SocketHandle.hpp"
#include "SFML/System/Vector2.hpp"

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    try
    {
        GGame Game(sf::Vector2u(1568, 928));
        Game.Run();
    } catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
