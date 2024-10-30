#include "pch.h"

#include "GameManager.hpp"
#include "Behaviour.hpp"
#include "Condition.hpp"
#include "Transition.hpp"
#include "Player.hpp"
#include "Ball.hpp"
#include "Entity.hpp"
#include "PlayerActions.hpp"
#include "PlayerConditions.hpp"
#include "Team.hpp"
#include "GoalZone.hpp"

GameManager::GameManager() : mWindow(nullptr), mDeltaTime(0.f) {
    mBehaviour = new Behaviour();

    mRenderWidth = 0;
    mRenderHeight = 0;

    mHasBallPlayer = nullptr;
    selectedPlayer = nullptr;

    if (!mFont.loadFromFile("../../../res/Hack-Regular.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
    }

    mScoreText.setFont(mFont);
    mScoreText.setCharacterSize(24);
    mScoreText.setFillColor(sf::Color::Black);
    mScoreText.setPosition(500, 20);
}

void GameManager::Initialize() {
    std::cout << "Init Game Manager" << std::endl;
    
    // ACTIONS ******************

    // Actions
    Action* actionDefend = new A_Defend();
    Action* actionOffensiveBallO = new A_OffensiveBallO();
    Action* actionSupportOffensive = new A_SupportOffensive();
    Action* actionPassingBall = new A_PassingBall();
    Action* actionReceivingBall = new A_ReceivingBall();
    Action* actionWaiting = new A_Waiting();
    Action* actionKeepDistance = new A_KeepDistanceWithTeamMates();

    //
    mBehaviour->AddAction(Context::StatePlayer::Defend, actionDefend);
    mBehaviour->AddAction(Context::StatePlayer::OffensiveBallO, actionOffensiveBallO);
    mBehaviour->AddAction(Context::StatePlayer::OffensiveBallX, actionSupportOffensive);
    mBehaviour->AddAction(Context::StatePlayer::PassBall, actionPassingBall);
    mBehaviour->AddAction(Context::StatePlayer::JustReceivedBall, actionReceivingBall);
    mBehaviour->AddAction(Context::StatePlayer::Waiting, actionWaiting);

    // Keep distance with teammates
    mBehaviour->AddAction(Context::StatePlayer::OffensiveBallX, actionKeepDistance);
    mBehaviour->AddAction(Context::StatePlayer::Defend, actionKeepDistance);

    // ****************************************

    // TRANSITION CONDITIONS

    // if Enemy near, pass to PassingBall
    Transition* transition1 = new Transition();
    transition1->setTargetState(Context::StatePlayer::PassBall);
    transition1->addCondition(new C_EnemyClose);
    mBehaviour->AddTransition(Context::StatePlayer::OffensiveBallO, transition1);


    // COLLISIONS
 
    // if baller touch opponent, lost ball & pass to defend
    Transition* loseBall = new Transition();
    loseBall->setTargetState(Context::StatePlayer::Defend);
    loseBall->addCondition(new C_TouchOpponent);
    mBehaviour->AddTransition(Context::StatePlayer::PassBall, loseBall);
    mBehaviour->AddTransition(Context::StatePlayer::OffensiveBallO, loseBall);

    // if denfensor touch baller, pass to Received Ball
    Transition* defendToOffensive0 = new Transition();
    defendToOffensive0->setTargetState(Context::StatePlayer::JustReceivedBall);
    defendToOffensive0->addCondition(new C_TouchBall);
    mBehaviour->AddTransition(Context::StatePlayer::Defend, defendToOffensive0);

    //***************

    Transition* defendToJustReceived = new Transition();
    defendToJustReceived->setTargetState(Context::StatePlayer::JustReceivedBall);
    defendToJustReceived->addCondition(new C_HasBall);
    mBehaviour->AddTransition(Context::StatePlayer::Defend, defendToJustReceived);

    // if defensor is in baller team, FORCE to Offensive support (AUTO FORCE)
    Transition* defendToOffensiveX = new Transition();
    defendToOffensiveX->setTargetState(Context::StatePlayer::OffensiveBallX);
    defendToOffensiveX->addCondition(new C_TeamHasBall);
    mBehaviour->AddTransition(Context::StatePlayer::Defend, defendToOffensiveX);

    // if support is not more in baller team, FORCE to defend (AUTO FORCE)
    Transition* loseBallTeam = new Transition();
    loseBallTeam->setTargetState(Context::StatePlayer::Defend);
    loseBallTeam->addCondition(new C_TeamHasNotBall);
    mBehaviour->AddTransition(Context::StatePlayer::OffensiveBallX, loseBallTeam);

    // if baller has not more ball, FORCE to support (AUTO FORCE)
    Transition* transition2 = new Transition();
    transition2->setTargetState(Context::StatePlayer::OffensiveBallX);
    transition2->addCondition(new C_TeamHasBall);
    mBehaviour->AddTransition(Context::StatePlayer::PassBall, transition2);

    // If get ball, in support, just receive ball
    Transition* transition3 = new Transition();
    transition3->setTargetState(Context::StatePlayer::JustReceivedBall);
    transition3->addCondition(new C_HasBall);
    mBehaviour->AddTransition(Context::StatePlayer::OffensiveBallX, transition3);


    // Effects are end, pass again to Has ball 
    Transition* transition5 = new Transition();
    transition5->setTargetState(Context::StatePlayer::OffensiveBallO);
    transition5->addCondition(new C_JustReceivedBallEnd);
    mBehaviour->AddTransition(Context::StatePlayer::JustReceivedBall, transition5);


    // AUTO FORCE STATE

    // Idle start
    Transition* transition20 = new Transition();
    transition20->setTargetState(Context::StatePlayer::Defend);
    transition20->addCondition(new C_TeamHasNotBall);
    mBehaviour->AddTransition(Context::StatePlayer::Idle, transition20);

    Transition* transition21 = new Transition();
    transition21->setTargetState(Context::StatePlayer::OffensiveBallX);
    transition21->addCondition(new C_TeamHasBall);
    mBehaviour->AddTransition(Context::StatePlayer::Idle, transition21);

    Transition* transition22 = new Transition();
    transition22->setTargetState(Context::StatePlayer::OffensiveBallO);
    transition22->addCondition(new C_HasBall);
    mBehaviour->AddTransition(Context::StatePlayer::Idle, transition22);

    Transition* attakToTeammate = new Transition();
    attakToTeammate->setTargetState(Context::StatePlayer::OffensiveBallX);
    attakToTeammate->addCondition(new C_HasNotBall);
    mBehaviour->AddTransition(Context::StatePlayer::OffensiveBallO, attakToTeammate);

    Transition* waitingToTeammate = new Transition();
    waitingToTeammate->setTargetState(Context::StatePlayer::OffensiveBallX);
    waitingToTeammate->addCondition(new C_TeamHasBall);
    mBehaviour->AddTransition(Context::StatePlayer::Waiting, waitingToTeammate);

    Transition* waitingToDefend = new Transition();
    waitingToDefend->setTargetState(Context::StatePlayer::Defend);
    waitingToDefend->addCondition(new C_OppTeamHasBall);
    mBehaviour->AddTransition(Context::StatePlayer::Waiting, waitingToDefend);

    //Pass ball to OffensiveBallX
    Transition* transition24 = new Transition();
    transition24->setTargetState(Context::StatePlayer::OffensiveBallX);
    transition24->addCondition(new C_HasNotBall);
    mBehaviour->AddTransition(Context::StatePlayer::PassBall, transition24);


    // Waiting to Has ball
    Transition* transition23 = new Transition();
    transition23->setTargetState(Context::StatePlayer::JustReceivedBall);
    transition23->addCondition(new C_HasBall);
    mBehaviour->AddTransition(Context::StatePlayer::Waiting, transition23);

    // ****************************************

    int random = rand() % 5;
    InitPlayground(random);
}

void GameManager::InitPlayground(int playerBall)
{
    const int numPlayers = 5;

    sf::Vector2f leftTeamPositions[numPlayers] = { sf::Vector2f(60.f, 80.f), sf::Vector2f(170.f, 200.f), sf::Vector2f(230.f, mRenderHeight / 2), sf::Vector2f(170.f, mRenderHeight - 200.f), sf::Vector2f(60.f, mRenderHeight - 80.f) };
    sf::Vector2f rightTeamPositions[numPlayers] = { sf::Vector2f(mRenderWidth - 60.f, 80.f), sf::Vector2f(mRenderWidth - 170.f, 200.f), sf::Vector2f(mRenderWidth - 230.f, mRenderHeight / 2), sf::Vector2f(mRenderWidth - 170.f, mRenderHeight - 200.f), sf::Vector2f(mRenderWidth - 60.f, mRenderHeight - 80.f) };

    mBall = new Ball(sf::Vector2f(mRenderWidth / 2.0f, mRenderHeight / 2.0f));
    mEntities.push_back(mBall);

    // Team left - 0
    Team* firstTeam = new Team(1);
    for (int i = 0; i < numPlayers; ++i) 
    {
        Player* leftPlayer = new Player(leftTeamPositions[i], mBehaviour, i+1);

        mPlayers.push_back(leftPlayer);
        mEntities.push_back(leftPlayer);

        // Team syst
        firstTeam->AddPlayer(leftPlayer);
        leftPlayer->setTeam(firstTeam);

        // Bounding area
        if (i < 2) 
        {
            leftPlayer->setBoundingZone(0, 0, mRenderWidth, mRenderHeight / 2/* - 100*/);
        }
        else if (i == 2) 
        {
            leftPlayer->setBoundingZone(0, mRenderHeight / 4, mRenderWidth, mRenderHeight / 2);
        }
        else if (i > 2) 
        {
            leftPlayer->setBoundingZone(0, mRenderHeight - mRenderHeight / 2/* + 100*/, mRenderWidth, mRenderHeight / 2);
        }
    }

    // Team right - 1
    Team* secondTeam = new Team(-1);
    for (int i = 0; i < numPlayers; ++i) 
    {
        Player* rightPlayer = new Player(rightTeamPositions[i], mBehaviour, i+1);

        mPlayers.push_back(rightPlayer);
        mEntities.push_back(rightPlayer);

        if (i < 2) 
        {
            rightPlayer->setBoundingZone(0, 0, mRenderWidth, mRenderHeight / 2);
        }
        else if (i == 2) 
        {
            rightPlayer->setBoundingZone(0, mRenderHeight / 4, mRenderWidth, mRenderHeight / 2);
        }
        else if (i > 2) 
        {
            rightPlayer->setBoundingZone(0, mRenderHeight - mRenderHeight / 2, mRenderWidth, mRenderHeight / 2);
        }

        // Team syst
        secondTeam->AddPlayer(rightPlayer);
        rightPlayer->setTeam(secondTeam);
    }

    if (playerBall < 5)
    {
        playerBall += 5;
    }
    else
    {
        playerBall -= 5;
    }

    mPlayers[playerBall]->setBall(mBall);
    mHasBallPlayer = mPlayers[playerBall];

    //Define the scoring area
    GoalZone* goalZone1 = new GoalZone(100, mRenderHeight, sf::Vector2f(0, 0));
    GoalZone* goalZone2 = new GoalZone(100, mRenderHeight, sf::Vector2f(mRenderWidth - 100, 0));
    mEntities.push_back(goalZone1);
    mEntities.push_back(goalZone2);;
}

void GameManager::resetGame(int team)
{
    for (auto& entity : mEntities)
    {
        delete entity;
    }
    mEntities.clear();
    mPlayers.clear();
    
    int random;
    if (team == 0)
    {
        random = rand() % 5 + 5;
        mScore1++;
    }
    else
    {
        random = rand() % 5;
        mScore2++;
    }

    InitPlayground(random);

    selectedPlayer = nullptr;
}

void GameManager::checkCollide()
{
    for (auto& entity : mEntities)
    {
        for (auto& ply : mPlayers)
        {
            float dx = entity->getPosition().x - ply->getPosition().x;
            float dy = entity->getPosition().y - ply->getPosition().y;
            float dist = sqrt(dx * dx + dy * dy);

            if (entity != ply &&
                entity->getShape().getGlobalBounds().intersects(ply->getShape().getGlobalBounds()))
            {
                entity->OnCollide(ply);
            }
        }
    }

}


GameManager::~GameManager() 
{
    for (Entity* entity : mEntities) 
    {
        delete entity;
    }
}

void GameManager::Update() 
{
    mScoreText.setString("Score: " + std::to_string(mScore2) + " - " + std::to_string(mScore1));

    for (Entity* entity : mEntities) 
    {
        entity->Update();
    }

    // Update player UI depends on player baller can make pass
    GameManager& gameManager = GameManager::GetInstance();
    std::vector<Player*> allPlayers = getAllPlayers();
    Player* baller = gameManager.getHasBallPlayer();

    for (Player* ply : allPlayers)
    {
        ply->mIsBeAbleToCatchBall = false;
        if (ply != nullptr && ply != baller && ply->isPlayerSameTeam(baller) && ply->isPlayerBehind(baller, 10)) 
        {
            ply->mIsBeAbleToCatchBall = true;
        }
    }

    if (mHasBallPlayer != nullptr && mBall != nullptr) 
    {
        if (mBall->m_state == BallState::Idle) 
        {
            mBall->setPosition(mHasBallPlayer->getPosition());
        }
    }

    // Behaviour
    for (Player* player : mPlayers) 
    {
        mBehaviour->Update(player);
    }

    checkCollide();
}

void GameManager::Draw() 
{
    //lower the priority is, faster will be drawing the entity
    std::sort(mEntities.begin(), mEntities.end(), [](Entity* a, Entity* b)
        {
            return a->getPriority() < b->getPriority();
        });


    for (Entity* entity : mEntities) 
    {
        mWindow->draw(entity->getShape());
    }

    // Draw player UI
    for (Player* ply : mPlayers) 
    {
        GameManager& gameManager = GameManager::GetInstance();
        mWindow->draw(ply->mHud);
    }
    
    mWindow->draw(mScoreText);
}

std::vector<Player*> GameManager::getAllPlayers()
{
    return mPlayers;
}

std::vector<Entity*> GameManager::getAllEntities()
{
    return mEntities;
}
