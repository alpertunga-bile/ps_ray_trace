#include "utility.h"

#include "tracer.h"

ispc::float3 make_float3(float x, float y, float z) {
  ispc::float3 vec;

  vec.v[0] = x;
  vec.v[1] = y;
  vec.v[2] = z;

  return vec;
}
