#include "engine/core/engine.h"
#include "engine/core/base_systems/render_system.h"
#include <iostream>

void RenderSystem::update(void* engine_core, float dt)
{
    EngineCore* engine = static_cast<EngineCore*>(engine_core);

    for (auto entity : entities)
    {
        GeometryComponent* geometry = engine->ecs.get_component<GeometryComponent>(entity);
        TransformComponent* transform = engine->ecs.get_component<TransformComponent>(entity);
        if (geometry->old_transform != *transform)
        {
            Vector3 delta_position = transform->position - geometry->old_transform.position;
            std::size_t n = geometry->vertices.size();
            for (std::size_t i = 0; i < n; ++i)
            {
                geometry->vertices[i] = geometry->vertices[i] + delta_position;
            }

            geometry->old_transform = *transform;
        }

        //apply transform then send to queue
        engine->render_manager.queue_geometry(*geometry);
    }
}
