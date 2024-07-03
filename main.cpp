#include "ispc_shaders/tracer.h"

#include "image_writer.h"

#include <filesystem>

int main() {
  // ideal ratio
  constexpr float aspect_ratio = 16.0f / 9.0f;
  constexpr uint32_t image_height = 400;
  constexpr float viewport_height = 2.0f;

  constexpr uint32_t image_width = uint32_t(image_height * aspect_ratio);
  constexpr size_t total_values = image_width * image_height * 3;

  constexpr float image_aspect_ratio = float(image_width) / float(image_height);
  constexpr float viewport_width = viewport_height * image_aspect_ratio;

  constexpr const char *filename = "test";

  std::string filepath = (std::filesystem::path("outputs") / filename).string();

  uint8_t pixels[total_values] = {0};

  ispc::trace(pixels, image_width, image_height, viewport_width,
              viewport_height);

  write_to_png(filepath.c_str(), pixels, image_width, image_height);
  write_to_ppm(filepath.c_str(), pixels, image_width, image_height);
}