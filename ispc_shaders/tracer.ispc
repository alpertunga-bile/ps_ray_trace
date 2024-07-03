#include "vec3.isph"

export void trace(uniform uint8 pixels[], uniform uint32 width, uniform uint32 height)
{
    foreach_tiled(row = 0 ... width, col = 0 ... height )
    {
        int index = (col * width + row) * 3;

        float r = (float)(row) * rcp((uniform float)(width - 1));
        float g = (float)(col) * rcp((uniform float)(height - 1));
        float b = 0.0;

        #pragma ignore warning(perf)
        pixels[index] = float_to_srgb8(r);
        #pragma ignore warning(perf)
        pixels[index + 1] = float_to_srgb8(g);
        #pragma ignore warning(perf)
        pixels[index + 2] = float_to_srgb8(b);
    }
}