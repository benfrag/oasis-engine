//engine core, initialises all subsystems enters main loop, coordinates higher level operations (working with Renderer3D).
#ifndef ENGINE_H
#define ENGINE_H

#include <windows.h>
#include <chrono>
#include "engine/window_manager/window_manager.h"
#include "engine/renderer/renderer.h"
#include "engine/core/ecs/ecs.h"
#include "engine/core/render_manager.h"
#include "engine/core/input_manager.h"

//include base components
#include "engine/core/base_components/include_components.h"

//include base systems
#include "engine/core/base_systems/include_systems.h"

class EngineCore
{
public:
    ECSController ecs;
    InputManager input_manager; // holds input information for given cycle
    RenderManager render_manager; // queues renderables

    EngineCore();
    ~EngineCore();

    void initialize();
    void run();
    void shutdown();

    void configure_window(WindowConfig window_config);

private:
    WindowConfig window_config;
    WindowManager window_manager;
    Renderer primitive_renderer;
    
    bool is_running;

    void process_input(); //user input
    void update(float dt);
    void render();

    std::chrono::high_resolution_clock::time_point last_frame_time;
};

#endif
