#include "pch.h"

#include "Behaviour.hpp"
#include "Context.hpp"

#include "Player.hpp"
#include "Transition.hpp"
#include "Action.hpp"


Behaviour::Behaviour()
{
}

Behaviour::~Behaviour()
{
    for (auto it : mTransitions)
    {
        for (int i = 0; i < it.second.size(); ++i)
        {
            delete it.second[i];
        }
    }

    for (auto it : mActions)
    {
        for (int i = 0; i < it.second.size(); ++i)
        {
            delete it.second[i];
        }
    }
}

void Behaviour::AddTransition(Context::StatePlayer key, Transition* value)
{
    mTransitions[key].push_back(value);
}

void Behaviour::AddAction(Context::StatePlayer key, Action* value)
{
    mActions[key].push_back(value);
}

void Behaviour::Start(Player * player)
{
    for (const auto & a : mActions[player->getPlayerState()])
    {
        a->Start(player);
    }
}

void Behaviour::Update(Player * player)
{
    for (const auto & a : mActions[player->getPlayerState()])
    {
        a->Update(player);
    }

    for (const auto & a : mTransitions[player->getPlayerState()])
    {
        a->Try(player);
    }
}


void Behaviour::End(Player * player)
{
    for (const auto& a : mActions[player->getPlayerState()])
    {
        a->End(player);
    }
}