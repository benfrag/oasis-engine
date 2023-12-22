#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "engine/core/ecs/system.h"

class RenderSystem : public System
{
public:
    void update(void* engine_core, float dt) override;
};

#endif
