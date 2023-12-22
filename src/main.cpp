#include <windows.h>
#include <iostream>

#include "engine/core/engine.h"
#include "example_game/core/game.h"

struct TransformComponent 
{
    Vector3 position = {-5, 0, -5};
    float r1, r2, r3;
};

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

    CameraComponent()
    {
        std::cout << "camera constructor called " << std::endl;

    }
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
            if (GetAsyncKeyState(VK_LEFT))
            {
                yaw += -0.1;
            }
            if (GetAsyncKeyState(VK_RIGHT))
            {
                yaw += 0.1;
            }
            if (GetAsyncKeyState(VK_UP))
            {
                pitch += 0.1;
            }
            if (GetAsyncKeyState(VK_DOWN))
            {
                pitch += -0.1;
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

/*class RenderSystem : public System
{
public:
     void update(void* engine_core, float dt) override
    {
        EngineCore* engine = static_cast<EngineCore*>(engine_core);

        for (auto entity : entities)
        {

            PositionComponent* pc = engine->ecs.get_component<PositionComponent>(entity);
            std::cout << "has entity " << entity << std::endl;
            std::cout << pc->y << std::endl;
            
        }
    }
};
*/



//render system adds to triangle queue?
//triangle queue handles depth perception
//triangle queue handles world to screen
//render system performs transformationcomponent onto meshcomponent and sends world coordinates of mesh (triangle and indices) to queue.



int main()
{
    EngineCore engine;

/*    auto render_system = engine.ecs.register_system<RenderSystem>();
    Signature render_signature;
    render_signature.set(engine.ecs.get_component_type_id<PositionComponent>());
    engine.ecs.set_system_signature<RenderSystem>(render_signature);


    Entity local_player = engine.ecs.create_entity();
    engine.ecs.add_component(local_player, PositionComponent{0, 101.14, 0});
    Entity enemy = engine.ecs.create_entity();
    engine.ecs.add_component(enemy, PositionComponent{0, 13, 0});
    engine.ecs.add_component(local_player, TransformComponent{0, 0, 0});
//    engine.ecs.remove_component<TransformComponent>(local_player);

    Entity cube = engine.ecs.create_entity();
    std::cout << "obtained local player " << local_player << std::endl;
    std::cout << "obtained enemy " << enemy << std::endl;
    std::cout << "obtained cube " << cube << std::endl;

    PositionComponent* pc = engine.ecs.get_component<PositionComponent>(local_player);

    std::cout << " should print correct value : " << pc->y << std::endl;
*/

    //allow systems to be defined after entities
    //what happens if state or position changes before where position is required, we will have two different positions, perhaps we need to queue actions

    auto local_player_input_system = engine.ecs.register_system<LocalPlayerInputSystem>();

    Signature local_player_input_signature;
    local_player_input_signature.set(engine.ecs.get_component_type_id<LocalPlayerComponent>());
    local_player_input_signature.set(engine.ecs.get_component_type_id<TransformComponent>());
    local_player_input_signature.set(engine.ecs.get_component_type_id<CameraComponent>());

    engine.ecs.set_system_signature<LocalPlayerInputSystem>(local_player_input_signature);


    Entity local_player = engine.ecs.create_entity();
    engine.ecs.add_component(local_player, TransformComponent{});
    engine.ecs.add_component(local_player, CameraComponent(true));
    engine.ecs.add_component(local_player, LocalPlayerComponent{});

    CameraComponent* main_camera = engine.ecs.get_component<CameraComponent>(local_player);
    std::cout << "comp yaw : " << main_camera->yaw << std::endl;

    //setup player

    main_camera->projection_matrix = Matrix4::create_perspective(90.0f, 800.f / 600.f, 0.1f, 100.0f);

    engine.render_manager.set_active_view_matrix(&(main_camera->view_projection_matrix));



    
    Game game(&engine);
    try
    {
        WindowConfig window_config;
        window_config.width = 800;
        window_config.height = 600;
        engine.configure_window(window_config);
        engine.initialize();

        game.setup();

        engine.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "An exception occured: " << e.what() << std::endl;
    }

    engine.shutdown();

    return 0;
}
