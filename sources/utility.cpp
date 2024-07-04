#include "utility.h"

#include "tracer.h"

ispc::float3 make_float3(float x, float y, float z) {
  ispc::float3 vec;

  vec.v[0] = x;
  vec.v[1] = y;
  vec.v[2] = z;

  return vec;
}

ispc::Sphere make_sphere(ispc::float3 center, float radius,
                         ispc::Material material) {
  ispc::Sphere sphere;

  sphere.center = center;
  sphere.radius = radius;
  sphere.material = material;

  return sphere;
}

ispc::Material make_lambertian_mat(ispc::float3 albedo) {
  return make_material(albedo, 1.0f, ispc::eMaterialType::DIFFUSE);
}

ispc::Material make_metal_mat(ispc::float3 albedo, float fuzz) {
  return make_material(albedo, fuzz, ispc::eMaterialType::METAL);
}

ispc::Material make_material(ispc::float3 albedo, float fuzz,
                             ispc::eMaterialType type) {
  ispc::Material material;

  material.albedo = albedo;
  material.fuzz = fuzz;
  material.material_type = type;

  return material;
}
