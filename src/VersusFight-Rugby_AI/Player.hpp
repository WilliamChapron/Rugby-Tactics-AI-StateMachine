#pragma once

#include "Entity.hpp"
#include "Context.hpp"

class Behaviour;

class Player : public Entity
{
    Context::State mState;
    Behaviour * mBehaviour;
public:
    Player(const sf::Vector2f& position, Behaviour* behaviour);
    ~Player();

    void Update() override;

    Context::State getState() const;
    void setState(Context::State);
};
