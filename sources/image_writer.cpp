#include "image_writer.h"

#include "os.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

bool write_to_png(const char *filename, uint8_t *pixels, uint32_t width,
                  uint32_t height) {
  auto str_filename = std::string(filename) + ".png";

  return stbi_write_png(str_filename.c_str(), width, height, 3, pixels, 0) == 0
             ? true
             : false;
}

bool write_to_ppm(const char *filename, uint8_t *pixels, uint32_t width,
                  uint32_t height) {

  auto str_filename = std::string(filename) + ".ppm";

  auto out_file = fmt::output_file(str_filename);

  out_file.print("P3\n{} {}\n255\n", width, height);

  for (size_t col = 0; col < width; ++col) {
    for (size_t row = 0; row < height; ++row) {
      size_t index = (col * width + row) * 3;

      out_file.print("{} {} {}\n", pixels[index], pixels[index + 1],
                     pixels[index + 2]);
    }
  }

  out_file.close();

  return true;
}
