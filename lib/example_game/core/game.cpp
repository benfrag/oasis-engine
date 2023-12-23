#include "example_game/core/game.h"

struct CameraComponent
{
    Matrix4 view_matrix;
    Matrix4 projection_matrix;
    Matrix4 view_projection_matrix;

    float yaw = 0, pitch = 0;

    CameraComponent(float aspect_ratio)
    {
        /*if (init)
        {
            projection_matrix = Matrix4::create_perspective(90.0f, 1280/720,)
        }*/
//        yaw = 25;
    }

    CameraComponent() {}
};

struct LocalPlayerComponent
{

};

class LocalPlayerInputSystem : public System
{
public:
    LocalPlayerInputSystem()
    {

    }
    void update(void* engine_core, float dt) override
    {
        EngineCore* engine = static_cast<EngineCore*>(engine_core);

        for (auto entity : entities)
        {
            CameraComponent* player_camera = engine->ecs.get_component<CameraComponent>(entity);
            TransformComponent* player_transform = engine->ecs.get_component<TransformComponent>(entity);

            float yaw = player_camera->yaw;
            float pitch = player_camera->pitch;
            Vector3 front;
            float yaw_rads = yaw * (3.14159f / 180.0f);
            float pitch_rads = pitch * (3.14159f / 180.0f);
            front.x = sin(yaw_rads) * cos(pitch_rads);
            front.y = 0; // Keep the camera level
            front.z = cos(yaw_rads) * cos(pitch_rads);
            front = front.normalized();
            Vector3 right = Vector3::new_cross(Vector3(0, 1, 0), front);
            right.new_normalize();
            
            float movement_speed = 5;
            if (engine->input_manager.is_key_pressed(VK_LEFT))
            {
                yaw += -90 * dt;
            }
            if (engine->input_manager.is_key_pressed(VK_RIGHT))
            {
                yaw += 90 * dt;
            }
            if (engine->input_manager.is_key_pressed(VK_UP))
            {
                pitch += 45 * dt;
            }
            if (engine->input_manager.is_key_pressed(VK_DOWN))
            {
                pitch += -45 * dt;
            }
            if (engine->input_manager.is_key_pressed(VK_SPACE))
            {
                player_transform->position.y += movement_speed * dt;
            }
            if (engine->input_manager.is_key_pressed(VK_CONTROL))
            {
                player_transform->position.y += -movement_speed * dt;
            }
            if (engine->input_manager.is_key_pressed(0x57))
            {
                player_transform->position = player_transform->position + (front * movement_speed) * dt;
            }
            if (engine->input_manager.is_key_pressed(0x53))
            {
                player_transform->position = player_transform->position + (front * -movement_speed) * dt;
            }
            if (engine->input_manager.is_key_pressed(0x44))
            {
                player_transform->position = player_transform->position + (right * movement_speed) * dt;
            }
            if (engine->input_manager.is_key_pressed(0x41))
            {
                player_transform->position = player_transform->position + (right * -movement_speed) * dt;
            }



            if (pitch > 89.0f) pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;
            // Normalize the yaw to be within the range 0 - 360 degrees
            if (yaw < -180.0f) {
                yaw += 360.0f;
            }
            else if (yaw > 180.0f) {
                yaw -= 360.0f;
            }

            player_camera->yaw = yaw;
            player_camera->pitch = pitch;

            player_camera->view_matrix = Matrix4::create_view(player_transform->position, player_camera->yaw, player_camera->pitch);
            player_camera->view_projection_matrix = Matrix4::multiply_mat(&(player_camera->projection_matrix), &(player_camera->view_matrix));
            for (int i = 0; i < 16; i++)
            {
//                std::cout << "m" << i << " " << player_camera->view_projection_matrix.m[i] << std::endl;
            }
//            std::cout << "entity " << entity << std::endl;
            //should be only one local_player


        }

    }


};


Game::Game(EngineCore* engine) : engine(engine)
{

}

void Game::setup()
{
    auto local_player_input_system = engine->ecs.register_system<LocalPlayerInputSystem>();

    Signature local_player_input_signature;
    local_player_input_signature.set(engine->ecs.get_component_type_id<LocalPlayerComponent>());
    local_player_input_signature.set(engine->ecs.get_component_type_id<TransformComponent>());
    local_player_input_signature.set(engine->ecs.get_component_type_id<CameraComponent>());

    engine->ecs.set_system_signature<LocalPlayerInputSystem>(local_player_input_signature);


    //have to define systems before hand for now

    auto render_system = engine->ecs.register_system<RenderSystem>();

    Signature render_signature;
    render_signature.set(engine->ecs.get_component_type_id<GeometryComponent>());
    render_signature.set(engine->ecs.get_component_type_id<TransformComponent>());

    engine->ecs.set_system_signature<RenderSystem>(render_signature);



    Entity test_cube = engine->ecs.create_entity();
    GeometryComponent cube_geometry;

    cube_geometry.vertices = {
        { 0.0f, 0.0f, 0.0f},    {0.0f, 1.0f, 0.0f},    {1.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f},    {1.0f, 1.0f, 0.0f},    {1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f},    {1.0f, 1.0f, 0.0f},    {1.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f},    {1.0f, 1.0f, 1.0f},    {1.0f, 0.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f},    {1.0f, 1.0f, 1.0f},    {0.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f},    {0.0f, 1.0f, 1.0f},    {0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f},    {0.0f, 1.0f, 1.0f},    {0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f},    {0.0f, 1.0f, 0.0f},    {0.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f},    {0.0f, 1.0f, 1.0f},    {1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f},    {1.0f, 1.0f, 1.0f},    {1.0f, 1.0f, 0.0f },
		{ 1.0f, 0.0f, 1.0f},    {0.0f, 0.0f, 1.0f},    {0.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 1.0f},    {0.0f, 0.0f, 0.0f},    {1.0f, 0.0f, 0.0f },
    };

    cube_geometry.clr = PACK(255, 100, 20, 255);

    engine->ecs.add_component(test_cube, TransformComponent{{0, 5, 0}});
    engine->ecs.add_component(test_cube, cube_geometry);

    Entity test_plane = engine->ecs.create_entity();
    GeometryComponent plane_geometry;

    plane_geometry.vertices = {
        { -5.0f, 0.0f, -5.0f},    {-5.0f, 0.0f, 5.0f},    {5.0f, 0.0f, 5.0f },
          { -5.0f, 0.0f, -5.0f},       { 5.0f, 0.0f, 5.0f },    { 5.0f, 0.0f, -5.0f }
     };

    plane_geometry.clr = PACK(52, 191, 246, 255);

    engine->ecs.add_component(test_plane, TransformComponent{{0, 0, 0}});
    engine->ecs.add_component(test_plane, plane_geometry);






    Entity local_player = engine->ecs.create_entity();
    engine->ecs.add_component(local_player, TransformComponent{{-5, 0, -5}});
    engine->ecs.add_component(local_player, CameraComponent(true));
    engine->ecs.add_component(local_player, LocalPlayerComponent{});

    CameraComponent* main_camera = engine->ecs.get_component<CameraComponent>(local_player);
    std::cout << "comp yaw : " << main_camera->yaw << std::endl;

    //setup player

    main_camera->projection_matrix = Matrix4::create_perspective(90.0f, 800.f / 600.f, 0.1f, 100.0f);

    engine->render_manager.set_active_view_matrix(&(main_camera->view_projection_matrix));

}
