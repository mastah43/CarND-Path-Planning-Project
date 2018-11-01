//
// Created by Marc Neumann on 02.11.18.
//

#ifndef PATH_PLANNING_TRIGONOMETRY_H
#define PATH_PLANNING_TRIGONOMETRY_H

#include <math.h>

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }


#endif //PATH_PLANNING_TRIGONOMETRY_H
