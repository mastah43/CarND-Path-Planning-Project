//
// Created by Marc Neumann on 20.10.18.
//

#include "Trajectory.h"
#include <iostream>

Trajectory::Trajectory() : xy(std::vector<XYCoord>()) {}

const std::vector<double> Trajectory::getX() const {
    std::vector<double> x;
    for (XYCoord c : xy) {
        x.push_back(c.x);
    }
    return x;
}

const std::vector<double> Trajectory::getY() const {
    std::vector<double> y;
    for (XYCoord c : xy) {
        y.push_back(c.y);
    }
    return y;
}

void Trajectory::append(double x, double y) {
    Trajectory::append(XYCoord(x, y));
}

void Trajectory::append(XYCoord xy) {
    Trajectory::xy.push_back(xy);
}

unsigned int Trajectory::size() const {
    return (int)Trajectory::xy.size();
}

XYCoord Trajectory::getAt(int index) const {
    return Trajectory::xy[index];
}

void Trajectory::transform(double tx, double ty, double tyaw) {
    for (int i=0; i<size(); i++) {
        Trajectory::xy[i].transform(tx, ty, tyaw);
    }
}

const std::vector<XYCoord> &Trajectory::getXY() const {
    return xy;
}

void Trajectory::cout(std::string msg) const {
    std::cout << msg << ": ";
    for (int i=0; i<Trajectory::size(); i++) {
        XYCoord xy = Trajectory::getAt(i);
        xy.cout();
        std::cout << ",";
    }
    std::cout << std::endl;
}

