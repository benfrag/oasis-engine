#ifndef GAME_H
#define GAME_H

#include "engine/core/engine.h"

class Game
{
public:
    Game(EngineCore* engine);
    
    void setup();

private:
    EngineCore* engine;

};

#endif
