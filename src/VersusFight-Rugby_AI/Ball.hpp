#pragma once

#include "Entity.hpp"
#include <SFML/Graphics/CircleShape.hpp>

class Player;

class Ball : public Entity
{
public:
    Ball(const sf::Vector2f& position);
    ~Ball();

    void Update() override;
};
