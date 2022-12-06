#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "imports/linalg.h"

using namespace linalg::aliases;

struct Player
{
    float2 pos;
    float2 dir;
    float2 plane;
    float2 speed; // the x is movement speed and the y is rotation speed
    Player() {
        this->pos = {2.0f,3.0f};
        this->dir = {-1.0f,0.0f};
        this->plane = {0.0f,0.66f};
        this->speed = {5.0f,3.0f};
    }
};



#endif // PLAYER_HPP_