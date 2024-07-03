#include "camera.isph"

float hit_sphere(const vec3& center, float radius, const Ray& r)
{
    const vec3 oc = center - r.origin;
    
    float a = length_squared(r.dir);
    float h = dot(r.dir, oc);
    float c = length_squared(oc) - radius * radius;
    float discriminant = h * h - a * c;

    if(discriminant < 0.0)
    {
        return -1.0;
    }
    else
    {
        return (h + sqrt(discriminant)) * rcp(a);
    }
}

vec3 get_traced_color(Ray& r)
{
    float t = hit_sphere(make_vec3(0, 0, -1), 0.5, r);

    if(t > 0.0)
    {
        vec3 N = unit_vec3(ray_at(r, t) - make_vec3(0, 0, -1));
        return 0.5 * make_vec3(N.x + 1, N.y + 1, N.z + 1);
    }

    vec3 unit_dir = unit_vec3(r.dir);
    float a = 0.5 * (unit_dir.y + 1.0);
    return linterp(make_vec3(1.0, 1.0, 1.0), make_vec3(0.5, 0.7, 1.0), a);
}

export void trace(
    uniform uint8 pixels[], 
    uniform CameraVariables& camera_vars,
    uniform uint32 image_width, 
    uniform uint32 image_height
)
{
    Camera cam = make_camera(camera_vars, image_width, image_height);

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