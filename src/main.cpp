#include "../lib/toojpeg.h"
#include "colour.h"
#include "ray.h"
#include "vec3.h"
#include <fstream>
#include <ios>
#include <iostream>

colour ray_colour(const ray &r) {
  vec3 unit_direction = unit_vector(r.direction());
  double a = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - a) * colour(1.0, 1.0, 1.0) + a * colour(0.5, 0.7, 1.0);
}

std::ofstream myFile("rendered.jpeg",
                     std::ios_base::out | std::ios_base::binary);
void myOutput(unsigned char byte) { myFile << byte; }

void fill_image_buffer(unsigned char *image, int width_ratio, int height_ratio,
                       int offset, bool test) {
  if (test) {
    // testing with gradient
    image[offset] = width_ratio;
    image[offset + 1] = height_ratio;
    image[offset + 2] = 127;
  }
}
void fill_image_buffer(unsigned char *image, const ray &r, int offset) {
  colour pixel_colour = ray_colour(r);
  // testing with ray colours
  image[offset] = 255 * pixel_colour[0];
  image[offset + 1] = 255 * pixel_colour[1];
  image[offset + 2] = 255 * pixel_colour[2];
}

int main() {
  // IMAGE
  constexpr int IMAGE_WIDTH = 3840;
  constexpr double ASPECT_RATIO = 16.0 / 9.0;
  constexpr int BYTES_PER_PIXEL = 3;
  constexpr bool TEST = true;

  int IMAGE_HEIGHT = int(IMAGE_WIDTH / ASPECT_RATIO);
  IMAGE_HEIGHT = (IMAGE_HEIGHT < 1) ? 1 : IMAGE_HEIGHT;
  unsigned char *image =
      new unsigned char[IMAGE_WIDTH * IMAGE_HEIGHT * BYTES_PER_PIXEL];

  // CAMERA
  constexpr double focal_length = 1.0;
  constexpr double viewport_height = 2.0;
  double viewport_width =
      viewport_height * (double(IMAGE_WIDTH) / IMAGE_HEIGHT);
  const point3 camera_center = point3(0, 0, 0);

  // CALCULATE vectors accross
  const vec3 viewport_u = vec3(viewport_width, 0, 0);
  const vec3 viewport_v = vec3(0, -viewport_height, 0);

  // CALCULATE horizontal and vertical delta from pixel to pixel
  vec3 pixel_delta_u = viewport_u / IMAGE_WIDTH;
  vec3 pixel_delta_v = viewport_v / IMAGE_HEIGHT;

  // CALCULATE upper left pixel
  vec3 viewport_upper_left = camera_center - vec3(0, 0, focal_length) -
                             viewport_u / 2 - viewport_v / 2;
  vec3 pixel100_loc =
      viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

  // RENDER
  for (int j = 0; j < IMAGE_HEIGHT; j++) {
    std::clog << "\rScanlines remaining: " << (IMAGE_HEIGHT - j) << " "
              << std::flush;
    for (int i = 0; i < IMAGE_WIDTH; i++) {
      vec3 pixel_center =
          pixel100_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
      vec3 ray_direction = pixel_center - camera_center;

      ray r(camera_center, ray_direction);
      int offset = (j * IMAGE_WIDTH + i) * BYTES_PER_PIXEL;

      if (!TEST) {
        fill_image_buffer(image, r, offset);
      } else {
        fill_image_buffer(image, 255 * i / IMAGE_WIDTH, 255 * j / IMAGE_HEIGHT,
                          offset, true);
      }
    }
  }
  std::clog << "\rDone.     \n";

  // JPEG WRITING
  std::clog << "\rWriting JPEG\n";
  const bool ISRGB = true; // true = RGB image, else false = grayscale
  const auto QUALITY = 90; // compression quality: 0 = worst, 100 = best, 80 to
                           // 90 are most often used
  const bool DOWNSAMPLE = false; // false = save as YCbCr444 JPEG (better
                                 // quality), true = YCbCr420 (smaller file)
  const char *COMMENT = "TooJpeg example image"; // arbitrary JPEG comment
  auto ok = TooJpeg::writeJpeg(myOutput, image, IMAGE_WIDTH, IMAGE_HEIGHT,
                               ISRGB, QUALITY, DOWNSAMPLE, COMMENT);
  delete[] image;
  return ok ? 1 : 0;
}
