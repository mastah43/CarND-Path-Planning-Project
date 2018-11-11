//
// Created by Marc Neumann on 02.11.18.
//

#ifndef PATH_PLANNING_TRIGONOMETRY_H
#define PATH_PLANNING_TRIGONOMETRY_H

#include <math.h>

// For converting back and forth between radians and degrees.
inline constexpr double pi() { return M_PI; }
inline double deg2rad(double x) { return x * pi() / 180; }
inline double rad2deg(double x) { return x * 180 / pi(); }

inline double angleRadDiff(double angle1, double angle2) {
    double angle = fabs(angle1 - angle2);
    return fmin(2 * pi() - angle, angle);
}

inline double anglePerpendicular(double a) {
    return a - pi() / 2;
}

#endif //PATH_PLANNING_TRIGONOMETRY_H
