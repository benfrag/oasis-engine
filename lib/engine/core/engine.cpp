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
    //rather than giving the rendermanager a pointer to the renderer, the rendermanager should create the renderer. this philosophy should be followed throughout for other things as well.

    ecs.engine_core = this;
    render_manager.primitive_renderer = &primitive_renderer;
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

    render_manager.render_geometry_queue();

    primitive_renderer.cycle_end();
    window_manager.update_window();
    render_manager.clear_queue();
}

void EngineCore::shutdown()
{

}
