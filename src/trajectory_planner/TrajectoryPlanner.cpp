//
// Created by Marc Neumann on 20.10.18.
//

#include "TrajectoryPlanner.h"
#include <iostream>
#include "../spline/spline.h"
#include "../Trigonometry.h"
#include "WorldConstants.h"



TrajectoryPlanner::TrajectoryPlanner(const Map &map) : map(map), lane(1) {
}

const Trajectory
TrajectoryPlanner::planTrajectory(EgoVehicleState &ego, const SensorFusionResult &sensorFusion,
                                            const TrajectoryFrenetEnd &trajectoryPrevious) {

    Map map = TrajectoryPlanner::map;

    /*
    std::cout << "========= planTrajectory" << std::endl;
    std::cout << "ego speed: " << ego.getSpeed() << " m/s" << std::endl;
    std::cout << "ego yaw: " << rad2deg(ego.getYaw()) << "°" << std::endl;
    std::cout << "ego pos xy: " << ego.getXY() << std::endl;
    std::cout << "ego pos frenet : " << ego.getFrenet() << std::endl;
    std::cout << "trajectory previous: " << trajectoryPrevious << std::endl;
    std::cout << "distance to prev waypoint: " << ego.getXY().distanceTo(map.getPrevWaypointByFrenetS(ego.getFrenet().s).xy) << std::endl;
    std::cout << "distance to next waypoint: " << ego.getXY().distanceTo(map.getNextWaypoint(ego.getXY(), ego.getYaw()).xy) << std::endl;
     */

    EgoVehicleState egoRef;
    Trajectory trajectoryAnchor;

    // determine ego state at the end of the previous trajectory
    egoRef = applyPreviousTrajectory(ego, trajectoryPrevious, trajectoryAnchor);
    /*
    std::cout << "new ego pos frenet: " << egoRef.getFrenet() << std::endl;
    std::cout << "new ego pos xy: " << egoRef.getXY() << std::endl;
    std::cout << "new ego yaw: " << rad2deg(egoRef.getYaw()) << "°" << std::endl;
     */

    // Prediction: see if other cars are in the way at the end of our previous trajectory
    double secondsFuture = trajectoryPrevious.getDurationSecs();
    CarPrediction carPrediction = predictCars(sensorFusion, egoRef, secondsFuture);

    // Behavior: set target land and determine speed
    double speedChangeStep = 0;
    if (carPrediction.carAhead) { // Car ahead
        if (!carPrediction.carLeft && lane > 0) {
            // if there is no car left and there is a left lane.
            lane--; // Change lane left.
        } else if (!carPrediction.carRight && lane != 2) {
            // if there is no car right and there is a right lane.
            lane++; // Change lane right.
        } else {
            speedChangeStep -= MAX_ACC;
        }
    } else {
        if (lane != 1) { // if we are not on the center lane.
            if ((lane == 0 && !carPrediction.carRight) || (lane == 2 && !carPrediction.carLeft)) {
                lane = 1; // Back to center.
            }
        }
        if (egoRef.getSpeed() < SPEED_MAX_MPH) {
            speedChangeStep += MAX_ACC;
        }
    }

    addAnchorPointsForTrajectory(egoRef, trajectoryAnchor);

    trajectoryAnchor.transformToLocal(egoRef.getXY(), egoRef.getYaw());
    std::cout << "new trajectory anchor points global: " << trajectoryAnchor << std::endl;

    // add previous trajectory
    Trajectory trajectoryActual;
    for (int i = 0; i < trajectoryPrevious.size(); i++) {
        trajectoryActual.append(trajectoryPrevious.getAt(i));
    }

    // add new waypoints to trajectory
    tk::spline trajectorySpline;
    trajectorySpline.set_points(trajectoryAnchor.getX(), trajectoryAnchor.getY());

    int waypointsNewCount = WAYPOINTS_COUNT - trajectoryActual.size();
    double targetX = WAYPOINT_HORICON_DISTANCE;
    XYCoord targetXY = XYCoord(targetX, trajectorySpline(targetX));
    double targetDistance = targetXY.distanceToOrigin();
    double waypointX = 0;
    std::cout << "adding " << waypointsNewCount << " new waypoints to previous trajectory  for distance "
              << targetDistance << std::endl;

    double speedMPH = egoRef.getSpeed();
    for (int i = 0; i < waypointsNewCount; i++) {
        speedMPH += speedChangeStep;
        if (speedMPH > SPEED_MAX_MPH) {
            speedMPH = SPEED_MAX_MPH;
        } else if (speedMPH < MAX_ACC) {
            speedMPH = MAX_ACC;
        }

        double stepDistance = WAYPOINT_STEP_TIME_SECS * speedMPH / SPEED_MS_TO_MPH;
        double N = targetDistance / stepDistance;
        double waypointXStep = targetX / N;
        waypointX += waypointXStep;
        XYCoord xy(waypointX, trajectorySpline(waypointX));
        xy.transformToGlobal(egoRef.getXY(), egoRef.getYaw());
        trajectoryActual.append(xy);
    }

    std::cout << "trajectory result: " << trajectoryActual << std::endl;

    return trajectoryActual;
}

