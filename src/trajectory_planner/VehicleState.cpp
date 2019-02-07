//
// Created by Marc Neumann on 13.11.18.
//

#include "VehicleState.h"
#include "WorldConstants.h"

VehicleState::VehicleState(int id, XYCoord xy, FrenetCoord frenet, XYVelocity velocity) :
id(id), xy(xy), velocity(velocity), frenet(frenet) {

}

int VehicleState::getLane() {
    int car_lane = -1;
    // is it on the same lane we are
    double d = frenet.d;
    // TODO use constants
    //for (int i=0; i<LANE_COUNT; i++) {
    //}
    if ( d > 0 && d < LANE_WIDTH ) {
        car_lane = 0;
    } else if ( d > 4 && d < 8 ) {
        car_lane = 1;
    } else if ( d > 8 && d < 12 ) {
        car_lane = 2;
    }
    return car_lane;
}
