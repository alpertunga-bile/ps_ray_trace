#include "ispc_shaders/tracer.h"

#include "image_writer.h"

#include <filesystem>

int main() {
  constexpr uint32_t WIDTH = 256;
  constexpr uint32_t HEIGHT = 256;
  constexpr size_t TOTAL_VALUES = WIDTH * HEIGHT * 3;
  constexpr const char *FILENAME = "test";

  std::string filepath = (std::filesystem::path("outputs") / FILENAME).string();

  uint8_t pixels[TOTAL_VALUES];

  ispc::trace(pixels, WIDTH, HEIGHT);

  write_to_png(filepath.c_str(), pixels, WIDTH, HEIGHT);
  write_to_ppm(filepath.c_str(), pixels, WIDTH, HEIGHT);
}