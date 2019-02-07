#include "MapTestData.h"

Map buildMapStraight(double distance, int waypointCount)  {
    MapBuilder mapBuilder(distance);
    double waypointDistance = distance/waypointCount;
    for (int i=0; i<waypointCount; i++) {
        double s = i*waypointDistance;
        mapBuilder.addCoord(XYCoord(s,0), FrenetCoord(s,0), 0, -1);
    }
    Map map = *mapBuilder.build();
    return map;
}

Map buildMapCircle(double radius, int waypointCount)  {
    double roadLength = radius*2*M_PI;
    MapBuilder mapBuilder(roadLength);
    for (int i=0; i<waypointCount; i++) {
        double angle = i*2*M_PI/waypointCount;
        double x = sin(angle)*radius;
        double y = cos(angle)*radius;
        XYCoord xy = XYCoord(x, y);
        // TODO remove //double s = angle/(2*M_PI)*roadLength;
        double dx = sin(angle);
        double dy = cos(angle);
        mapBuilder.addCoord(xy, dx, dy);
    }
    Map map = *mapBuilder.build();
    return map;
}