#include "core.h"

#include "ispc_shaders/simple.h"
using namespace ispc;

int main() {
  float vin[16], vout[16];

  for (int i = 0; i < 16; i++) {
    vin[i] = i;
  }

  simple(vin, vout, 16);

  for (int i = 0; i < 16; ++i)
    fmt::println("{}: simple({}) = {}", i, vin[i], vout[i]);
}