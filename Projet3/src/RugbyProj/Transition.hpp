#pragma once

#include "Context.hpp"
#include "Condition.hpp"

class Player;

class Transition
{
protected:
    Context::StatePlayer mTargetState;
    std::vector<Condition*> mConditions;

public:
    void Try(Player * Player);
    void setTargetState(Context::StatePlayer target_state);
    void addCondition(Condition* condition);
};
