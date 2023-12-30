#include "engine/window_manager/window_manager.h"
#include <windowsx.h>

WindowManager::WindowManager()
{

}

WindowManager::~WindowManager()
{

}

LRESULT CALLBACK WindowManager::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    WindowManager* pThis = nullptr;

    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCT* create = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = static_cast<WindowManager*>(create->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
        pThis->hwnd = hwnd;
    }
    else
    {
        pThis = reinterpret_cast<WindowManager*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (pThis)
        return pThis->class_window_proc(uMsg, wParam, lParam);

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool WindowManager::create_window(WindowConfig* config, IRenderable* renderer)
{
    this->window_config = config;
    this->renderer = renderer;
    hInstance = GetModuleHandle(nullptr);

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowManager::WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = config->class_name.c_str();
    RegisterClass(&wc);

    hwnd = CreateWindowEx(0, config->class_name.c_str(), config->window_name.c_str(), WS_OVERLAPPEDWINDOW, config->x, config->y, config->window_width, config->window_height, nullptr, nullptr, hInstance, this);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    renderer->update_config(config->width, config->height);
    return true;
}

LRESULT WindowManager::class_window_proc(UINT umsg, WPARAM wparam, LPARAM lparam)
{
    switch (umsg)
    {
        case WM_KEYDOWN:
            input_manager->set_key_down(wparam);
            break;
        case WM_KEYUP:
            input_manager->set_key_up(wparam);
            break;
        case WM_MOUSEMOVE:
            input_manager->set_mouse_position(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            break;
        case WM_LBUTTONDOWN:
            input_manager->set_mouse_button_down(MOUSE_LEFT);
            break;
        case WM_LBUTTONUP:
            input_manager->set_mouse_button_up(MOUSE_LEFT);
            break;
        case WM_RBUTTONDOWN:
            input_manager->set_mouse_button_down(MOUSE_LEFT);
            break;
        case WM_RBUTTONUP:
            input_manager->set_mouse_button_up(MOUSE_LEFT);
            break;
        case WM_PAINT:
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

//            int width, height;
//            width = renderer->get_width();
//            height = renderer->get_height();

            uint32_t* front_buffer = renderer->get_front_buffer();
            BITMAPINFO bmi = {};
            bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
            bmi.bmiHeader.biWidth = window_config->width;
            bmi.bmiHeader.biHeight = -window_config->height;
            bmi.bmiHeader.biPlanes = 1;
            bmi.bmiHeader.biBitCount = 32;
            bmi.bmiHeader.biCompression = BI_RGB;

            StretchDIBits(hdc, 0, 0, window_config->window_width, window_config->window_height, 0, 0, window_config->width, window_config->height, front_buffer, &bmi, DIB_RGB_COLORS, SRCCOPY);
            EndPaint(hwnd, &ps);
            return 0;
    }

    return DefWindowProc(hwnd, umsg, wparam, lparam);
}

bool WindowManager::process_messages()
{
    MSG msg;
    bool should_close = false;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            should_close = true;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return !should_close;
}

void WindowManager::update_window()
{
    InvalidateRect(hwnd, nullptr, false);
    UpdateWindow(hwnd);
}

HWND WindowManager::get_hwnd()
{
    return hwnd;
}
