//
// Created by Marc Neumann on 20.10.18.
//

#ifndef PATH_PLANNING_EGOVEHICLESTATE_H
#define PATH_PLANNING_EGOVEHICLESTATE_H

#include "XYCoord.h"
#include "FrenetCoord.h"

/**
 * The state of the ego vehicle.
 */
class EgoVehicleState {

private:
    XYCoord xy;

    FrenetCoord frenet;

    /**
     * yaw angle in degrees
     */
    double yaw;

    /**
     * in meter per second
     */
    double speed;

public:
    EgoVehicleState();
    EgoVehicleState(EgoVehicleState const &toCopy);

    XYCoord getXY() const;

    FrenetCoord getFrenet() const;

    double getX() const;

    double getY() const;

    /**
     *
     * @return in radians
     */
    double getYaw() const;

    double getSpeed() const;

    void setPos(double x, double y, double s, double d);
    void setPos(XYCoord xy, FrenetCoord f);

    void setYaw(double yaw);

    void setSpeed(double speed);

};


#endif //PATH_PLANNING_EGOVEHICLESTATE_H
