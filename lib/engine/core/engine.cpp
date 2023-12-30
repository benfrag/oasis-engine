#include "engine/core/engine.h"

EngineCore::EngineCore() : is_running(false)
{

}

EngineCore::~EngineCore()
{
    shutdown();
}

WindowConfig EngineCore::get_window_config()
{
    return window_config;
}

void EngineCore::configure_window(WindowConfig window_config)
{
    this->window_config = window_config;
}

void EngineCore::reconfigure_window(WindowConfig window_config)
{
    //need to update window
    //need to update renderer, frame buffer and tiles
    //this needs to be performed prior to a render cycle
    //need to update renderer manager etc.

}

void EngineCore::initialize()
{
    //rather than giving the rendermanager a pointer to the renderer, the rendermanager should create the renderer. this philosophy should be followed throughout for other things as well.

    ecs.engine_core = this;
    render_manager.primitive_renderer = &primitive_renderer;
    render_manager.window_config = &window_config;
    is_running = true;
    window_manager.input_manager = &input_manager;
    window_manager.create_window(&window_config, &primitive_renderer);
}

void EngineCore::run()
{
    last_frame_time = std::chrono::high_resolution_clock::now();
    while (is_running)
    {
        auto current_time = std::chrono::high_resolution_clock::now();
        float delta = std::chrono::duration<float, std::chrono::seconds::period>(current_time - last_frame_time).count();
        last_frame_time = current_time;

        process_input();
        update(delta);
        render(delta);
    }
}

void EngineCore::process_input()
{
    window_manager.process_messages();
}

void EngineCore::update(float dt)
{
    ecs.update_systems(dt);
}

void EngineCore::render(float dt)
{
    primitive_renderer.cycle_start();

    render_manager.render_geometry_queue();

    primitive_renderer.shader_filter_hook(dt);
    primitive_renderer.cycle_end();
    window_manager.update_window();
    render_manager.clear_queue();
}

void EngineCore::shutdown()
{

}
