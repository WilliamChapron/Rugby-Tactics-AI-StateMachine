#pragma once

#include <vector>

class Entity;
class Player;
class Behaviour;


#include "Player.hpp"


namespace sf {
    class RenderWindow;
}

class GameManager {
private:
    std::vector<Entity*> mEntities;
    std::vector<Player*> mPlayers;
    sf::RenderWindow* mWindow;
    Behaviour* mBehaviour;
    float mDeltaTime;

    Player* mHasBallPlayer;
    sf::Font mFont;


    sf::Text mScoreText;
    int mScore1 = 0;
    int mScore2 = 0;

    int mRenderWidth;
    int mRenderHeight;
public:

    Player* selectedPlayer;
    Ball* mBall;
    GameManager();
    ~GameManager();

    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

    static GameManager& GetInstance()
    {
        static GameManager instance;
        return instance;
    }
    inline Ball* getBall() { return mBall; };

    inline Player* getHasBallPlayer() { return mHasBallPlayer; };
    inline void setHasBallPlayer(Player* ply) { mHasBallPlayer = ply; };

    void Initialize();
    void InitPlayground(int playerBall);
    void resetGame(int team);

    void checkCollide();

    void addEntity(Entity* entity) {
        mEntities.push_back(entity);
    }

    void Update();
    void Draw();

    void setWindow(sf::RenderWindow* window)
    {
        mWindow = window;
        mRenderWidth = mWindow->getSize().x;
        mRenderHeight = mWindow->getSize().y;
    }

    void setDeltaTime(float deltaTime) { mDeltaTime = deltaTime; }
    float getDeltaTime() const { return mDeltaTime; }

    int getScoreTeam1() { return mScore1; }
    int getScoreTeam2() { return mScore2; }

    inline const sf::Font& getFont() const { return mFont; }
    sf::Text getScoreText() { return mScoreText; }

    std::vector<Player*> getAllPlayers();
    std::vector<Entity*> getAllEntities();

    friend Entity;
};
