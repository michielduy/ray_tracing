#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>
#include <ostream>

using colour = vec3;

inline void write_colour(std::ostream &out, const colour &pixel_colour) {
  double r = pixel_colour.x();
  double g = pixel_colour.y();
  double b = pixel_colour.z();

  // Translate the [0,1] component values to the byte range [0,255].
  int rbyte = int(255.999 * r);
  int gbyte = int(255.999 * g);
  int bbyte = int(255.999 * b);

  // Write out the pixel color components.
  out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif // !COLOR_H
