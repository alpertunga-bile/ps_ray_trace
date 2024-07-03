#include "sphere.isph"

bool hit_sphere(const Sphere& sphere, const Ray& r, float ray_tmin, float ray_tmax, HitRecord& hit_record)
{
    const vec3 oc = sphere.center - r.origin;
    
    float a = length_squared(r.dir);
    float h = dot(r.dir, oc);
    float c = length_squared(oc) - sphere.radius * sphere.radius;
    float discriminant = h * h - a * c;

    if(discriminant < 0.0)
    {
        return false;
    }

    float sqrt_disc = sqrt(discriminant);

    float root = (h - sqrt_disc) * rcp(a);

    // check the possibilities
    if(root <= ray_tmin || ray_tmax <= root)
    {
        root = (h + sqrt_disc) * rcp(a);

        if(root <= ray_tmin || ray_tmax <= root)
        {
            return false;
        }
    }

    hit_record.t = root;
    hit_record.pos = ray_at(r, root);
    vec3 outward_normal = (hit_record.pos - sphere.center) * rcp(sphere.radius);
    hit_rec_set_face_normal(hit_record, r, outward_normal);

    return true;
}