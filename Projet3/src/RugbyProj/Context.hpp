#pragma once
namespace Context
{
enum class StatePlayer
{
    // State
    Idle,
    OffensiveBallO,
    OffensiveBallX,
    JustReceivedBall,
    Defend,
    // 
    PassBall,
    Waiting,

    // Size
    Size = 7,
};
}

