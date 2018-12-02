//
// Created by Marc Neumann on 10.11.18.
//

#include "TrajectoryPlannerFollowLane.h"

#include <iostream>
#include "../spdlog/spdlog.h"
#include "../spdlog/sinks/stdout_color_sinks.h"
#include "../spline/spline.h"
#include "../Trigonometry.h"
#include "WorldConstants.h"


TrajectoryPlannerFollowLane::TrajectoryPlannerFollowLane(const Map &map) : TrajectoryPlanner(map), lane(1) {
}

const Trajectory
TrajectoryPlannerFollowLane::planTrajectory(EgoVehicleState &ego, const SensorFusionResult &sensorFusion,
                                  const TrajectoryFrenetEnd &trajectoryPrevious) {

    double speed = ego.getSpeed();
    Map map = TrajectoryPlanner::getMap();

    // TODO auto console = spdlog::stdout_color_mt("console");

    std::cout << "========= planTrajectory" << std::endl;
    std::cout << "ego speed: " << speed << " m/s" << std::endl;
    std::cout << "ego yaw: " << rad2deg(ego.getYaw()) << "°" << std::endl;
    std::cout << "ego pos xy: " << ego.getXY() << std::endl;
    std::cout << "ego pos frenet : " << ego.getFrenet() << std::endl;
    std::cout << "trajectory previous: " << trajectoryPrevious << std::endl;
    std::cout << "distance to prev waypoint: " << ego.getXY().distanceTo(map.getPrevWaypointByFrenetS(ego.getFrenet().s).xy) << std::endl;
    std::cout << "distance to next waypoint: " << ego.getXY().distanceTo(map.getNextWaypoint(ego.getXY(), ego.getYaw()).xy) << std::endl;

    EgoVehicleState egoRef;
    Trajectory trajectoryAnchor;
    egoRef = applyPreviousTrajectory(ego, trajectoryPrevious, trajectoryAnchor);
    std::cout << "new ego pos frenet: " << egoRef.getFrenet() << std::endl;
    std::cout << "new ego pos xy: " << egoRef.getXY() << std::endl;
    std::cout << "new ego yaw: " << rad2deg(egoRef.getYaw()) << "°" << std::endl;

    // Prediction : Analysing other cars positions.
    bool carAhead = false;
    bool carLeft = false;
    bool carRight = false;
    double egoRefS = egoRef.getFrenet().s;
    for (VehicleState vehicle : sensorFusion.vehicles) {
        int carLane = vehicle.getLane();
        if (carLane < 0) {
            // vehicle not on our side of the road
            continue;
        }

        double vehicleSpeed = vehicle.velocity.getSpeed();
        double vehicleRefS = vehicle.frenet.s;

        // Estimate car s position after executing previous trajectory.
        vehicleRefS += trajectoryPrevious.getDurationSecs()*vehicleSpeed;

        if ( carLane == lane ) {
            carAhead |= vehicleRefS > egoRefS && vehicleRefS - egoRefS < KEEP_DISTANCE_METERS;
        } else if ( carLane - lane == -1 ) {
            carLeft |= egoRefS - 30 < vehicleRefS && egoRefS + 30 > vehicleRefS;
        } else if ( carLane - lane == 1 ) {
            carRight |= egoRefS - 30 < vehicleRefS && egoRefS + 30 > vehicleRefS;
        }
    }

    // Behavior : Let's see what to do.
    double speedChangeStep = 0;

    if ( carAhead ) { // Car ahead
        if ( !carLeft && lane > 0 ) {
            // if there is no car left and there is a left lane.
            lane--; // Change lane left.
        } else if ( !carRight && lane != 2 ){
            // if there is no car right and there is a right lane.
            lane++; // Change lane right.
        } else {
            speedChangeStep -= MAX_ACC;
        }
    } else {
        if ( lane != 1 ) { // if we are not on the center lane.
            if ( ( lane == 0 && !carRight ) || ( lane == 2 && !carLeft ) ) {
                lane = 1; // Back to center.
            }
        }
        if ( egoRef.getSpeed() < SPEED_MAX_MPH ) {
            speedChangeStep += MAX_ACC;
        }
    }

    // TODO extract method with point distance and point count parameters
    double targetD = map.getFrenetDeviationForLane(lane);
    FrenetCoord waypoint(egoRef.getFrenet().s, targetD);
    for (int i=0; i<3; i++) {
        waypoint.incS(30);
        const XYCoord xy = map.getXY(waypoint);
        trajectoryAnchor.append(xy);
    }
    std::cout << "new trajectory anchor points local: " << trajectoryAnchor << std::endl;

    trajectoryAnchor.transformToLocal(egoRef.getXY(), egoRef.getYaw());
    std::cout << "new trajectory anchor points global: " << trajectoryAnchor << std::endl;

    // add previous trajectory
    Trajectory trajectoryActual;
    for (int i=0; i<trajectoryPrevious.size(); i++) {
        trajectoryActual.append(trajectoryPrevious.getAt(i));
    }

    tk::spline trajectorySpline;
    trajectorySpline.set_points(trajectoryAnchor.getX(), trajectoryAnchor.getY());

    int waypointsNewCount = WAYPOINTS_COUNT - trajectoryActual.size();
    double targetX = WAYPOINT_HORICON_DISTANCE;
    XYCoord targetXY = XYCoord(targetX, trajectorySpline(targetX));
    double targetDistance = targetXY.distanceToOrigin();
    double waypointX = 0;
    std::cout << "adding " << waypointsNewCount << " new waypoints to previous trajectory with size " <<
        trajectoryPrevious.size() << " for distance " << targetDistance << std::endl;

    double speedMPH = egoRef.getSpeed();
    for (int i=0; i<waypointsNewCount; i++) {
        speedMPH += speedChangeStep;
        if ( speedMPH > SPEED_MAX_MPH ) {
            speedMPH = SPEED_MAX_MPH;
        } else if ( speedMPH < MAX_ACC ) {
            speedMPH = MAX_ACC;
        }

        double stepDistance = WAYPOINT_STEP_TIME_SECS*speedMPH/SPEED_MS_TO_MPH;
        double N = targetDistance/stepDistance;
        double waypointXStep = targetX/N;
        waypointX += waypointXStep;
        XYCoord xy(waypointX, trajectorySpline(waypointX));
        xy.transformToGlobal(egoRef.getXY(), egoRef.getYaw());
        trajectoryActual.append(xy);
    }

    std::cout << "trajectory result: " << trajectoryActual << std::endl;

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
        egoStateNewTrajectory.setPos(xyLast, getMap().getFrenet(xyLast, egoStateNewTrajectory.getYaw()));

        double speedMeterSecond = xyPrev.distanceTo(xyLast)/WAYPOINT_STEP_TIME_SECS;
        double speedMilesHour = speedMeterSecond*SPEED_MS_TO_MPH;
        egoStateNewTrajectory.setSpeed(speedMilesHour);

        trajectory.append(xyPrev);
        trajectory.append(xyLast);
    }

    return egoStateNewTrajectory;
}

