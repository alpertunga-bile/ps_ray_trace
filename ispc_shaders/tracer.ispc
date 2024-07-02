#include "vec3.isph"

export void calculate_pixels(uniform float pixels[], uniform int width, uniform int height)
{
    foreach_tiled(row = 0 ... width, col = 0 ... height )
    {
        int index = col * width + row;

        #pragma ignore warning(perf)
        pixels[index * 3] = (float)(row) * rcp((uniform float)(width - 1));
        #pragma ignore warning(perf)
        pixels[index * 3 + 1] = (float)(col) * rcp((uniform float)(height - 1));
        #pragma ignore warning(perf)
        pixels[index * 3 + 2] = 0.0;
    }
}

export void calculate_colors(uniform float pixels[], uniform int colors[], uniform int size)
{
    
    foreach(index = 0 ... size)
    {
        colors[index] = float_to_srgb8(pixels[index]);
    }
    
}