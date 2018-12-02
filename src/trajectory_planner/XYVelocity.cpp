//
// Created by Marc Neumann on 13.11.18.
//

#include "XYVelocity.h"
#include <math.h>

XYVelocity::XYVelocity(double x, double y) : x(x), y(y) {

}

double XYVelocity::getSpeed() {
    return sqrt(x*x + y*y);
}
