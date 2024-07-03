#include "utility.h"

#include "tracer.h"

ispc::float3 make_float3(float x, float y, float z) {
  ispc::float3 vec;

  vec.v[0] = x;
  vec.v[1] = y;
  vec.v[2] = z;

  return vec;
}

ispc::Sphere make_sphere(ispc::float3 center, float radius) {
  ispc::Sphere sphere;

  sphere.center = center;
  sphere.radius = radius;

  return sphere;
}
