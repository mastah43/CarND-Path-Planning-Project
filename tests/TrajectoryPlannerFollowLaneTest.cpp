#include "gtest/gtest.h"
#include "../src/trajectory_planner/TrajectoryPlannerFollowLane.h"
#include "../src/Trigonometry.h"
#include "Verifiers.h"
#include <math.h>

Map buildMapStraight()  {
    MapBuilder mapBuilder(1000);
    for (int i=0; i<10; i++) {
        double s = i*10;
        mapBuilder.addCoord(XYCoord(s,0), FrenetCoord(s,0), 0, -1);
    }
    Map map = *mapBuilder.build();
    return map;
}

Map buildMapCircle(double radius)  {
    int waypointCount = 10;
    double roadLength = radius*2*M_PI;
    MapBuilder mapBuilder(roadLength);
    for (int i=0; i<waypointCount; i++) {
        double angle = i*2*M_PI/waypointCount;
        double x = sin(angle)*radius;
        double y = cos(angle)*radius;
        double s = angle/2*M_PI*roadLength;
        double dx = sin(angle);
        double dy = cos(angle);
        mapBuilder.addCoord(XYCoord(x,y), FrenetCoord(s,0), 0, -1);
    }
    Map map = *mapBuilder.build();
    return map;
}

TrajectoryFrenetEnd moveOnTrajectory(EgoVehicleState &ego, Trajectory &trajectory, const Map &map) {
    unsigned int moveSteps = trajectory.size() / 2;

    double lastS = ego.getFrenet().s;
    for (int i=0; i < moveSteps; i++) {
        const XYCoord &wp = trajectory.getAt(0);    
        const double yawToWP = ego.getXY().headingTo(wp);
        ego.setYaw(yawToWP);
        ego.setPos(wp, map.getFrenet(wp, yawToWP));

        FrenetCoord egoF = ego.getFrenet();
        EXPECT_NEAR(0, egoF.d, 0.001);
        EXPECT_LT(lastS, egoF.s);
        lastS = egoF.s;
    }
    TrajectoryFrenetEnd trajectoryPrev(ego.getFrenet());
    for (int i=moveSteps; i<trajectory.size(); i++) {
        trajectoryPrev.append(trajectory.getAt(i));
    }
    
    return trajectoryPrev;
}

TEST(TrajectoryPlannerFollowLane, PlanStraight) {
    Map map = buildMapStraight();
    TrajectoryPlannerFollowLane planner(map);
    SensorFusionResult sensorFusionResult;
    EgoVehicleState egoState;
    FrenetCoord egoPosF(0,0);
    egoState.setPos(map.getXY(egoPosF), egoPosF);
    TrajectoryFrenetEnd trajectoryPrevious(egoState.getFrenet());
    Trajectory trajectory = planner.planTrajectory(egoState, sensorFusionResult, trajectoryPrevious);
    double lastS = -1;
    for (int i=0; i<trajectory.size(); i++) {
        const XYCoord &c = trajectory.getAt(i);
        const FrenetCoord &f = map.getFrenet(c, egoState.getYaw());
        EXPECT_NEAR(0, f.d, 0.001);
        EXPECT_LT(lastS, f.s);
        lastS = f.s;
    }
    EXPECT_EQ(50, trajectory.size());
}


TEST(TrajectoryPlannerFollowLane, PlanCircle) {
    double roadRadius = 500;
    const Map map = buildMapCircle(roadRadius);
    TrajectoryPlannerFollowLane planner(map);
    SensorFusionResult sensorFusionResult;
    
    EgoVehicleState ego;
    FrenetCoord egoPosF(map.getWaypointAt(0).f);
    ego.setPos(map.getXY(egoPosF), egoPosF);

    const XYCoord mapCircleCenter(0,0);
    TrajectoryFrenetEnd trajectoryPrev(ego.getFrenet());
    for (int i=0; i<1000; i++) {
        Trajectory trajectory = planner.planTrajectory(ego, sensorFusionResult, trajectoryPrev);
        trajectoryPrev = moveOnTrajectory(ego, trajectory, map);
        EXPECT_NEAR(roadRadius, ego.getXY().distanceTo(mapCircleCenter), 0.01);
    }
}



