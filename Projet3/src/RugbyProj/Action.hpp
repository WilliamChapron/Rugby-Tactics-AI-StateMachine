#pragma once

class Player;

class Action
{
public:
    virtual void Start(Player * Player) = 0;
    virtual void Update(Player * Player) = 0;
    virtual void End(Player * Player) = 0;
};