#pragma once

#include "Context.hpp"
#include <map>
#include <vector>

class Action;
class Player;
class Transition;

class Behaviour
{
    std::map<Context::State, std::vector<Action*>> mActions;
    std::map<Context::State, std::vector<Transition*>> mTransitions;
public:
    Behaviour();
    ~Behaviour();

    void Start(Player *player);
    void Update(Player *player);
    void End(Player *player);

    void AddAction(Context::State key, Action* value);
    void AddTransition(Context::State key, Transition* value);
};