#include "pch.h"

#include <iostream>

#include "GameManager.hpp"
#include "Player.hpp"
#include "Ball.hpp"
#include "Behaviour.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

void drawLegend(sf::RenderWindow& window, sf::Font font) {
    // Position de départ pour la légende
    sf::Vector2f startPosition(window.getSize().x - 200, 20);
    float offsetY = 25.0f;

    // Tableau des noms d'état et des couleurs associées
    std::vector<std::pair<std::string, sf::Color>> legend = {
        {"Idle", sf::Color::White},
        {"OffensiveBallO", sf::Color::Red},
        {"OffensiveBallX", sf::Color::Yellow},
        {"Defend", sf::Color::Blue},
        {"JustReceivedBall", sf::Color::Green},
        {"PassBall", sf::Color::Magenta},
        {"Waiting", sf::Color::Cyan}
    };

    for (std::size_t i = 0; i < legend.size(); ++i) {
        // Création du rectangle de couleur pour chaque état
        sf::RectangleShape colorBox(sf::Vector2f(15.0f, 15.0f));
        colorBox.setFillColor(legend[i].second);
        colorBox.setPosition(startPosition.x, startPosition.y + i * offsetY);

        // Création du texte pour chaque état
        sf::Text labelText;
        labelText.setFont(font);
        labelText.setString(legend[i].first);
        labelText.setCharacterSize(12);
        labelText.setFillColor(sf::Color::Black);  // Couleur du texte
        labelText.setPosition(startPosition.x + 25.0f, startPosition.y + i * offsetY - 2.0f);

        // Affichage du rectangle et du texte
        window.draw(colorBox);
        window.draw(labelText);
    }
}

int main(void)
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "GamingCampus - Rugby - IA/StateMachines");
    window.setFramerateLimit(60);

    sf::Vector2f forcedVect = sf::Vector2f(0.f,0.f);
	GameManager& gameManagerI = GameManager::GetInstance();
    gameManagerI.setWindow(&window);
    gameManagerI.setDeltaTime(0.f);

    gameManagerI.Initialize();

    // Redirect output on console for cout
    #ifdef _WIN32
        AllocConsole();
        FILE* stream;
        freopen_s(&stream, "CONOUT$", "w", stdout);
    #endif

    sf::Clock clock;
    bool isPaused = false;  
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
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
            {
                isPaused = !isPaused;
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                float x = event.mouseButton.x;
                float y = event.mouseButton.y;
                for (auto& ply : gameManagerI.getAllPlayers())
                {
                    if (ply->getShape().getGlobalBounds().contains(sf::Vector2f(x, y)))
                    {
                        gameManagerI.selectedPlayer = ply;
                        std::cout << "player selectionne" << std::endl;
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P && 
                gameManagerI.selectedPlayer != nullptr)
            {
                //ForcePass
                std::cout << "Forcer la passe" << std::endl;

                Player* selectedPlayer = gameManagerI.selectedPlayer;
                GameManager& gameManager = GameManager::GetInstance();
                Player* currentBaller = gameManager.getHasBallPlayer(); // Joueur ayant la balle

                // Vérifier que le joueur sélectionné est éligible pour recevoir la passe
                if (currentBaller != nullptr && currentBaller != selectedPlayer)
                {
                    // Effectuer la passe vers le joueur sélectionné
                    Ball* ball = currentBaller->getBall();
                    if (ball != nullptr)
                    {
                        // Position cible de la balle (vers le joueur sélectionné)
                        ball->goToPosition(selectedPlayer->getPosition());
                        ball->m_state = BallState::Travel;

                        // Mettre à jour l'état de passe
                        ball->mSender = currentBaller;
                        ball->mExpectedReceiver = selectedPlayer;

                        // Mettre à jour les informations des joueurs
                        selectedPlayer->setStatePlayer(Context::StatePlayer::Waiting); // Mettre le joueur sélectionné en attente
                    }
                }
            }

            //**********************************************************

            if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z &&
                gameManagerI.selectedPlayer != nullptr))
            {
                forcedVect = sf::Vector2f(0.f, -5.f);
            }
            if ((event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Z &&
                gameManagerI.selectedPlayer != nullptr))
            {
                forcedVect = sf::Vector2f(0.f, 0.f);
            }

            if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q &&
                gameManagerI.selectedPlayer != nullptr))
            {
                forcedVect = sf::Vector2f(-5.f, 0.f);
            }
            if ((event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Q &&
                gameManagerI.selectedPlayer != nullptr))
            {
                forcedVect = sf::Vector2f(0.f, 0.f);
            }

            if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S &&
                gameManagerI.selectedPlayer != nullptr))
            {
                forcedVect = sf::Vector2f(0.f, 5.f);
            }
            if ((event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::S &&
                gameManagerI.selectedPlayer != nullptr))
            {
                forcedVect = sf::Vector2f(0.f, 0.f);
            }

            if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D &&
                gameManagerI.selectedPlayer != nullptr))
            {
                forcedVect = sf::Vector2f(5.f, 0.f);
            }
            if ((event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::D &&
                gameManagerI.selectedPlayer != nullptr))
            {
                forcedVect = sf::Vector2f(0.f, 0.f);
            }
        }

        if (!isPaused)
        {
            if (gameManagerI.selectedPlayer != nullptr)
            {
                gameManagerI.selectedPlayer->setPosition(gameManagerI.selectedPlayer->getPosition() + forcedVect);
            }

            window.clear();
            window.clear(sf::Color(51, 153, 102));

            gameManagerI.Update();
            gameManagerI.Draw();


            drawLegend(window, gameManagerI.getFont());

            window.display();



            gameManagerI.setDeltaTime(dt.asSeconds());
        }

        
    }

    return 0;
}
