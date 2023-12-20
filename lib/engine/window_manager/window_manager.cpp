#include "engine/window_manager/window_manager.h"

WindowManager::WindowManager(HINSTANCE hInstance, int nCmdShow) : hInstance(hInstance), nCmdShow(nCmdShow)
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

bool WindowManager::create_window(WindowConfig config, IRenderable* renderer)
{
    this->renderer = renderer;

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowManager::WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = config.class_name.c_str();
    RegisterClass(&wc);

    hwnd = CreateWindowEx(0, config.class_name.c_str(), config.window_name.c_str(), WS_OVERLAPPEDWINDOW, config.x, config.y, config.width, config.height, nullptr, nullptr, hInstance, this);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    return true;
}

LRESULT WindowManager::class_window_proc(UINT umsg, WPARAM wparam, LPARAM lparam)
{
    switch (umsg)
    {
        case WM_PAINT:
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            int width, height;
            width = renderer->get_width();
            height = renderer->get_height();

            uint32_t* front_buffer = renderer->get_front_buffer();
            BITMAPINFO bmi = {};
            bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
            bmi.bmiHeader.biWidth = width;
            bmi.bmiHeader.biHeight = -height;
            bmi.bmiHeader.biPlanes = 1;
            bmi.bmiHeader.biBitCount = 32;
            bmi.bmiHeader.biCompression = BI_RGB;

            StretchDIBits(hdc, 0, 0, width, height, 0, 0, width, height, front_buffer, &bmi, DIB_RGB_COLORS, SRCCOPY);
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
