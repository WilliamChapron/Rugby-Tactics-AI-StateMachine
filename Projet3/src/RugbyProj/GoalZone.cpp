#include "pch.h"
#include "GoalZone.hpp"
#include "Player.hpp"
#include "Entity.hpp"
#include "GameManager.hpp"

GoalZone::GoalZone(int width, int height, const sf::Vector2f& position)
{
    mSpeed = 1.f;

    mShape = new sf::RectangleShape(sf::Vector2f(static_cast<float>(width), static_cast<float>(height)));
    mShape->setPosition(position);
    sf::Color lightBrown(210, 180, 140);
    mShape->setFillColor(lightBrown);

    mDrawPriority = 1;
}

GoalZone::~GoalZone()
{
}

void GoalZone::Update()
{
    Entity::Update();
}

void GoalZone::OnCollide(Entity* entity)
{
    GameManager& gameManager = GameManager::GetInstance();
    if ((Player*)entity == gameManager.getHasBallPlayer())
    {
        if (gameManager.getHasBallPlayer()->getTeam()->getDirection() == 1)
        {
            gameManager.resetGame(1);
        }
        else
        {
            gameManager.resetGame(0);
        }
    }
}
