#include "platform.h"
#include "map_generation.h"
#include "game_of_life.h"
#include "belousov_zhabotinsky.h"

const uint32_t WHITE = 0xffffff;
const uint32_t BLACK = 0x000000;

#define MAP_GENERATION
//#define GAME_OF_LIFE 
//#define BELOUSOV_ZHABOTINSKY

void get_mouse_position(uint32_t* x, uint32_t* y, window* wnd, graphics_context* ctx)
{
    POINT p = { (LONG)0.0, (LONG)0.0 };
    GetCursorPos(&p);
    ScreenToClient(wnd->window_handle, &p);

    float norm_x = (float)ctx->width / wnd->width;
    float norm_y = (float)ctx->height / wnd->height;
    *x = (uint32_t)((float)p.x * norm_x);
    *y = (uint32_t)((float)p.y * norm_y);

    *y = ctx->height - *y;
}

int main()
{
    const uint32_t pixel_size = 1;
    const uint32_t window_width = 800;
    const uint32_t window_height = 600;

    window* wnd = create_window(L"Cellular Automata", window_width, window_height);
    graphics_context* ctx = create_graphics_context(wnd, pixel_size);

#ifdef MAP_GENERATION
    map_generation_application* app = create_map_generation_application(window_width / pixel_size, window_height / pixel_size, 60);
#endif
#ifdef GAME_OF_LIFE
    game_of_life_application* app = create_game_of_life_application(window_width / pixel_size, window_height / pixel_size);
#endif
#ifdef BELOUSOV_ZHABOTINSKY
    belousov_zhabotinsky_application* app = create_belousov_zhabotinsky_application(window_width / pixel_size, 
        window_height / pixel_size, 1, 1, 10);
#endif

    while (wnd->is_running == 1)
    {
#ifdef MAP_GENERATION
        if (update_map_generation(app))
            memcpy(ctx->memory_buffer, app->map_buffer, app->map_width * app->map_height * sizeof(uint32_t));
#endif

#ifdef GAME_OF_LIFE
        if (GetKeyState(VK_LBUTTON) < 0)
        {
            uint32_t x, y;
            get_mouse_position(&x, &y, wnd, ctx);
            put_cell(app, x, y);
            memcpy(ctx->memory_buffer, app->map_buffer, app->map_width * app->map_height * sizeof(uint32_t));
        }

        if (update_game_of_life(app))
            memcpy(ctx->memory_buffer, app->map_buffer, app->map_width * app->map_height * sizeof(uint32_t));
#endif

#ifdef BELOUSOV_ZHABOTINSKY
        if (update_belousov_zhabotinsky(app))
            memcpy(ctx->memory_buffer, app->color_buffer, app->map_width * app->map_height * sizeof(uint32_t));
#endif

        poll_events(wnd);
        present(ctx);
    }

#ifdef MAP_GENERATION
    delete_map_generation_application(app);
#endif

#ifdef GAME_OF_LIFE
    delete_game_of_life_application(app);
#endif

#ifdef BELOUSOV_ZHABOTINSKY
    delete_belousov_zhabotinsky_application(app);
#endif

    delete_graphics_context(ctx);
    delete_window(wnd);
}