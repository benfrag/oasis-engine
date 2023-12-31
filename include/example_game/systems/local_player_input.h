#include "engine/core/engine.h"

class LocalPlayerInputSystem : public System
{
public:
    LocalPlayerInputSystem() {}

    void update(void* engine_core, float dt) override
    {
        EngineCore* engine = static_cast<EngineCore*>(engine_core);

        for (auto entity : entities)
        {
            CameraComponent* player_camera = engine->ecs.get_component<CameraComponent>(entity);
            TransformComponent* player_transform = engine->ecs.get_component<TransformComponent>(entity);
            PhysicsComponent* player_physics = engine->ecs.get_component<PhysicsComponent>(entity);

            bool has_changed = false;
            bool has_changed2 = false;
            has_changed = update_camera_rotation(engine, player_camera, dt);
            has_changed2 = update_player_position(engine, player_camera, player_physics, player_transform, dt);
            if (has_changed || has_changed2)
            {
                player_camera->clamp_rotation();
                Vector3 new_position = player_transform->position;
                player_camera->update_view(new_position);
                player_camera->last_position = new_position;
            }
       }

    }

private:
    const float MOUSE_SENSITIVITY = 90.f;
    const float MOVEMENT_SPEED = 5.0f;
    const float MOVEMENT_ACCELERATION = 10.0f;

    bool update_camera_rotation(EngineCore* engine, CameraComponent* camera, float dt)
    {
        bool has_changed = false;

        if (engine->input_manager.is_key_pressed(VK_LEFT))
        {
            camera->yaw += -MOUSE_SENSITIVITY * dt;
            has_changed = true;
        }
        if (engine->input_manager.is_key_pressed(VK_RIGHT))
        {
            camera->yaw += MOUSE_SENSITIVITY * dt;
            has_changed = true;
        }
        if (engine->input_manager.is_key_pressed(VK_UP))
        {
            camera->pitch += MOUSE_SENSITIVITY * dt;
            has_changed = true;
        }
        if (engine->input_manager.is_key_pressed(VK_DOWN))
        {
            camera->pitch -= MOUSE_SENSITIVITY * dt;
            has_changed = true;
        }

        return has_changed;
    }

    bool update_player_position(EngineCore* engine, CameraComponent* camera, PhysicsComponent* physics, TransformComponent* transform, float dt)
    {
        //should be based on velocity should perhaps just update the camera if the position has changed
        //but allow the physics systems to do the movement
        //or just add the velocity here

        bool has_changed = false;
        Vector3 front = camera->get_front();
        Vector3 right = Vector3::new_cross(Vector3(0, 1, 0), front).normalized();
        Vector3 movement_acceleration = 0;

        if (engine->input_manager.is_key_pressed(VK_SPACE))
        {
            transform->position.y += MOVEMENT_SPEED * dt;
            has_changed = true;
        }
        if (engine->input_manager.is_key_pressed(VK_CONTROL))
        {
            transform->position.y -= MOVEMENT_SPEED * dt;
            has_changed = true;
        }
        if (engine->input_manager.is_key_pressed(0x57))  // W Key
        {
            movement_acceleration = movement_acceleration + front;
            //movement_acceleration = front * MOVEMENT_ACCELERATION;
//            transform->position = transform->position + front * MOVEMENT_SPEED * dt;
//            has_changed = true;
        }
        if (engine->input_manager.is_key_pressed(0x53))  // S Key
        {
//            transform->position = transform->position - front * MOVEMENT_SPEED * dt;
//            has_changed = true;
//
            movement_acceleration = movement_acceleration - front;
        }
        if (engine->input_manager.is_key_pressed(0x44))  // D Key
        {
            //transform->position = transform->position + right * MOVEMENT_SPEED * dt;
       //     has_changed = true;
            movement_acceleration = movement_acceleration + right;
        }
        if (engine->input_manager.is_key_pressed(0x41))  // A Key
        {
            //transform->position = transform->position - right * MOVEMENT_SPEED * dt;
            //has_changed = true;
            movement_acceleration = movement_acceleration - right;
        }
        
        if (transform->position != camera->last_position)
        {
            has_changed = true;
        }


physics->acceleration = movement_acceleration.normalized() * MOVEMENT_ACCELERATION;

        //physics->acceleration = movement_acceleration;

        return has_changed;
    }
};

