#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <vector>

#include "engine/window_manager/window_manager.h"
#include "engine/renderer/renderer.h"
#include "engine/renderer/renderer3d.h"

void enable_console()
{
    AllocConsole();
    AttachConsole(GetCurrentProcessId());
    freopen("CON", "w", stdout);
}

Vector3 world_to_screen(Vector3* vec, int s_width, int s_height, Matrix4* matrix, bool is_combined)
{
    Vector3 result;
    float* m = matrix->m;
    result.x = m[0] * vec->x + m[4] * vec->y + m[8] * vec->z + m[12];
    result.y = m[1] * vec->x + m[5] * vec->y + m[9] * vec->z + m[13];
    result.z = m[2] * vec->x + m[6] * vec->y + m[10] * vec->z + m[14];
    float w  = m[3] * vec->x + m[7] * vec->y + m[11] * vec->z + m[15];

    if (w < 0.01f)
        return {0, 0, 0};

    Vector3 NDC;
    NDC.x = result.x / w;
    NDC.y = result.y / w;
    NDC.z = result.z / w;

    Vector3 screen;
    screen.x = (s_width / 2 * NDC.x) + (NDC.x + s_width / 2);
    screen.y = -(s_height / 2 * NDC.y) + (NDC.y + s_height / 2);
    screen.z = 0;
    return screen;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    enable_console();
    WindowConfig config;


    Renderer renderer;
    WindowManager window_manager(hInstance, nCmdShow);

    window_manager.create_window(config, &renderer);
    renderer.update_config(config.width, config.height);

    Renderer3D engine_renderer;
    Camera main_camera(Vector3{0, 0, -5}, Vector3{0, 0, 0}, Vector3{0, 1, 0});
    engine_renderer.set_camera(&main_camera);

    HWND hwnd = window_manager.get_hwnd();
          RECT rect;
            if (GetWindowRect(hwnd, &rect))
            {
                int x = rect.left; // X coordinate of the window's top-left corner
                int y = rect.top;  // Y coordinate of the window's top-left corner
                SetCursorPos(x + 1280 / 2, y + 720 / 2);

            }

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

    Vector3 simple_triangle[] = {{0.0f, 0.0f, 0.0f,}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}};
    bool running = true;
    while (running)
    {
        running = window_manager.process_messages();
        if (!running) break;

        renderer.cycle_start();
        main_camera.fps_cycle(hwnd);


        for (int i = 0; i < triangle_list.size() - 3; )
        {
            Vector3 list_screen[3];
            for (int j = 0; j < 3; j++) {
                Vector3 pt = triangle_list[i + j];
                Vector3 screen_pt = world_to_screen(&pt, 1280, 720, &main_camera.view_projection_matrix, true);
                list_screen[j] = screen_pt;
            }
            renderer.draw_triangle(list_screen[0].x, list_screen[0].y, list_screen[1].x, list_screen[1].y, list_screen[2].x, list_screen[2].y, PACK(255, 255, 100, 255));
            i += 3;
        }

        renderer.cycle_end();

        window_manager.update_window();

        if (GetActiveWindow() == hwnd)
        {
            RECT rect;
            if (GetWindowRect(hwnd, &rect))
            {
                int x = rect.left; // X coordinate of the window's top-left corner
                int y = rect.top;  // Y coordinate of the window's top-left corner
                SetCursorPos(x + 1280 / 2, y + 720 / 2);
            }
        }
    }

    return 0;
}
