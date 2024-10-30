#pragma once

#include "Condition.hpp"
#include "Player.hpp"

#include "GameManager.hpp"
#include "Ball.hpp"


class C_EnemyClose : public Condition {
public:
    bool Test(Player* ply) override {
        bool hasInvincible = std::find(ply->effects.begin(), ply->effects.end(), Effect::Invincible) != ply->effects.end();
        if (hasInvincible) {
            return false;
        }

        GameManager& gameManager = GameManager::GetInstance();
        float thresholdDistance = 200.0f;

        int plyTeamDir = ply->getTeam()->getDirection();

        for (Player* opponent : gameManager.getAllPlayers()) {
            if (opponent->getTeam() != ply->getTeam()) { 


                if (plyTeamDir == -1) {
                    if (opponent->getPosition().x < ply->getPosition().x &&
                        ply->getPosition().x - opponent->getPosition().x <= thresholdDistance) {
                        //std::cout << "Enemy proche" << "\n";
                        return true;
                    }
                }
                else if (plyTeamDir == 1) {
                    if (opponent->getPosition().x > ply->getPosition().x &&
                        opponent->getPosition().x - ply->getPosition().x <= thresholdDistance) {
                        //std::cout << "Enemy proche" << "\n";
                        return true;
                    }
                }

            }
        }
        return false; 
    }
};


class C_HasBall : public Condition
{
        bool Test(Player* ply) override
        {
            // Ball is travelling
            if (ply->getBall() && ply->getBall()->m_state == BallState::Travel) {
                return false;
            }
            if (ply->getBall() != nullptr)
            {
                return true;
            }
            return false;
        }
};
class C_HasNotBall : public Condition
{
    bool Test(Player* ply) override
    {
        GameManager& gameManager = GameManager::GetInstance();
        // Ball is travelling
        if (ply != gameManager.getHasBallPlayer())
        {
            return true;
        }
        return false;
    }
};


class C_TeamHasBall : public Condition
{
    bool Test(Player* ply) override
    {
        
        GameManager& gameManager = GameManager::GetInstance();
        Player* hasBallPlayer = gameManager.getHasBallPlayer();

        // Ball is travelling
        if (gameManager.getBall()->m_state == BallState::Travel) {
            return false;
        }

        if (ply != hasBallPlayer && ply->getTeam() == hasBallPlayer->getTeam())
        {
            return true;
        }
        return false;
    }
};
class C_TeamHasNotBall : public Condition
{
    bool Test(Player* ply) override
    {
        GameManager& gameManager = GameManager::GetInstance();
        Player* hasBallPlayer = gameManager.getHasBallPlayer();

        // Ball is travelling
        if (hasBallPlayer != nullptr && gameManager.getBall() != nullptr)
        {
            if (gameManager.getBall()->m_state == BallState::Travel) {
                return false;
            }
        }

        if (ply != hasBallPlayer &&
            ply->getTeam() != hasBallPlayer->getTeam())
        {
            return true;
        }
        return false;
    }
};
class C_OppTeamHasBall : public Condition
{
    bool Test(Player* ply) override
    {
        GameManager& gameManager = GameManager::GetInstance();
        Player* hasBallPlayer = gameManager.getHasBallPlayer();

        // Ball is travelling
        if (hasBallPlayer != nullptr && gameManager.getBall() != nullptr)
        {
            if (gameManager.getBall()->m_state == BallState::Travel) {
                return false;
            }
        }

        if ( ply->getTeam() != hasBallPlayer->getTeam())
        {
            return true;
        }
        return false;
    }
};


class C_IsBehind : public Condition
{
    bool Test(Player* player) override
    {
        GameManager& gameManager = GameManager::GetInstance();
        Player* hasBallPlayer = gameManager.getHasBallPlayer();

        if (hasBallPlayer != nullptr)
        {
            if (player->getTeam()->getDirection() != hasBallPlayer->getTeam()->getDirection())
            {
                return false;
            }

            //check la position par rapport à celui qui à la balle peu importe le coté (1 ou -1)
            if (player->getTeam()->getDirection() * player->getPosition().x < player->getTeam()->getDirection() * hasBallPlayer->getPosition().x)
            {
                return true;
            }
        }
        return false;
    }
};
class C_IsEnemyInFront : public Condition 
{
    bool Test(Player* player) override 
    {
        GameManager& gm = GameManager::GetInstance();
        Player* hasBallPlayer = gm.getHasBallPlayer();

        if (!hasBallPlayer) return false; 

        sf::Vector2f playerPos = hasBallPlayer->getPosition();
        //int direction = hasBallPlayer->getTeamDirection();
        float detectionDistance = 100.0f;

        for (Player* otherPlayer : gm.getAllPlayers()) 
        {
            if (otherPlayer != hasBallPlayer && otherPlayer->getPlayerState() == Context::StatePlayer::Defend)
            {
                sf::Vector2f otherPos = otherPlayer->getPosition();
                sf::Vector2f toEnemy = otherPos - playerPos;
                float distance = std::sqrt(toEnemy.x * toEnemy.x + toEnemy.y * toEnemy.y);

                if (distance <= detectionDistance /*&&
                    (toEnemy.x * direction.x + toEnemy.y * direction.y) > 0*/)
                {
                    return true;
                }
            }
        }
        return false;
    }
};


class C_JustReceivedBallEnd : public Condition
{
    bool Test(Player* ply) override
    {
        if (ply->mReceivedBallT.CheckEndTimer(1)) { 
            GameManager& gameManager = GameManager::GetInstance();
            Player* hasBallPlayer = gameManager.getHasBallPlayer();
            gameManager.getBall()->mIsBallReceived = false;
            gameManager.getBall()->mSender = nullptr;
            gameManager.getBall()->mReceiver = nullptr;
            gameManager.getBall()->mExpectedReceiver = nullptr;
            ply->effects.clear();
            ply->setSpeed(90.f);
            return true;
        }
        return false;
    }
};



class C_TouchBall : public Condition 
{
    bool Test(Player* ply) override
    {
        GameManager& gm = GameManager::GetInstance();
        Player* hasBallPlayer = gm.getHasBallPlayer();

        if (gm.getBall()->m_state != BallState::Travel)
        {
            if (ply->getTeam() != hasBallPlayer->getTeam() &&
                hasBallPlayer->getPlayerState() != Context::StatePlayer::JustReceivedBall)
            {
                if (ply->getShape().getGlobalBounds().intersects(hasBallPlayer->getShape().getGlobalBounds()))
                {
                    if (rand() % 3 != 0)
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }
};
class C_TouchOpponent : public Condition
{
    bool Test(Player* ply) override
    {
        GameManager& gm = GameManager::GetInstance();

        if (gm.getBall()->m_state != BallState::Travel)
        {
            for (auto& plys : gm.getAllPlayers())
            {
                if (ply->getTeam() != plys->getTeam())
                {
                    if (plys->getShape().getGlobalBounds().intersects(ply->getShape().getGlobalBounds()))
                    {
                        return true;
                    }
                }

            }
        }
        return false;
    }
};