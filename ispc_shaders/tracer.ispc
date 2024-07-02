#include "vec3.isph"

export void calculate_colors(uniform float pixels[], uniform int width, uniform int height)
{

    foreach(row = 0 ... width, col = 0 ... height )
    {
        int index = col * width + row;

        pixels[index * 3] = (float)(row) * rcp((uniform float)(width - 1));
        pixels[index * 3 + 1] = (float)(col) * rcp((uniform float)(height - 1));
        pixels[index * 3 + 2] = 0.0;
    }
}