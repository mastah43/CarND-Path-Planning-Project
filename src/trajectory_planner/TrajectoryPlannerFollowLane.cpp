//
// Created by Marc Neumann on 10.11.18.
//

#include "TrajectoryPlannerFollowLane.h"

#include <iostream>
#include "../spline/spline.h"

#define WAYPOINTS_COUNT 50

#define WAYPOINT_STEP_TIME_MS 20
#define WAYPOINT_HORICON_DISTANCE 30
#define LANE_COUNT 6
#define LANE_DIRECTION_COUNT 3
#define LANE_WIDTH_METERS 4
#define VELOCITY_DESIRED_KMH 49.5


TrajectoryPlannerFollowLane::TrajectoryPlannerFollowLane(Map &map) : TrajectoryPlanner(map) {
}

const Trajectory
TrajectoryPlannerFollowLane::planTrajectory(EgoVehicleState &egoState, const SensorFusionResult &sensorFusion,
                                  const TrajectoryFrenetEnd &trajectoryPrevious) {

    // TODO use previous trajectory

    double speed = egoState.getSpeed();
    Map map = TrajectoryPlanner::getMap();

    std::cout << "========= planTrajectory" << std::endl;
    std::cout << "ego speed: " << speed << " m/s" << std::endl;
    std::cout << "ego yaw: " << egoState.getYaw() << std::endl;
    std::cout << "ego pos xy: " << egoState.getXY() << std::endl;
    std::cout << "ego pos frenet : " << egoState.getFrenet() << std::endl;
    trajectoryPrevious.cout("trajectory previous");

    EgoVehicleState egoStateNew;
    Trajectory trajectoryAnchor;
    egoStateNew = applyPreviousTrajectory(egoState, trajectoryPrevious, trajectoryAnchor);
    std::cout << "ego pos frenet new: " << egoStateNew.getFrenet() << std::endl;
    std::cout << "ego pos xy new: " << egoStateNew.getXY() << std::endl;

    // add previous trajectory
    Trajectory trajectoryActual;
    for (int i=0; i<trajectoryPrevious.size(); i++) {
        trajectoryActual.append(trajectoryPrevious.getAt(i));
    }

    // TODO extract method with point distance and point count parameters
    FrenetCoord waypoint(egoStateNew.getFrenet());
    for (int i=0; i<3; i++) {
        waypoint.incS(30);
        const XYCoord xy = map.getXY(waypoint);
        trajectoryAnchor.append(xy);
    }
    trajectoryAnchor.cout("trajectory anchor points");
    /*
    double time = 0;
    double accelerationMax = 6;
    double accelerationMaxTime = 3000;
    double acceleration = 3;
    double speedMax = 100/3.6;
    for (int i = 0; i < WAYPOINTS_COUNT; i++) {
        double timeStep = WAYPOINT_STEP_TIME_MS;
        // TODO acceleration = fmin(accelerationMax, acceleration + (accelerationMax*timeStep/accelerationMaxTime));
        speed += timeStep/1000.*acceleration;
        speed = fmin(speedMax, speed);
        waypoint.incS(timeStep/1000. * speed);

        const XYCoord xy = map.getXY(waypoint);
        trajectoryAnchor.append(xy);

        time += timeStep;
    }
    */

    // transform from world to vehicle coordinate system
    // TODO yaw in egoStateNew was not updated above
    trajectoryAnchor.transformToLocal(egoStateNew.getXY(), egoStateNew.getYaw());
    trajectoryAnchor.cout("trajectory anchor points transformed");

    // TODO determine speed right before new trajectory points based on trajectory

    tk::spline trajectorySpline;
    auto trajectoryAnchorX = trajectoryAnchor.getX();
    auto trajectoryAnchorY = trajectoryAnchor.getY();
    trajectorySpline.set_points(trajectoryAnchorX, trajectoryAnchorY);
    int waypointsNewCount = WAYPOINTS_COUNT - trajectoryActual.size();
    double targetX = WAYPOINT_HORICON_DISTANCE;
    XYCoord targetXY = XYCoord(targetX, trajectorySpline(targetX));
    double targetDistance = targetXY.distanceToOrigin();
    double stepDistance = VELOCITY_DESIRED_KMH/3.6*WAYPOINT_STEP_TIME_MS/1000.;
    double steps = targetDistance/stepDistance;
    double waypointX = 0;
    double waypointXStep = targetX/steps;
    std::cout << "adding " << waypointsNewCount << " new waypoints to previous trajectory" << std::endl;
    std::cout << "waypoint x step " << waypointXStep << std::endl;

    // TODO somehow on the second call to planTrajectory the resulting new pos has x=906

    for (int i=0; i<waypointsNewCount; i++) {
        waypointX += waypointXStep;
        // TODO handle wrap around track
        XYCoord xy(waypointX, trajectorySpline(waypointX));
        xy.transformToGlobal(egoStateNew.getXY(), egoStateNew.getYaw());
        trajectoryActual.append(xy);
    }

    trajectoryActual.cout("trajectory");

    return trajectoryActual;
}

EgoVehicleState TrajectoryPlannerFollowLane::applyPreviousTrajectory(EgoVehicleState &egoState,
                                                         const TrajectoryFrenetEnd &trajectoryPrevious,
                                                         Trajectory &trajectory) const {
    EgoVehicleState egoStateNewTrajectory(egoState);
    if (trajectoryPrevious.size() < 2) {
        std::cout << "creating trajectory from scratch" << std::endl;
        XYCoord xyEgoPrev(egoState.getXY());
        xyEgoPrev.moveYaw(egoState.getYaw(), -1);
        trajectory.append(xyEgoPrev);
        trajectory.append(egoState.getXY());
    } else {
        std::cout << "using previous trajectory" << std::endl;
        const XYCoord &xyPrev = trajectoryPrevious.getAt(trajectoryPrevious.size()-2);
        const XYCoord &xyLast = trajectoryPrevious.getAt(trajectoryPrevious.size()-1);
        egoStateNewTrajectory.setYaw(xyPrev.headingTo(xyLast));
        // TODO set correct frenet - implementation of getFrenet missing
        egoStateNewTrajectory.setPos(xyLast, getMap().getFrenet(xyLast, egoStateNewTrajectory.getYaw()));

        trajectory.append(xyPrev);
        trajectory.append(xyLast);
    }

    return egoStateNewTrajectory;
}

