#include "gtest/gtest.h"
#include "../src/trajectory_planner/TrajectoryPlanner.h"
#include "../src/Trigonometry.h"
#include "Verifiers.h"
#include "MapTestData.h"
#include <math.h>


TrajectoryFrenetEnd moveOnTrajectory(EgoVehicleState &ego, Trajectory &trajectory, const Map &map) {
    unsigned int moveSteps = trajectory.size() / 2;

    double lastS = ego.getFrenet().s;
    for (int i=0; i < moveSteps; i++) {
        const XYCoord &wp = trajectory.getAt(i);
        const double yawToWP = ego.getXY().headingTo(wp);
        ego.setYaw(yawToWP);
        ego.setPos(wp, map.getFrenet(wp, yawToWP));

        FrenetCoord egoF = ego.getFrenet();
        //EXPECT_NEAR(0, egoF.d, 0.001);
        EXPECT_LT(lastS, egoF.s);
        lastS = egoF.s;
    }
    std::cout << "driven distance: " << ego.getFrenet().s << std::endl;

    TrajectoryFrenetEnd trajectoryPrev(ego.getFrenet());
    for (int i=moveSteps; i<trajectory.size(); i++) {
        trajectoryPrev.append(trajectory.getAt(i));
    }
    
    return trajectoryPrev;
}

TEST(TrajectoryPlanner, PlanStraight) {
    Map map = buildMapStraight(1000, 10);
    TrajectoryPlanner planner(map);
    EgoVehicleState ego;
    FrenetCoord egoPosF = map.getEgoStartPosFrenet();
    ego.setPos(map.getXY(egoPosF), egoPosF);
    TrajectoryFrenetEnd trajectoryPrevious(ego.getFrenet());
    Trajectory trajectory = planner.planTrajectory(ego, SensorFusionResult(), trajectoryPrevious);
    double lastS = -1;
    for (int i=0; i<trajectory.size(); i++) {
        const XYCoord &c = trajectory.getAt(i);
        const FrenetCoord &f = map.getFrenet(c, ego.getYaw());
        EXPECT_NEAR(map.getFrenetDeviationForLane(1), f.d, 0.001);
        EXPECT_LT(lastS, f.s);
        lastS = f.s;
    }
    EXPECT_EQ(50, trajectory.size());
}


TEST(TrajectoryPlanner, MoveOnCircle) {
    double roadRadius = 500;
    const Map map = buildMapCircle(roadRadius, 1000);
    TrajectoryPlanner planner(map);

    EgoVehicleState ego;
    FrenetCoord egoPosF = map.getEgoStartPosFrenet();
    ego.setPos(map.getXY(egoPosF), egoPosF);

    const XYCoord mapCircleCenter(0,0);
    TrajectoryFrenetEnd trajectoryPrev(ego.getFrenet());
    // TODO change test if needed so that circular road is moved on twice
    //  to test issues regarding moving from end of track to start of track
    for (int i=0; i<1000; i++) {
        std::cout << "====> plan step: " << i << std::endl;
        Trajectory trajectory = planner.planTrajectory(ego, SensorFusionResult(), trajectoryPrev);
        trajectoryPrev = moveOnTrajectory(ego, trajectory, map);
        EXPECT_NEAR(roadRadius, ego.getXY().distanceTo(mapCircleCenter), 0.1);
    }
}



