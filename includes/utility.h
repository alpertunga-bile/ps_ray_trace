#pragma once

namespace ispc {
struct float3;
struct Sphere;
struct Material;
enum eMaterialType;
} // namespace ispc

ispc::float3 make_float3(float x, float y, float z);
ispc::Sphere make_sphere(ispc::float3 center, float radius,
                         ispc::Material material);

ispc::Material make_lambertian_mat(ispc::float3 albedo);
ispc::Material make_metal_mat(ispc::float3 albedo);

ispc::Material make_material(ispc::float3 albedo, ispc::eMaterialType type);