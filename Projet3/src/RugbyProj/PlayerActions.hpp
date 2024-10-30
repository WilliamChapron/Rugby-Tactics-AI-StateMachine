#pragma once

#include "./Player.hpp"

#include "./Action.hpp"

#include "./PlayerActions.hpp"

#include "./GameManager.hpp"

#include "./Utils.hpp"

#include "./Team.hpp"

// BASIC BEHAVIOUR
class A_OffensiveBallO : public Action {
public:

    void Start(Player* ply) override {
    }
    void Update(Player* ply) override {
        sf::Vector2f direction = sf::Vector2f(ply->getTeam()->getDirection(), 0);
        ply->setDirection(direction);
    }
    void End(Player* ply) override {
    }
};

class A_SupportOffensive : public Action {
public:

    void Start(Player* ply) override {
    }
    void Update(Player* ply) override {

        GameManager& gameManager = GameManager::GetInstance();
        int nbBehind = 0;
        for (auto& players : gameManager.getAllPlayers())
        {
            if (players->getTeam() == ply->getTeam() &&
                players != gameManager.getHasBallPlayer())
            {
                if (abs(gameManager.getHasBallPlayer()->getPosition().y - ply->getPosition().y) > abs(gameManager.getHasBallPlayer()->getPosition().y - players->getPosition().y))
                {
                    nbBehind++;
                }
            }
        }

        // Allow support baller if, your are behind
        if (ply->isPlayerBehind(gameManager.getHasBallPlayer(),50+nbBehind*50)) {
            ply->setSpeed(90);
            sf::Vector2f direction = sf::Vector2f(ply->getTeam()->getDirection(), 0);
            ply->setDirection(direction);
        }

        if (ply->isPlayerInFront(gameManager.getHasBallPlayer())) {
            sf::Vector2f direction = sf::Vector2f(ply->getTeam()->getDirection() * -1, 0);
            ply->setSpeed(30);
            ply->setDirection(direction);
        }

    }
    void End(Player* ply) override {
        //ply->removeBall();
    }
private:
};

class A_Defend : public Action {
public:

    void Start(Player* ply) override {
    }
    void Update(Player* ply) override {
        // Chase Baller Oponent

         // Follow player has ball
        GameManager& gmInstance = GameManager::GetInstance();
        Player* targetEnemy = gmInstance.getHasBallPlayer();

        MoveToPlayerHasBall(ply, targetEnemy);
    }
    void End(Player* ply) override {
    }
private:
    void MoveToPlayerHasBall(Player* ply, Player* targetEnemy) {
 
        sf::Vector2f enemyPos = targetEnemy->getPosition();
        sf::Vector2f playerPos = ply->getPosition();
        // Direction
        sf::Vector2f direction = enemyPos - playerPos;
        // Check if blocked var, if case, block top or bottom direction 
        if (ply->tooCloseTop && direction.y > 0) {
            direction.y = 0;
        }
        if (ply->tooCloseBottom && direction.y < 0) {
            direction.y = 0;
        }
        // Normalize direction - #WARNING Normalize after to not affect distance for no move in y
        direction = Utils::Vector2Normalize(direction);
        ply->setDirection(direction);
    }
};


// ========== PASSING BALL ==========
class A_PassingBall : public Action {
public:
    void Start(Player* ply) override { 
        //std::cout << "Entre pass ball" << std::endl; 
    }

    void Update(Player* ply) override {
        // if ball already travelling or forced
        GameManager& gameManager = GameManager::GetInstance();
        Ball* ball = gameManager.getBall();
        if (ball->m_state == BallState::Travel) {
            //std::cout << "Ball Alrady travelling" << "\n";
            return;
        }

        if (!CheckTimer(ply)) return;
        if (!TryRandomPass())
        {
            //std::cout << "passe loupe" << std::endl;
            return;
        }

        if (Player* teamMate = FindReceiver(ply)) {
            ThrowBall(ply, teamMate);
        }
    }

    void End(Player* ply) override { 
        ply->removeBall();
    }

private:
    bool CheckTimer(Player* ply) {
        if (!ply->mPassBallT.CheckEndTimer(1)) {
            return false;
        }
        ply->mPassBallT.Start();
        return true;
    }

