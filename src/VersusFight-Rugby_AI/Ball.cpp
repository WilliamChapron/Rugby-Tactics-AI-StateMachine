#include "pch.h"

#include "Ball.hpp"
#include "Player.hpp"

Ball::Ball(const sf::Vector2f& position)
{
    sf::CircleShape* shape = new sf::CircleShape();

    mSize = 30.f;
    shape->setRadius(mSize / 2.f);
    shape->setFillColor(sf::Color::Yellow);
    shape->setOrigin(mSize / 2.f, mSize / 2.f);
    shape->setPosition(position);
    mShape = shape;
}

Ball::~Ball()
{
}

void Ball::Update()
{
    Entity::Update();
}
