//
// Created by Marc Neumann on 20.10.18.
//

#include "Trajectory.h"
#include <iostream>
#include <stdexcept>
#include "WorldConstants.h"

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

void Trajectory::transformToLocal(XYCoord origin, double yaw) {
    for (int i=0; i<size(); i++) {
        Trajectory::xy[i].transformToLocal(origin, yaw);
    }
}

const std::vector<XYCoord> &Trajectory::getXY() const {
    return xy;
}

std::ostream & operator<<(std::ostream & str, Trajectory const & t) {
    for (int i=0; i<t.size(); i++) {
        std::cout << t.getAt(i);
        std::cout << ",";
    }
    return str;
}

XYCoord Trajectory::getLast() const {
    if (xy.size() == 0) {
        throw std::bad_function_call( );
    }

    return getAt((unsigned int)(xy.size() - 1));
}

double Trajectory::getDurationSecs() const {
    return size()*WAYPOINT_STEP_TIME_SECS;
}



