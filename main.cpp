#include "os.h"

constexpr size_t WIDTH = 256;
constexpr size_t HEIGHT = 256;

int main() {
  auto out_file = fmt::output_file("test.ppm");

  out_file.print("P3\n{} {}\n255\n", WIDTH, HEIGHT);

  for (size_t j = 0; j < HEIGHT; ++j) {
    for (size_t i = 0; i < WIDTH; ++i) {
      auto r = double(i) / (WIDTH - 1);
      auto g = double(j) / (HEIGHT - 1);
      auto b = 0.0;

      int ir = int(255.999 * r);
      int ig = int(255.999 * g);
      int ib = int(255.999 * b);

      out_file.print("{} {} {}\n", ir, ig, ib);
    }
  }

  out_file.close();
}