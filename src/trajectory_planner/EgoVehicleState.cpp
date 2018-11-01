//
// Created by Marc Neumann on 20.10.18.
//

#include "EgoVehicleState.h"
#include "../Trigonometry.h"

EgoVehicleState::EgoVehicleState() : xy(XYCoord(-1, -1), frenet(FrenetCoord(-1, -1))) {}

XYCoord &EgoVehicleState::getXy() const {
    return xy;
}

FrenetCoord &EgoVehicleState::getFrenet() const {
    return frenet;
}

double EgoVehicleState::getYaw() const {
    return yaw;
}

double EgoVehicleState::getSpeed() const {
    return speed;
}

void EgoVehicleState::setPos(double x, double y, double s, double d) {
    EgoVehicleState::xy.x = x;
    EgoVehicleState::xy.y = y;
    EgoVehicleState::frenet.s = s;
    EgoVehicleState::frenet.d = d;
}

void EgoVehicleState::setYaw(double yaw) {
    EgoVehicleState::yaw = yaw;
}

void EgoVehicleState::setSpeed(double speed) {
    EgoVehicleState::speed = speed;
}

double EgoVehicleState::getYawRad() const {
    return deg2rad(EgoVehicleState::yaw);
}

const double EgoVehicleState::getX() const {
    return EgoVehicleState::xy.x;
}

const double EgoVehicleState::getY() const {
    return EgoVehicleState::xy.y;
}



