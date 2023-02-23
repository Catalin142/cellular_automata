#include "platform.h"

#include <stdio.h>

uint32_t WINDOW_WIDTH = 0;
uint32_t WINDOW_HEIGHT = 0;

LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_SIZE:
        WINDOW_WIDTH = LOWORD(lParam);
        WINDOW_HEIGHT = HIWORD(lParam);
        break;

    case WM_CLOSE:   DestroyWindow(hwnd); break;
    case WM_DESTROY: PostQuitMessage(0); break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}


window* create_window(const wchar_t* window_name, uint32_t width, uint32_t height)
{
    window* wnd                      = malloc(sizeof(window));
    if (!wnd)
        return (void*)0;

    WINDOW_WIDTH  = width;
    WINDOW_HEIGHT = height;

    wnd->is_running                  = 1;
    wnd->width                       = width;
    wnd->height                      = height;
    wnd->window_class.style          = 0;
    wnd->window_class.lpfnWndProc    = window_proc;
    wnd->window_class.cbClsExtra     = 0;
    wnd->window_class.cbWndExtra     = 0;
    wnd->window_class.hInstance      = GetModuleHandle(NULL);
    wnd->window_class.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
    wnd->window_class.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wnd->window_class.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wnd->window_class.lpszMenuName   = NULL;
    wnd->window_class.lpszClassName  = L"Window class";

    RegisterClass(&wnd->window_class);

    wnd->window_handle = CreateWindowEx(0, wnd->window_class.lpszClassName, window_name, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, GetModuleHandle(NULL), NULL);

    if (!wnd->window_handle)
    {
        printf("Error creating the window");
        return (void*)0;
    }

    wnd->device_context = GetDC(wnd->window_handle);
    ShowWindow(wnd->window_handle, SW_SHOW);

    return wnd;
}

void delete_window(window* wnd)
{
    free(wnd);
}

void poll_events(window* wnd)
{
    if (wnd->width != WINDOW_WIDTH)
        wnd->width = WINDOW_WIDTH;

    if (wnd->width != WINDOW_HEIGHT)
        wnd->height = WINDOW_HEIGHT;

    MSG msg = { 0 };
    PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);

    if (msg.message == WM_QUIT)
        wnd->is_running = 0;

    TranslateMessage(&msg);
    DispatchMessage(&msg);
}

graphics_context* create_graphics_context(window* wnd, uint32_t pixel_size)
{
    graphics_context* gfx = malloc(sizeof(graphics_context));
    if (!gfx)
        return (void*)0;

    gfx->wnd              = wnd;
    gfx->width            = wnd->width  / pixel_size;
    gfx->height           = wnd->height / pixel_size;
    gfx->buffer_size      = gfx->width  * gfx->height;

    gfx->memory_buffer = (uint32_t*)VirtualAlloc(0,
        gfx->buffer_size * sizeof(uint32_t), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    if (!gfx->memory_buffer)
    {
        printf("Error creating the window");
        return (void*)0;
    }

    gfx->bitmap_info.bmiHeader.biWidth       = gfx->width;
    gfx->bitmap_info.bmiHeader.biHeight      = gfx->height;

    gfx->bitmap_info.bmiHeader.biSize        = sizeof(gfx->bitmap_info.bmiHeader);
    gfx->bitmap_info.bmiHeader.biPlanes      = 1;
    gfx->bitmap_info.bmiHeader.biBitCount    = sizeof(uint32_t) * 8;
    gfx->bitmap_info.bmiHeader.biCompression = BI_RGB;

    return gfx;
}

void delete_graphics_context(graphics_context* gfx)
{
    free(gfx);
}

void present(graphics_context* gfx)
{
    StretchDIBits(gfx->wnd->device_context, 0, 0, gfx->wnd->width, gfx->wnd->height,
        0, 0, gfx->width, gfx->height, gfx->memory_buffer, &gfx->bitmap_info, DIB_RGB_COLORS, SRCCOPY);
}
