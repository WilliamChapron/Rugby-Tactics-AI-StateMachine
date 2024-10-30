#pragma once

#include "Context.hpp"

class Action;
class Player;
class Transition;

class Behaviour
{
    std::map<Context::StatePlayer, std::vector<Action*>> mActions;
    std::map<Context::StatePlayer, std::vector<Transition*>> mTransitions;
public:
    Behaviour();
    ~Behaviour();

    void Start(Player *player);
    void Update(Player *player);
    void End(Player *player);

    void AddAction(Context::StatePlayer playerkey, Action* value);
    void AddTransition(Context::StatePlayer key, Transition* value);
};