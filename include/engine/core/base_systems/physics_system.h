#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "engine/core/ecs/system.h"

class PhysicsSystem : public System
{
public:
    void update(void* engine_core, float dt) override;
};

#endif
