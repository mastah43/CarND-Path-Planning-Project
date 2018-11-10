//
// Created by Marc Neumann on 20.10.18.
//

#include "TrajectoryPlanner.h"

#define WAYPOINTS_COUNT 50

static const int MAX_JERK = 10;

static const int MAX_ACCELERATION = 10;

const Trajectory
TrajectoryPlanner::planTrajectory(const EgoVehicleState &egoState, const TrajectoryFrenetEnd &trajectorPrevious) {
    double distInc = 0.5;
    double distX = 0;
    double distY = 0;

    Trajectory trajectory;

    for(int i = 0; i < WAYPOINTS_COUNT; i++) {
        const double speed = (double)i/(double)WAYPOINTS_COUNT;
        const double yawRad = egoState.getYawRad();
        distX += distInc * speed * cos(yawRad);
        distY += distInc * speed * sin(yawRad);
        trajectory.append(egoState.getY()+distY, egoState.getY()+distY);
    }

    return trajectory;
}
