#pragma once
#include <stdint.h>

typedef struct
{
    uint32_t  map_width;
    uint32_t  map_height;
    uint32_t* map_buffer;
    uint32_t* map_aux;
    uint32_t  noise_density;

} map_generation_application;


map_generation_application* create_map_generation_application(uint32_t width, uint32_t height, uint32_t noise_density);
void delete_map_generation_application(map_generation_application* app);
uint8_t update_map_generation(map_generation_application* app);

void generate_map(map_generation_application* app);