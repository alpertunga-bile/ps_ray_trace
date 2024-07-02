#include "os.h"

#include "ispc_shaders/tracer.h"

constexpr size_t WIDTH = 256;
constexpr size_t HEIGHT = 256;
constexpr size_t TOTAL_VALUES = WIDTH * HEIGHT * 3;

int main() {
  float pixels[TOTAL_VALUES];
  int32_t colors[TOTAL_VALUES];

  ispc::calculate_pixels(pixels, WIDTH, HEIGHT);
  ispc::calculate_colors(pixels, colors, TOTAL_VALUES);

  auto out_file = fmt::output_file("test.ppm");

  out_file.print("P3\n{} {}\n255\n", WIDTH, HEIGHT);

  for (size_t col = 0; col < WIDTH; ++col) {
    for (size_t row = 0; row < HEIGHT; ++row) {
      size_t index = col * WIDTH + row;

      out_file.print("{} {} {}\n", colors[index], colors[index + 1],
                     colors[index + 2]);
    }
  }

  out_file.close();
}