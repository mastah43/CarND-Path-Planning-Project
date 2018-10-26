//
// Created by Marc Neumann on 20.10.18.
//

#ifndef PATH_PLANNING_EGOVEHICLESTATE_H
#define PATH_PLANNING_EGOVEHICLESTATE_H

/**
 * The state if the ego vehicle.
 */
class EgoVehicleState {

private:
    /**
     * in meter
     */
    double x; // in meter

    /**
     * in meter
     */
    double y; // in meter

    /**
     * frenet distance longitudinal in meter
     */
    double s;

    /**
     * frenet deviation lateral in meter
     */
    double d;

    /**
     * yaw angle in degrees
     */
    double yaw;

    /**
     * in meter per second
     */
    double speed;

public:
    double getX() const;

    double getY() const;

    double getS() const;

    double getD() const;

    double getYaw() const;

    double getSpeed() const;

    void setX(double x);

    void setY(double y);

    void setS(double s);

    void setD(double d);

    void setYaw(double yaw);

    void setSpeed(double speed);

};


#endif //PATH_PLANNING_EGOVEHICLESTATE_H