    bool TryRandomPass() const {
        return std::rand() % 3 != 0;
    }

    Player* FindReceiver(Player* ply) const {
        GameManager& gameManager = GameManager::GetInstance();

        Player* teamMate = nullptr;
        float dist = 10000;
        for (auto& players : gameManager.getAllPlayers())
        {
            if (players->getTeam() == ply->getTeam() &&
                players != ply)
            {
                int newValue = abs(ply->getPosition().y - players->getPosition().y);
                if (dist > newValue)
                {
                    dist = newValue;
                    teamMate = players;
                }
            }
        }

        if (teamMate != nullptr && ply->isPlayerSameTeam(teamMate) && ply->isPlayerInFront(teamMate, 10)) 
        {
            return teamMate;
        }

        return nullptr;
    }

    void ThrowBall(Player* ply, Player* teamMate) {

        GameManager& gameManager = GameManager::GetInstance();

        // Throw ball
        Ball* ball = gameManager.getHasBallPlayer()->getBall();
        ball->goToPosition(teamMate->getPosition());
        ball->m_state = BallState::Travel;


        // Wait
        ply->clearDirection();
        teamMate->setStatePlayer(Context::StatePlayer::Waiting);

        ball->mSender = ply;
        ball->mExpectedReceiver = teamMate;
    }
};

class A_ReceivingBall : public Action {
public:
    void Start(Player* ply) override {
        GameManager& gameManager = GameManager::GetInstance();
        Ball* ball = gameManager.getBall();

        // Force if collide, else it's manage in collide ball
        gameManager.setHasBallPlayer(ply);
        ply->setBall(ball);

        if (ball->mIsBallReceived) {
            return;
        }

        if (ball->mExpectedReceiver && ball->mExpectedReceiver != ply) {
            //std::cout << "Un joueur adverse a intercepté la balle !\n";
        }

        ply->mReceivedBallT.Start();

        ApplyReceiverEffects(ply);
        ball->mIsBallReceived = true;

    }

    void Update(Player* ply) override {
        ply->setSpeed(150.f);
        sf::Vector2f direction = sf::Vector2f(ply->getTeam()->getDirection(), 0);
        ply->setDirection(direction);
    }

    void End(Player* ply) override { }

private:
    void ApplyReceiverEffects(Player* receiver) {
        receiver->effects.push_back(Effect::Invincible);
        receiver->effects.push_back(Effect::CannotPass);
        receiver->effects.push_back(Effect::SpeedBoost);
    }
};


// ========== CHECK BOUND ==========
class A_KeepDistanceWithTeamMates : public Action {
public:

    void Start(Player* ply) override {
        ply->mProximityCheckT.Start();
    }

    void Update(Player* ply) override {
        // Proximity Check 
        if (!ply->mProximityCheckT.CheckEndTimer(0.3)) {
            return;
        }

        ply->tooCloseTop = false;
        ply->tooCloseBottom = false;


        GameManager& gmInstance = GameManager::GetInstance();
        std::vector<Player*> players = gmInstance.getAllPlayers();
        for (int i = 0; i < players.size(); ++i) {
            if (players[i] == ply) {
                continue;
            }
            // Is its team?
            if (ply->getTeam()->getDirection() == players[i]->getTeam()->getDirection()) {
                float curY = ply->getPosition().y;
                float otherY = players[i]->getPosition().y;
                float distanceY = abs(curY - otherY);
                if (distanceY <= 80.0f) {
                    if (curY > otherY) {
                        // Bloque le déplacement vers le bas
                        ply->tooCloseBottom = true;
                    }
                    if (curY < otherY) {
                        // Bloque le déplacement vers le haut
                        ply->tooCloseTop = true;
                    }
                }
            }
        }

        // Reset check timer
        ply->mProximityCheckT.Start();
        //*


    }
    void End(Player* ply) override {
    }
private:

};

// ========== OTHERS ==========
class A_Waiting : public Action {
public:

    void Start(Player* ply) override {
    }

    void Update(Player* ply) override {
        ply->clearDirection();

    }
    void End(Player* ply) override {
    }
private:

};

