#pragma once
#include <stdint.h>

//https://www.hermetic-systems.com/compsci/cellular_automata_algorithms.htm#bz2

typedef struct
{
    uint32_t  map_width;
    uint32_t  map_height;
    uint32_t* color_buffer;
    uint32_t* map_buffer;
    uint32_t* map_aux;
    uint8_t   is_running;

    uint32_t infection_rate;
    uint32_t k1;
    uint32_t k2;
    uint32_t number_of_states;

} belousov_zhabotinsky_application;

belousov_zhabotinsky_application* create_belousov_zhabotinsky_application(uint32_t width, uint32_t height, 
    uint32_t k1, uint32_t k2, uint32_t infection_rate);
void delete_belousov_zhabotinsky_application(belousov_zhabotinsky_application* app);
uint8_t update_belousov_zhabotinsky(belousov_zhabotinsky_application* app);
