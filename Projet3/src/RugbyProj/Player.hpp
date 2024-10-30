#pragma once

#include "Entity.hpp"
#include "Context.hpp"
#include "Utils.hpp"
#include "Team.hpp"

enum class Effect {
    Invincible,
    CannotPass,
    SpeedBoost,
};


class Behaviour;
class Ball;
class Team;

class Player : public Entity
{
private:
    Context::StatePlayer mStatePlayer;
    Behaviour* mBehaviour;
    Team* mTeam;
    Ball* mBall;

    // Ui info

    int mId;

    //

    // Avoid exit bounding zone
    Utils::BoundingRect* mBorderArea = nullptr;

public:
    Player(const sf::Vector2f& position, Behaviour* behaviour, int id);
    ~Player();

    void Update() override;

    // UI
    sf::Text mHud;
    bool mIsBeAbleToCatchBall = false;
    //inline const sf::Text& getHud() const { return mHud; }


    void OnCollide(Entity* ply) override;

    inline void setTeam(Team* team) {mTeam = team;}
    inline Team* getTeam() { return mTeam; }

    inline Behaviour* getBehaviour() { return mBehaviour; }

    // Bounding Zone
    inline void setBoundingZone(float x, float y, float width, float height) { mBorderArea = new Utils::BoundingRect(x, y, width, height);}
    inline Utils::BoundingRect* getBoundingZone() { return mBorderArea; }

    void AvoidExitBoundingRect();

    // State
    inline Context::StatePlayer getPlayerState() { return mStatePlayer; }
    void setStatePlayer(Context::StatePlayer);
    // Ball
    Ball* getBall();
    void setBall(Ball* new_ball);
    void removeBall() {mBall = nullptr;}

    // Actions logic
    bool isPlayerInFront(Player* otherPlayer, int gabMore = 0);
    bool isPlayerBehind(Player* otherPlayer, int gabMore = 0);

    bool isPlayerSameTeam(Player* otherPlayer);

    // TRANSITION / ACTIONS VAR
    // Zone area Exceeded block vars, out of own area bound
    bool mHasExceededTop = false;
    bool mHasExceededBottom = false;

    // Proximity check block vars, player is too close other team mate 
    bool tooCloseTop = false;
    bool tooCloseBottom = false;
    Utils::Timer mProximityCheckT;

    // Try pass ball
    Utils::Timer mPassBallT;

    // Just received ball
    std::vector<Effect> effects;
    Utils::Timer mReceivedBallT;
    //*********************************************

};


