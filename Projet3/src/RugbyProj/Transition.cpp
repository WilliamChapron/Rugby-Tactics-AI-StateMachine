#include "pch.h"

#include "Transition.hpp"
#include "Player.hpp"
#include "Behaviour.hpp"

void Transition::setTargetState(Context::StatePlayer target_state)
{
    mTargetState = target_state;

}

void Transition::addCondition(Condition* condition)
{
    mConditions.push_back(condition);
}

void Transition::Try(Player * Player)
{
    int true_tests = 0;
    for (const auto &c : mConditions)
    {
        true_tests += c->Test(Player);
    }
    if (true_tests != 0 && true_tests == mConditions.size())
    {
        Player->setStatePlayer(mTargetState);
        // Start actions
    }
}
