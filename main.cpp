#include "ispc_shaders/tracer.h"

#include "image_writer.h"
#include "utility.h"

#include <vector>

#include "os.h"

int main() {
  // ----------------------------------------------------------------------------------------
  // -- Configurable Parameters
  constexpr const char *filename = "hittables";

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

  std::vector<ispc::Sphere> spheres;
  spheres.push_back(make_sphere(make_float3(0, 0, -1), 0.5));
  spheres.push_back(make_sphere(make_float3(0, -100.5, -1), 100));

  ispc::trace(pixels, cam_vars, spheres.data(), spheres.size(), image_width,
              image_height);

  // ----------------------------------------------------------------------------------------
  // -- Write the outputs

  std::string filepath = fmt::format("outputs/{}", filename);

  write_to_png(filepath.c_str(), pixels, image_width, image_height);
  write_to_ppm(filepath.c_str(), pixels, image_width, image_height);
}