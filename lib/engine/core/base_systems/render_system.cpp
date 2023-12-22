#include "engine/core/engine.h"
#include "engine/core/base_systems/render_system.h"
#include <iostream>

void RenderSystem::update(void* engine_core, float dt)
{
    EngineCore* engine = static_cast<EngineCore*>(engine_core);

    for (auto entity : entities)
    {
        GeometryComponent* geometry = engine->ecs.get_component<GeometryComponent>(entity);

        //apply transform then send to queue
        engine->render_manager.queue_geometry(*geometry);
    }
}
