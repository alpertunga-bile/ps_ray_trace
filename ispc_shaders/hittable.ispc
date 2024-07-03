#include "hittable.isph"

bool world_hit(uniform Sphere spheres[], uniform int32 sphere_count, const Ray& r, float ray_tmin, float ray_tmax, HitRecord& hit_record)
{
    HitRecord temp_rec;
    bool is_hit = false;
    float closest_hit = ray_tmax;

    for(int32 sphere_index = 0; sphere_index < sphere_count; ++sphere_index)
    {
        const Sphere sphere = spheres[sphere_index];

        if(hit_sphere(sphere, r, ray_tmin, closest_hit, temp_rec))
        {
            is_hit = true;
            closest_hit = temp_rec.t;
            hit_record = temp_rec;
        }
    }
    
    return is_hit;
}