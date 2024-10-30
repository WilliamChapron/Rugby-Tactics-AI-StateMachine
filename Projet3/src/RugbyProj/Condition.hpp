#pragma once

class Playground;
class GameManager;
class Player;

class Condition 
{
public:
    virtual bool Test(Player * Player) = 0;
};
