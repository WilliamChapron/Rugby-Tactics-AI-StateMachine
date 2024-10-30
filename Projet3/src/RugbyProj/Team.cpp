#include "pch.h"

#include "Team.hpp"

Team::Team()
{
}

Team::Team(int direction)
{
	mDirection = direction;
}

void Team::AddPlayer(Player* player)
{
	mPlayers.push_back(player);
}

std::vector<Player*> Team::getPlayers()
{
	return mPlayers;
}

int Team::getDirection()
{
	return mDirection;
}
