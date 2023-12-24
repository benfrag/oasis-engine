#ifndef GAME_H
#define GAME_H

#include "engine/core/engine.h"
#include "example_game/components/include_components.h"
#include "example_game/systems/include_systems.h"

class Game
{
public:
    Game(EngineCore* engine);
    
    void setup();

private:
    EngineCore* engine;

};

#endif
