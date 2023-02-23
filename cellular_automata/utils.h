#pragma once
#include <stdint.h>

static uint32_t get_hex(float r, float g, float b)
{
	uint32_t i = (uint32_t)(r * 255) << 16 | (uint32_t)(g * 255) << 8 | (uint32_t)(b * 255);
	return i;
}

static float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}