CarPrediction TrajectoryPlanner::predictCars(const SensorFusionResult &sensorFusion, const EgoVehicleState &egoRef,
                                             double secondsFuture) const {
    CarPrediction carPrediction;
    carPrediction.carAhead = false;
    carPrediction.carLeft = false;
    carPrediction.carRight = false;
    double egoRefS = egoRef.getFrenet().s;
    for (VehicleState vehicle : sensorFusion.vehicles) {
        int carLane = vehicle.getLane();
        if (carLane < 0) {
            // vehicle not on our side of the road
            continue;
        }

        double vehicleSpeed = vehicle.velocity.getSpeed();
        double vehicleRefS = vehicle.frenet.s;

        vehicleRefS += secondsFuture * vehicleSpeed;

        if (carLane == lane) {
            carPrediction.carAhead |= vehicleRefS > egoRefS && vehicleRefS - egoRefS < KEEP_DISTANCE_METERS;
        } else if (carLane - lane == -1) {
            carPrediction.carLeft |= egoRefS - KEEP_DISTANCE_METERS < vehicleRefS && egoRefS + KEEP_DISTANCE_METERS > vehicleRefS;
        } else if (carLane - lane == 1) {
            carPrediction.carRight |= egoRefS - KEEP_DISTANCE_METERS < vehicleRefS && egoRefS + KEEP_DISTANCE_METERS > vehicleRefS;
        }
    }
    return carPrediction;
}

void TrajectoryPlanner::addAnchorPointsForTrajectory(const EgoVehicleState &egoRef,
                                                               Trajectory &trajectoryAnchor) const {
    double targetD = map.getFrenetDeviationForLane(lane);
    FrenetCoord waypoint(egoRef.getFrenet().s, targetD);
    for (int i = 0; i < 3; i++) {
        waypoint.incS(30);
        const XYCoord xy = map.getXY(waypoint);
        trajectoryAnchor.append(xy);
    }
    std::__1::cout << "new trajectory anchor points local: " << trajectoryAnchor << std::__1::endl;
}

EgoVehicleState TrajectoryPlanner::applyPreviousTrajectory(EgoVehicleState &egoState,
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
        const XYCoord &xyPrev = trajectoryPrevious.getAt(trajectoryPrevious.size() - 2);
        const XYCoord &xyLast = trajectoryPrevious.getAt(trajectoryPrevious.size() - 1);
        egoStateNewTrajectory.setYaw(xyPrev.headingTo(xyLast));
        egoStateNewTrajectory.setPos(xyLast, map.getFrenet(xyLast, egoStateNewTrajectory.getYaw()));

        double speedMeterSecond = xyPrev.distanceTo(xyLast) / WAYPOINT_STEP_TIME_SECS;
        double speedMilesHour = speedMeterSecond * SPEED_MS_TO_MPH;
        egoStateNewTrajectory.setSpeed(speedMilesHour);

        trajectory.append(xyPrev);
        trajectory.append(xyLast);
    }

    return egoStateNewTrajectory;
}



