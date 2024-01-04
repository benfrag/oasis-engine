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
        
        if (physics->velocity != Vector3{0, 0, 0})
        {
            Vector3 normalized_velocity = physics->velocity;
            Vector3 normalized_acceleration = physics->acceleration.normalized();
            float _dot = Vector3::new_dot(normalized_velocity, normalized_acceleration);
            Vector3 parallel = normalized_acceleration * _dot;
/*
            std::cout.precision(17);
            std::cout << "parallel " << parallel.x << " " << parallel.y << " " << parallel.z << std::endl;
            std::cout << "velocity " << normalized_velocity.x << " " << normalized_velocity.y << " " << normalized_velocity.z << std::endl;
            std::cout << "n accela " << normalized_acceleration.x << " " << normalized_acceleration.y << " " << normalized_acceleration.z << std::endl;
            std::cout << "accela   " << physics->acceleration.x << " " << physics->acceleration.y << " " << physics->acceleration.z << std::endl;
*/
            //dont apply deceleration parallel to acceleration
            Vector3 unaligned_velocity = physics->velocity - parallel;

            float friction = 0.05;
            Vector3 friction_velocity = unaligned_velocity * (-friction);
            physics->velocity = physics->velocity + friction_velocity;
            float magnitude = physics->velocity.magnitude();
            if (magnitude <= 0.001)
                physics->velocity = Vector3{0, 0, 0};
        }
        physics->velocity = physics->velocity + (physics->acceleration * dt);
        
        transform->position = transform->position + physics->velocity * dt;
    }

}
