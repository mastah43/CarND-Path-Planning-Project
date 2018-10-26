//
// Created by Marc Neumann on 20.10.18.
//

#include "Trajectory.h"

const std::vector<double> &Trajectory::getX() const {
    return x;
}

const std::vector<double> &Trajectory::getY() const {
    return y;
}

Trajectory::Trajectory(const std::vector<double> &x, const std::vector<double> &y) : x(x), y(y) {}
