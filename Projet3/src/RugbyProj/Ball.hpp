#pragma once

#include "Entity.hpp"
#include <SFML/Graphics/CircleShape.hpp>

class Player;

enum class BallState {
    Idle,    // Stationary ball
    Travel   // Ball in motion
};

class Ball : public Entity
{
public:
    Ball(const sf::Vector2f& position);
    ~Ball();
    
    BallState m_state = BallState::Idle;

    Player* mSender = nullptr;
    Player* mReceiver = nullptr;
    Player* mExpectedReceiver = nullptr;

    bool mIsBallReceived;

    void Update() override;
    void OnCollide(Entity* ply) override;
};
