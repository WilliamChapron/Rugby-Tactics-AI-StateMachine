#pragma once

#include <SFML/System/Vector2.hpp>

namespace sf 
{
	class Shape;
}

class Player;

class Entity
{
protected:
    sf::Vector2f mDirection;
    float mSize;
	float mSpeed;

	sf::Shape* mShape;

    int mDrawPriority;

public:
    Entity(int width, int height, sf::Vector2f position);
    Entity();

    virtual void Update();

    virtual void OnCollide(Entity* ply) = 0;

    inline float getSpeed() { return mSpeed; }
    inline void setSpeed(float speed) { mSpeed = speed; }

    ~Entity();

    void setDirection(const sf::Vector2f& direction);
    sf::Vector2f const getDirection() { return mDirection; };
    inline void clearDirection(){ mDirection = sf::Vector2f(0.f, 0.f);}


    void goToPosition(const sf::Vector2f& position);



    void setPosition(const sf::Vector2f& position);
    const sf::Vector2f& getPosition() const;

    const sf::Shape& getShape();

    inline void setShapeColor(const sf::Color& color) { mShape->setFillColor(color);}

    int getPriority() const { return mDrawPriority; }
    int setPriority(int priority) { mDrawPriority = priority; }
};
