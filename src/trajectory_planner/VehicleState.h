//
// Created by Marc Neumann on 13.11.18.
//

#ifndef PATH_PLANNING_VEHICLESTATE_H
#define PATH_PLANNING_VEHICLESTATE_H

#include "XYCoord.h"
#include "XYVelocity.h"
#include "FrenetCoord.h"

class VehicleState {
public:
    int id;
    XYCoord xy;
    XYVelocity velocity;
    FrenetCoord frenet;
    VehicleState(int id, XYCoord xy, FrenetCoord frenet, XYVelocity velocity);

    int getLane();
};


#endif //PATH_PLANNING_VEHICLESTATE_H
