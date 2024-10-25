#include "pch.h"

#include "GameManager.hpp"

int main(void)
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "GamingCampus - Rugby - IA/StateMachines");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("./Hack-Regular.ttf"))
    {
        std::cerr << "Failed to load font" << std::endl;
        return -1;
    }

	GameManager * game_manager = GameManager::Instantiate();
    game_manager->setWindow(&window);
    game_manager->setDeltaTime(0.f);

    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Time dt = clock.restart();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || 
                (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape))
            {
                window.close();
            }
        }

        window.clear();
        window.clear(sf::Color(51, 153, 102));

        game_manager->Update();
        game_manager->Draw();

        window.display();

        game_manager->setDeltaTime(dt.asSeconds());
    }
}
