#include "gtest/gtest.h"
#include "../src/trajectory_planner/Trajectory.h"
#include "../src/Trigonometry.h"
#include "Verifiers.h"
#include <math.h>


TEST(Trajectory, Size) {
    Trajectory t;
    t.append(0, 0);
    t.append(1, 2);
    t.append(2, 4);
    EXPECT_EQ(3, t.size());
}

TEST(Trajectory, TransformToLocal) {
    Trajectory t;
    t.append(2, 2);
    t.append(3, 2);
    t.transformToLocal(XYCoord(2, 2), deg2rad(90));
    EXPECT_EQ(2, t.size());
    EXPECT_XY_EQ(0, 0, t.getAt(0));
    EXPECT_XY_EQ(0, -1, t.getAt(1));
}


