#include "ispc_shaders/tracer.h"

#include "image_writer.h"
#include "utility.h"

#include <vector>

#include "os.h"

int main() {
  // ----------------------------------------------------------------------------------------
  // -- Configurable Parameters
  constexpr const char *filename = "metal";

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
  // distance between the camera and the viewport
  cam_vars.focal_length = 1.0f;
  cam_vars.origin = make_float3(0.0f, 0.0f, 0.0f);
  cam_vars.viewport_width = viewport_width;
  cam_vars.viewport_height = viewport_height;
  cam_vars.samples_per_pixel = 100;
  cam_vars.ray_max_depth = 50;

  ispc::Material material_ground =
      make_lambertian_mat(make_float3(0.8, 0.8, 0.0));
  ispc::Material material_center =
      make_lambertian_mat(make_float3(0.1, 0.2, 0.5));
  ispc::Material material_left = make_metal_mat(make_float3(0.8, 0.8, 0.8));
  ispc::Material material_right = make_metal_mat(make_float3(0.8, 0.6, 0.2));

  std::vector<ispc::Sphere> spheres;
  spheres.push_back(
      make_sphere(make_float3(0.0, -100.5, -1.0), 100.0, material_ground));
  spheres.push_back(
      make_sphere(make_float3(0.0, 0.0, -1.2), 0.5, material_center));
  spheres.push_back(
      make_sphere(make_float3(-1.0, 0.0, -1.0), 0.5, material_left));
  spheres.push_back(
      make_sphere(make_float3(1.0, 0.0, -1.0), 0.5, material_right));

  ispc::trace(pixels, cam_vars, spheres.data(), spheres.size(), image_width,
              image_height);

  // ----------------------------------------------------------------------------------------
  // -- Write the outputs

  std::string filepath = fmt::format("outputs/{}", filename);

  write_to_png(filepath.c_str(), pixels, image_width, image_height);
  write_to_ppm(filepath.c_str(), pixels, image_width, image_height);
}