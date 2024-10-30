#include "pch.h"

#include "Player.hpp"

#include "Team.hpp"
#include "Behaviour.hpp"
#include "GameManager.hpp"

#include "GameManager.hpp"
#include "Ball.hpp"


Player::Player(const sf::Vector2f& position, Behaviour* behaviour, int id) : mStatePlayer(Context::StatePlayer::Idle)
{
    mSpeed = 90.f;

	mBall = nullptr;

    mBehaviour = behaviour;
	mSize = 50.f;

    sf::CircleShape* shape = new sf::CircleShape(10);
	shape->setRadius(mSize / 2.f);
	shape->setOrigin(mSize / 2.f, mSize / 2.f);
	shape->setPosition(position);

    mShape = shape;

	mDrawPriority = 2;

    // UI
    mId = id;

    GameManager& gameManager = GameManager::GetInstance();
    mHud.setFont(gameManager.getFont());
    mHud.setString("Joueur " + std::to_string(mId));
    mHud.setCharacterSize(20);
    mHud.setFillColor(sf::Color::Black);
    mHud.setPosition(position);
}

Player::~Player()
{
}

void Player::Update()
{
    // Player move
    AvoidExitBoundingRect();

    //Entity::Update();

    if (mHasExceededTop && mDirection.y < 0) {
        if (getBall() == nullptr) {
            std::cout << "top" << "\n";
        }

        mDirection.y = 0;
    }
    if (mHasExceededBottom && mDirection.y > 0) {
        if (getBall() == nullptr) {
            std::cout << "btm" << "\n";
        }
        mDirection.y = 0;
    }
    float dt = GameManager::GetInstance().getDeltaTime();

    sf::Vector2f newPosition;
    newPosition.x += dt * mSpeed * mDirection.x;
    newPosition.y += dt * mSpeed * mDirection.y;
    mShape->move(newPosition);


    sf::Color color;


    switch (mStatePlayer) {
    case Context::StatePlayer::Idle:
        color = sf::Color::White;
        break;
    case Context::StatePlayer::OffensiveBallO:
        color = sf::Color::Red;
        break;
    case Context::StatePlayer::OffensiveBallX:
        color = sf::Color::Yellow;
        break;
    case Context::StatePlayer::Defend:
        color = sf::Color::Blue;
        break;
    case Context::StatePlayer::JustReceivedBall:
        color = sf::Color::Green;
        break;
    case Context::StatePlayer::PassBall:
        color = sf::Color::Magenta;
        break;
    case Context::StatePlayer::Waiting:
        color = sf::Color::Cyan;
        break;
    default:
        color = sf::Color::Black;
        break;
    }

    setShapeColor(color);

    // Update player UI
    mHud.setPosition(mShape->getPosition());
    mHud.setString("Joueur " + std::to_string(mId) + (mIsBeAbleToCatchBall ? "(ON)" : ""));
}


void Player::OnCollide(Entity* ply)
{
    float posX = ply->getPosition().x;
    float posY = ply->getPosition().y;

    sf::Vector2f oppDir(getPosition().x - posX, getPosition().y - posY);
    setPosition(getPosition() + Utils::Vector2Normalize(oppDir)* 2.f + sf::Vector2f(0.f, -0.1f));
    if (getPlayerState() == Context::StatePlayer::JustReceivedBall)
    {
        setPosition(getPosition() + Utils::Vector2Normalize(oppDir) * 1.5f + sf::Vector2f(0.f,-0.1f));
    }
}

void Player::AvoidExitBoundingRect() {
    // Avoid Zone Exit
    if (getBoundingZone() == nullptr) {
        return;
    }

    mHasExceededTop = false;
    mHasExceededBottom = false;

    if (getBoundingZone()->exceedsBottomLimit(getPosition().y)) {
        //std::cout << "depassement bottom";
        mHasExceededBottom = true;
    }
    if (getBoundingZone()->exceedsTopLimit(getPosition().y)) {
        //std::cout << "depassement top";
        mHasExceededTop = true;
    }
}

void Player::setStatePlayer(Context::StatePlayer new_state) 
{
    mBehaviour->End(this);
    mStatePlayer = new_state;
	mBehaviour->Start(this);
}


Ball* Player::getBall()
{
	return mBall;
}

void Player::setBall(Ball* new_ball)
{
	mBall = new_ball;
}

// only with team mate
bool Player::isPlayerInFront(Player* otherPlayer, int gabMore) {
    int xDir = getTeam()->getDirection();
    return xDir * (getPosition().x - otherPlayer->getPosition().x) > gabMore;
}

bool Player::isPlayerBehind(Player* otherPlayer, int gabMore) {
    int xDir = getTeam()->getDirection();
    return xDir * (otherPlayer->getPosition().x - getPosition().x) > gabMore;
}
bool Player::isPlayerSameTeam(Player* otherPlayer) {
    return getTeam()->getDirection() == otherPlayer->getTeam()->getDirection();
}