#include "engine/core/engine.h"
#include "engine/core/base_systems/physics_system.h"

void PhysicsSystem::update(void* engine_core, float dt)
{
    EngineCore* engine = static_cast<EngineCore*>(engine_core);

    for (auto entity : entities)
    {
        TransformComponent* transform = engine->ecs.get_component<TransformComponent>(entity);
        PhysicsComponent* physics = engine->ecs.get_component<PhysicsComponent>(entity);

        //gravity
        
//        Vector3 g = {0, -9.8, 0};
        if (physics->velocity != Vector3{0, 0, 0})
        {
              float friction = 0.8;
            Vector3 friction_velocity = physics->velocity.normalized() * (-friction * dt);
            physics->velocity = physics->velocity + friction_velocity;
          float magnitude = physics->velocity.magnitude();
            if (magnitude <= 0.001)
                physics->velocity = Vector3{0, 0, 0};
            std::cout << "magnitude " << magnitude << std::endl;

        }
        physics->velocity = physics->velocity + (physics->acceleration * dt);
        
        transform->position = transform->position + physics->velocity * dt;
    }

}
