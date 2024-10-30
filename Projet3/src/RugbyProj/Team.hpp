#pragma once

class Player;

class Team
{
private :
	std::vector<Player*> mPlayers;
	int mDirection;

public :
	Team();
	Team(int direction);

	void AddPlayer(Player* player);

	std::vector<Player*> getPlayers();
	int getDirection();
};