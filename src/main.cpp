#include "colour.h"
#include "ray.h"
#include "vec3.h"
#include <iostream>

colour ray_colour(const ray &r) {
  vec3 unit_direction = unit_vector(r.direction());
  double a = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - a) * colour(1.0, 1.0, 1.0) + a * colour(0.5, 0.7, 1.0);
}

int main() {
  // IMAGE

  int IMAGE_WIDTH = 400;
  double ASPECT_RATIO = 16.0 / 9.0;

  int IMAGE_HEIGHT = int(IMAGE_WIDTH / ASPECT_RATIO);
  IMAGE_HEIGHT = (IMAGE_HEIGHT < 1) ? 1 : IMAGE_HEIGHT;

  // CAMERA
  double focal_length = 1.0;
  double viewport_height = 2.0;
  double viewport_width =
      viewport_height * (double(IMAGE_WIDTH) / IMAGE_HEIGHT);
  point3 camera_center = point3(0, 0, 0);

  // CALCULATE vectors accross
  vec3 viewport_u = vec3(viewport_width, 0, 0);
  vec3 viewport_v = vec3(0, -viewport_height, 0);

  // CALCULATE horizontal and vertical delta from pixel to pixel
  vec3 pixel_delta_u = viewport_u / IMAGE_WIDTH;
  vec3 pixel_delta_v = viewport_v / IMAGE_HEIGHT;

  // CALCULATE upper left pixel
  vec3 viewport_upper_left = camera_center - vec3(0, 0, focal_length) -
                             viewport_u / 2 - viewport_v / 2;
  vec3 pixel100_loc =
      viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

  // RENDER

  std::cout << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";

  for (int j = 0; j < IMAGE_HEIGHT; j++) {
    std::clog << "\rScanlines remaining: " << (IMAGE_HEIGHT - j) << " "
              << std::flush;
    for (int i = 0; i < IMAGE_WIDTH; i++) {
      vec3 pixel_center =
          pixel100_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
      vec3 ray_direction = pixel_center - camera_center;

      ray r(camera_center, ray_direction);
      colour pixel_colour = ray_colour(r);
      write_colour(std::cout, pixel_colour);
    }
  }
  std::clog << "\rDone.     \n";
  return 0;
}
