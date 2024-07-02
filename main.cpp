#include "os.h"

#include "ispc_shaders/tracer.h"

constexpr size_t WIDTH = 256;
constexpr size_t HEIGHT = 256;

int main() {
  float pixels[WIDTH * HEIGHT * 3] = {0};

  ispc::calculate_colors(pixels, WIDTH, HEIGHT);

  auto out_file = fmt::output_file("test.ppm");

  out_file.print("P3\n{} {}\n255\n", WIDTH, HEIGHT);

  for (size_t col = 0; col < WIDTH; ++col) {
    for (size_t row = 0; row < HEIGHT; ++row) {
      size_t index = col * WIDTH + row;

      auto r = pixels[index * 3];
      auto g = pixels[index * 3 + 1];
      auto b = 0.0;

      int ir = int(255.999 * r);
      int ig = int(255.999 * g);
      int ib = int(255.999 * b);

      out_file.print("{} {} {}\n", ir, ig, ib);
    }
  }

  out_file.close();
}