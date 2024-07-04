#include "camera.isph"

#include "hittable.isph"

// #define USE_LOOP

vec3 get_traced_color(int32 max_depth, Ray& r, uniform Sphere spheres[], uniform int32 sphere_count)
{
#ifdef USE_LOOP
    vec3 accum_color = make_vec3(1.0, 1.0, 1.0);
    Ray ray = r;

    for(int32 depth = max_depth; depth >= 0; --depth)
    {
        HitRecord rec;

        if(world_hit(spheres, sphere_count, ray, 0.001, FLT_MAX, rec))
        {
            vec3 unit_dir = unit_vec3(ray.dir);
            float a = 0.5 * (unit_dir.y + 1.0);
            accum_color *= 0.5 * linterp(make_vec3(1.0, 1.0, 1.0), make_vec3(0.5, 0.7, 1.0), a);
        }
        else
        {
            vec3 unit_dir = unit_vec3(ray.dir);
            float a = 0.5 * (unit_dir.y + 1.0);
            accum_color *= linterp(make_vec3(1.0, 1.0, 1.0), make_vec3(0.5, 0.7, 1.0), a);
            break;
        }

        ray = make_ray(rec.pos, rec.normal + random_unit_vector());
    }

    return accum_color;
#else
    if(max_depth <= 0)
    {
        return make_vec3(0, 0, 0);
    }

    HitRecord rec;

    if(world_hit(spheres, sphere_count, r, 0.001, FLT_MAX, rec))
    {
        Ray scattered;
        vec3 attenuation;
        bool is_hit = false;
        
        switch (rec.material.material_type)
        {
            case DIFFUSE:
                is_hit = scatter_lambertian(r, rec, attenuation, scattered);
                break;
            case METAL:
                is_hit = scatter_metal(r, rec, attenuation, scattered);
                break;
            default:
                break;
        }

        if(is_hit)
        {
            return attenuation * get_traced_color(max_depth - 1, scattered, spheres, sphere_count);
        }

        return make_vec3(0.0, 0.0, 0.0);
    }

    vec3 unit_dir = unit_vec3(r.dir);
    float a = 0.5 * (unit_dir.y + 1.0);
    return linterp(make_vec3(1.0, 1.0, 1.0), make_vec3(0.5, 0.7, 1.0), a);
#endif
}

inline float linear2gamma(float linear_component)
{
    if(linear_component > 0.0)
    {
        return sqrt(linear_component);
    }

    return 0.0;
}

export void trace(
    uniform uint8 pixels[], 
    uniform CameraVariables& camera_vars,
    uniform Sphere spheres[],
    uniform int32 sphere_count,
    uniform uint32 image_width, 
    uniform uint32 image_height
)
{
    Camera cam = make_camera(camera_vars, image_width, image_height);

    foreach_tiled(row = 0 ... image_width, col = 0 ... image_height)
    {        
        vec3 color = make_vec3(0, 0, 0);

        for(int32 sample = 0; sample < camera_vars.samples_per_pixel; ++sample)
        {
            Ray r = get_ray(cam, (float)(row), (float)(col));

            color += get_traced_color(camera_vars.ray_max_depth, r, spheres, sphere_count);
        }

        color *= cam.pixel_sample_scale;

        color.r = linear2gamma(color.r);
        color.g = linear2gamma(color.g);
        color.b = linear2gamma(color.b);

        int index = (col * image_width + row) * 3;

        #pragma ignore warning(perf)
        pixels[index] = float_to_srgb8(color.r);
        #pragma ignore warning(perf)
        pixels[index + 1] = float_to_srgb8(color.g);
        #pragma ignore warning(perf)
        pixels[index + 2] = float_to_srgb8(color.b);
    }
}