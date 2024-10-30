#pragma once

#include "Entity.hpp"
#include <SFML/Graphics/CircleShape.hpp>

class Player;

class GoalZone : public Entity
{
public:
    GoalZone(int width, int height, const sf::Vector2f& position);
    ~GoalZone();

    void Update() override;

    void OnCollide(Entity* entity) override;
};
