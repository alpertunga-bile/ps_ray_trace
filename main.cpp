#include "ispc_shaders/tracer.h"

#include "image_writer.h"
#include "utility.h"

#include <filesystem>

int main() {
  // ----------------------------------------------------------------------------------------
  // -- Configurable Parameters
  // ideal ratio
  constexpr float aspect_ratio = 16.0f / 9.0f;
  constexpr uint32_t image_height = 400;
  constexpr float viewport_height = 2.0f;

  // ----------------------------------------------------------------------------------------
  // -- Image & Viewport Calculations
  constexpr uint32_t image_width = uint32_t(image_height * aspect_ratio);
  constexpr size_t total_values = image_width * image_height * 3;

  constexpr float image_aspect_ratio = float(image_width) / float(image_height);
  constexpr float viewport_width = viewport_height * image_aspect_ratio;

  // ----------------------------------------------------------------------------------------
  // -- Tracing
  uint8_t pixels[total_values] = {0};

  ispc::CameraVariables cam_vars;
  cam_vars.focal_length = 1.0f;
  cam_vars.origin = make_float3(0.0f, 0.0f, 0.0f);
  cam_vars.viewport_width = viewport_width;
  cam_vars.viewport_height = viewport_height;

  ispc::trace(pixels, cam_vars, image_width, image_height);

  // ----------------------------------------------------------------------------------------
  // -- Write the outputs

  constexpr const char *filename = "test";

  std::string filepath = (std::filesystem::path("outputs") / filename).string();

  write_to_png(filepath.c_str(), pixels, image_width, image_height);
  write_to_ppm(filepath.c_str(), pixels, image_width, image_height);
}