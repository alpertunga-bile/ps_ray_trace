#pragma once

#include <cstdint>

bool write_to_png(const char *filename, uint8_t *pixels, uint32_t width,
                  uint32_t height);

bool write_to_ppm(const char *filename, uint8_t *pixels, uint32_t width,
                  uint32_t height);