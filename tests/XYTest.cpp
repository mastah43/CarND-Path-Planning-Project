#include "gtest/gtest.h"
#include "../src/trajectory_planner/XYCoord.h"
#include "../src/Trigonometry.h"
#include "Verifiers.h"
#include <math.h>

TEST(XYTest, FirstTest) {
    EXPECT_DOUBLE_EQ(0., XYCoord(0,0).x);
}

TEST(XY, DistanceTo) {
    const XYCoord &a = XYCoord(0, 0);
    const XYCoord &b = XYCoord(1, 1);
    EXPECT_DOUBLE_EQ(sqrt(2), a.distanceTo(b));
}

TEST(XY, HeadingTo) {
    const XYCoord &origin = XYCoord(0, 0);
    EXPECT_DOUBLE_EQ(0, origin.headingTo(XYCoord(1, 0)));
    EXPECT_DOUBLE_EQ(M_PI/4, origin.headingTo(XYCoord(1, 1)));
    EXPECT_DOUBLE_EQ(M_PI/2, origin.headingTo(XYCoord(0, 1)));
    EXPECT_DOUBLE_EQ(M_PI, origin.headingTo(XYCoord(-1, 0)));
    EXPECT_DOUBLE_EQ(-M_PI/2, origin.headingTo(XYCoord(0, -1)));
    EXPECT_DOUBLE_EQ(-M_PI/4, origin.headingTo(XYCoord(1, -1)));
}

TEST(XY, MoveYaw) {
    const XYCoord &origin = XYCoord(0, 0);
    const XYCoord &target = XYCoord(1, 2);
    XYCoord moved = XYCoord(origin);
    moved.moveYaw(origin.headingTo(target), origin.distanceTo(target));
    EXPECT_XY_EQ(target.x, target.y, moved);
}

TEST(XY, TransformLocalMove) {
    EXPECT_XY_EQ(-1,0, XYCoord(0, 0).transformToLocal(XYCoord(1, 0), 0));
    EXPECT_XY_EQ(-1,-1, XYCoord(0, 0).transformToLocal(XYCoord(1, 1), 0));
}

TEST(XY, TransformLocalMoveAndRotate) {
    EXPECT_XY_EQ(-1,0, XYCoord(1, 0).transformToLocal(XYCoord(0, 0), M_PI));
}

TEST(XY, TransformToLocalAndBackToGlobal) {
    XYCoord origin(1, 2);
    double yaw = M_PI/2;
    EXPECT_XY_EQ(2, 3, XYCoord(2, 3).transformToLocal(origin, yaw).transformToGlobal(origin, yaw));
}

TEST(XY, TransformRealistic) {
    XYCoord c(0.27430445606884768, 0.00031840767033004943);
    c.transformToGlobal(XYCoord(923.22619999999995, 1128.7660000000001), 0.01454971894163009);
    EXPECT_XY_EQ(923.50047078965451, 1128.7703087960767, c);
}

