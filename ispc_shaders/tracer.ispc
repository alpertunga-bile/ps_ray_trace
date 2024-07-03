#include "camera.isph"

vec3 get_traced_color(Ray& r)
{
    vec3 unit_dir = unit_vec3(r.dir);

    float a = 0.5 * (unit_dir.y + 1.0);

    return linterp(make_vec3(1.0, 1.0, 1.0), make_vec3(0.3, 0.3, 1.0), a);
}

export void trace(
    uniform uint8 pixels[], 
    uniform uint32 image_width, 
    uniform uint32 image_height,
    uniform float viewport_width,
    uniform float viewport_height
)
{
    Camera cam = make_camera(1.0, make_vec3(0.0, 0.0, 0.0), image_width, image_height, viewport_width, viewport_height);

    foreach_tiled(row = 0 ... image_width, col = 0 ... image_height)
    {        
        Ray r = get_ray(cam, row, col);

        vec3 color = get_traced_color(r);

        int index = (col * image_width + row) * 3;

        #pragma ignore warning(perf)
        pixels[index] = float_to_srgb8(color.r);
        #pragma ignore warning(perf)
        pixels[index + 1] = float_to_srgb8(color.g);
        #pragma ignore warning(perf)
        pixels[index + 2] = float_to_srgb8(color.b);
    }
}