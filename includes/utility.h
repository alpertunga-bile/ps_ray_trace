#pragma once

namespace ispc {
struct float3;
struct Sphere;
} // namespace ispc

ispc::float3 make_float3(float x, float y, float z);
ispc::Sphere make_sphere(ispc::float3 center, float radius);