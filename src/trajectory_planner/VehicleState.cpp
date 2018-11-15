//
// Created by Marc Neumann on 13.11.18.
//

#include "VehicleState.h"

VehicleState::VehicleState(int id, XYCoord xy, FrenetCoord frenet, XYVelocity velocity) :
id(id), xy(xy), velocity(velocity), frenet(frenet) {

}
