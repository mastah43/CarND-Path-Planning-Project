//
// Created by Marc Neumann on 20.10.18.
//

#include "Trajectory.h"

Trajectory::Trajectory() : x(std::vector<double>()), y(std::vector<double>()) {}

const std::vector<double> &Trajectory::getX() const {
    return x;
}

const std::vector<double> &Trajectory::getY() const {
    return y;
}

void Trajectory::append(const double &x, const double &y) {
    Trajectory::x.push_back(x);
    Trajectory::y.push_back(y);
}

