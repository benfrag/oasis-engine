#include "engine/core/engine.h"

EngineCore::EngineCore() : is_running(false)
{

}

EngineCore::~EngineCore()
{
    shutdown();
}

void EngineCore::configure_window(WindowConfig window_config)
{
    this->window_config = window_config;
}

void EngineCore::initialize()
{
    ecs.engine_core = this;
    is_running = true;
    window_manager.create_window(window_config, &primitive_renderer);
}

void EngineCore::run()
{
    while (is_running)
    {
        process_input();
        update();
        render();
    }
}

void EngineCore::process_input()
{
    window_manager.process_messages();
}

void EngineCore::update()
{
    ecs.update_systems(1);
}

void EngineCore::render()
{
    primitive_renderer.cycle_start();

    primitive_renderer.draw_line(0, 0, 100, 100, PACK(255, 255, 255, 255), 1);
        
    std::vector<Vector3> triangle_list = {
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

    
    for (int i = 0; i < triangle_list.size() - 3; )
    {
        Vector3 list_screen[3];
        for (int j = 0; j < 3; j++) {
            Vector3 pt = triangle_list[i + j];
            Vector3 screen_pt = render_manager.world_to_screen(&pt, 800, 600, true);
            list_screen[j] = screen_pt;
        }
        primitive_renderer.draw_triangle(list_screen[0].x, list_screen[0].y, list_screen[1].x, list_screen[1].y, list_screen[2].x, list_screen[2].y, PACK(255, 255, 100, 255));
        i += 3;
    }

    primitive_renderer.cycle_end();
    window_manager.update_window();
}

void EngineCore::shutdown()
{

}
