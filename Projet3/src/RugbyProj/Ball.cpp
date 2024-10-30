#include "pch.h"
#include "Ball.hpp"
#include "Player.hpp"
#include "GameManager.hpp"
#include "Entity.hpp"

Ball::Ball(const sf::Vector2f& position)
{
    mSpeed = 200.f;

    // Create the shape of the ball
    sf::CircleShape* shape = new sf::CircleShape();

    mSize = 26.f;
    shape->setRadius(mSize / 2.f);
    sf::Color brown(200, 119, 109);
    shape->setFillColor(brown);
    shape->setOrigin(mSize / 2.f, mSize / 2.f);
    shape->setPosition(position);
    mShape = shape;
 
    mDrawPriority = 3;

    // Init var
    mSender = nullptr;
    mReceiver = nullptr;
    mExpectedReceiver = nullptr;

    mIsBallReceived = false;
}

Ball::~Ball()
{
}

void Ball::Update()
{
    Entity::Update();
}

void Ball::OnCollide(Entity* ent)
{
    GameManager& gameManager = GameManager::GetInstance();
    Ball* ball = gameManager.getHasBallPlayer()->getBall();

    if (ball != nullptr)
    {
        if (m_state == BallState::Travel && ent != ball->mSender) 
        {
            ball->mReceiver = static_cast<Player*>(ent);

            // Set the new receiver as the player with the ball
            ball->mReceiver->setBall(ball);
            gameManager.setHasBallPlayer(ball->mReceiver);
            ball->m_state = BallState::Idle;

            // Remove the ball from the old sender
            if (mSender != nullptr)
            {
                ball->mSender->removeBall();
            }
        }
    }
}
