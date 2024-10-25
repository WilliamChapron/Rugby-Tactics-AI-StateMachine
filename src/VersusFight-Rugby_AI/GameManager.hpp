#pragma once

#include <vector>

class Entity;

namespace sf 
{
	class RenderWindow;
}

class GameManager
{
	std::vector<Entity*> mEntities;
	sf::RenderWindow* mWindow;
	float mDeltaTime;

	GameManager();

	void addEntity(Entity* entity);

public:
	~GameManager();

	static GameManager* Instantiate();
	static GameManager* Get();
	void Update();
	void Draw();

	void setWindow(sf::RenderWindow* window);
	void setDeltaTime(float deltaTime);
	float getDeltaTime() const;

	friend Entity;
};

