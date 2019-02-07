#include "gtest/gtest.h"
#include "../src/trajectory_planner/Map.h"
#include "Verifiers.h"
#include "MapTestData.h"

TEST(Map, GetFrenetAndGetXY) {
    Map map = *MapLoader().load("../data/highway_map.csv");

    XYCoord c(909.48,1128.67);
    double yaw = c.headingTo(XYCoord(912,1128.67));

    FrenetCoord f = map.getFrenet(c, yaw);
    EXPECT_FRENET_EQ(124.834, 6.16483, f);

    XYCoord c2 = map.getXY(f);
    EXPECT_XY_EQ(c.x, c.y, c2);
}

void testGetFrenetForAllWaypoints(Map &map) {
    for (int i=0; i<map.getWaypointCount(); i++) {
        MapCoord c = map.getWaypointAt(i);
        FrenetCoord f = map.getFrenet(c.xy, map.getPrev(c).headingTo(c));
        EXPECT_NEAR(c.f.s, f.s, 0.01);
        EXPECT_NEAR(c.f.d, f.d, 0.01);
    }
}

TEST(Map, GetFrenetHighwayMap) {
    Map map = *MapLoader().load("../data/highway_map.csv");
    testGetFrenetForAllWaypoints(map);
}

TEST(Map, GetFrenetCircularMap) {
    // TODO frenet s of build map coordinates is unexpectedly 0, seen for coord[1]
    Map map = buildMapCircle(1000, 100);
    testGetFrenetForAllWaypoints(map);
}

TEST(Map, NormalizeFrenetS) {
    Map map = *MapLoader().load("../data/highway_map.csv");
    EXPECT_NEAR(10.5, map.normalizeFrenetS(10.5), 0.001);
    EXPECT_NEAR(10.5, map.normalizeFrenetS(map.getMaxS() + 10.5), 0.001);
}

TEST(Map, GetNextPrevWaypoint) {
    Map map = *MapLoader().load("../data/highway_map.csv");
    double yaw = 6.22631;
    XYCoord xy = XYCoord(1792.57, 1146.62);
    FrenetCoord f = map.getFrenet(xy, yaw);
    MapCoord prevWP = map.getPrevWaypointByFrenetS(f.s);
    MapCoord nextWP = map.getNextWaypoint(xy, yaw);
    EXPECT_EQ(prevWP.index + 1, nextWP.index);
}

TEST(Map, GetXyOnStraightRoad) {
    Map map = buildMapStraight(1000, 10);
    EXPECT_XY_EQ(150, 0, map.getXY(FrenetCoord(150,0)));
    EXPECT_XY_EQ(450, 0, map.getXY(FrenetCoord(450,0)))
}

TEST(Map, GetXyOnCirclularRoad) {
    double radius = 100;
    Map map = buildMapCircle(radius, 1000);
    FrenetCoord f(0,0);
    XYCoord origin(0,0);

    int steps = 100;
    double distance = 2*M_PI*radius;
    double stepDistance = distance/steps/2;

    // verify known points on map
    EXPECT_XY_EQ(0, radius, map.getXY(FrenetCoord(0,0)));
    EXPECT_XY_EQ(radius, 0, map.getXY(FrenetCoord(distance/4,0)));
    EXPECT_XY_EQ(0, -radius, map.getXY(FrenetCoord(distance/2,0)));
    EXPECT_XY_EQ(-radius, 0, map.getXY(FrenetCoord(distance/4*3,0)));

    // verify distance to map center going two times around the trackl
    double distanceStep = 0.2;
    for (int i=0; i<2000; i++) {
        XYCoord c = map.getXY(f);
        EXPECT_NEAR(radius, origin.distanceTo(c), 0.2);
        f.incS(stepDistance);
    }
    std::cout << "total s: " << f.s << std::endl;
}




