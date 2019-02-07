//
// Created by Marc Neumann on 29.10.18.
//

#ifndef PATH_PLANNING_XYCOORD_H
#define PATH_PLANNING_XYCOORD_H

#include <math.h>
#include <string>

class XYCoord {
public:

    XYCoord(double x, double y);
    XYCoord(const XYCoord &xy);

    friend std::ostream & operator<<(std::ostream & Str, XYCoord const & c);

    /**
     * in meter
     */
    double x;

    /**
     * in meter
     */
    double y;

    double distanceTo(const XYCoord &c) const;

    double distanceToOrigin() const;

    double headingTo(const XYCoord &c) const;

    /**
     * Transform to local coordinate system by using given new origin and orientation.
     * @param origin absolute position of local coordinate system in global coordinate system
     * @param yaw orientation of local coordinate system in absolute coordinate syste in radians
     * @return reference to self
     */
    XYCoord& transformToLocal(XYCoord origin, double yaw);

    /**
     * Transform to global coordinate system by using given origin and orientation.
     * @param origin absolute position of local coordinate system in global coordinate system
     * @param yaw orientation of local coordinate system in absolute coordinate syste in radians
     * @return reference to self
     */
    XYCoord& transformToGlobal(XYCoord origin, double yaw);

    /**
     * Moves this position by the given distance in the given direction.
     * @param yaw in radians
     * @param distance
     * @return reference to self
     */
    XYCoord& moveYaw(double yaw, double distance);

};

#endif //PATH_PLANNING_XYCOORD_H
