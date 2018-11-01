//
// Created by Marc Neumann on 27.10.18.
//

#include "TrajectoryFrenetEnd.h"

const FrenetCoord &TrajectoryFrenetEnd::getEnd() const {
    return end;
}

void TrajectoryFrenetEnd::setFrenetEnd(double s, double d) {
    end.d = d;
    end.s = s;
}

TrajectoryFrenetEnd::TrajectoryFrenetEnd(FrenetCoord &end) : end(end) {

}
