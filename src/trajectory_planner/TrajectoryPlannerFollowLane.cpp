//
// Created by Marc Neumann on 10.11.18.
//

#include "TrajectoryPlannerFollowLane.h"

#define WAYPOINTS_COUNT 50

#define WAYPOINT_STEP_TIME_MS 20
#define LANE_COUNT 6
#define LANE_DIRECTION_COUNT 3
#define LANE_WIDTH_METERS 4


TrajectoryPlannerFollowLane::TrajectoryPlannerFollowLane(Map &map) : TrajectoryPlanner(map) {
}

const Trajectory
TrajectoryPlannerFollowLane::planTrajectory(const EgoVehicleState &egoState, const SensorFusionResult &sensorFusion,
                                  const TrajectoryFrenetEnd &trajectoryPrevious) {

    // TODO use previous trajectory

    double speed = 0;
    double time = 0;
    double accelerationMax = 6;
    double accelerationMaxTime = 3000;
    double acceleration = 0;
    double speedMax = 100/3.6;
    Trajectory trajectory;
    Map map = TrajectoryPlanner::getMap();
    FrenetCoord waypoint(0, 0);
    for (int i = 0; i < WAYPOINTS_COUNT; i++) {
        double timeStep = WAYPOINT_STEP_TIME_MS;
        acceleration = fmin(accelerationMax, acceleration + (accelerationMax*timeStep/accelerationMaxTime));
        speed = fmin(speedMax, timeStep*acceleration);
        waypoint.incS(timeStep/1000. * speed);
        const XYCoord xy = map.getXY(waypoint);
        // TODO xy calculation wrong
        trajectory.append(xy);

        time += timeStep;
    }

    return trajectory;
}
