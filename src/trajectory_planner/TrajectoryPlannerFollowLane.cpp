//
// Created by Marc Neumann on 10.11.18.
//

#include "TrajectoryPlannerFollowLane.h"

#define WAYPOINTS_COUNT 50

TrajectoryPlannerFollowLane::TrajectoryPlannerFollowLane(Map &map) : TrajectoryPlanner(map) {
}

const Trajectory TrajectoryPlannerFollowLane::planTrajectory(const EgoVehicleState &egoState,
                                                             const TrajectoryFrenetEnd &trajectorPrevious) {

    double distInc = 0.5;
    Trajectory trajectory;
    Map map = TrajectoryPlanner::getMap();
    FrenetCoord waypoint(0, 0);
    for (int i = 0; i < WAYPOINTS_COUNT; i++) {
        const double speed = (double) i / (double) WAYPOINTS_COUNT;
        waypoint.incS(distInc * speed);
        const XYCoord xy = map.getXY(waypoint);
        trajectory.append(xy);
    }

    return trajectory;
}
