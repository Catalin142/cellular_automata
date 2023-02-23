#pragma once
#include <Windows.h>
#include <stdint.h>

typedef struct
{
	WNDCLASS window_class;
	HWND window_handle;
	HDC device_context;

	uint32_t width;
	uint32_t height;

	uint8_t is_running;

} window;

window* create_window(const wchar_t* window_name, uint32_t width, uint32_t height);
void delete_window(window* wnd);
void poll_events(window* wnd);

typedef struct
{
	BITMAPINFO bitmap_info;
	uint32_t* memory_buffer;
	window* wnd;
	uint32_t pixel_size;
	uint32_t width;
	uint32_t height;
	uint32_t buffer_size;

} graphics_context;

graphics_context* create_graphics_context(window* wnd, uint32_t pixel_size);
void delete_graphics_context(graphics_context* gfx);
void present(graphics_context* gfx);
