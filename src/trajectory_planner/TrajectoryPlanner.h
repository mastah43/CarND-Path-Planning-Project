//
// Created by Marc Neumann on 20.10.18.
//

#ifndef PATH_PLANNING_TRAJECTORYPLANNER_H
#define PATH_PLANNING_TRAJECTORYPLANNER_H

#include "Map.h"
#include "EgoVehicleState.h"
#include "Trajectory.h"
#include "SensorFusionResult.h"
#include "TrajectoryFrenetEnd.h"

struct CarPrediction {
    bool carAhead;
    bool carLeft;
    bool carRight;
};

class TrajectoryPlanner {
private:
    const Map map;
    int lane;
    EgoVehicleState applyPreviousTrajectory(EgoVehicleState &egoState, const TrajectoryFrenetEnd &trajectoryPrevious,
                                            Trajectory &trajectory) const;

    void addAnchorPointsForTrajectory(const EgoVehicleState &egoRef, Trajectory &trajectoryAnchor) const;

    CarPrediction predictCars(const SensorFusionResult &sensorFusion, const EgoVehicleState &egoRef, double secondsFuture) const;

public:
    explicit TrajectoryPlanner(const Map &map);

    virtual const Trajectory planTrajectory(EgoVehicleState &egoState,
                                            const SensorFusionResult &sensorFusion,
                                            const TrajectoryFrenetEnd &trajectorPrevious);



};

#endif //PATH_PLANNING_TRAJECTORYPLANNER_H
