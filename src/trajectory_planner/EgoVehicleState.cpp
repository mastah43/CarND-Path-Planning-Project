//
// Created by Marc Neumann on 20.10.18.
//

#include "EgoVehicleState.h"
#include "../Trigonometry.h"

EgoVehicleState::EgoVehicleState() : xy(XYCoord(-1, -1)), frenet(FrenetCoord(-1, -1)) {}

EgoVehicleState::EgoVehicleState(EgoVehicleState const &toCopy) = default;

XYCoord EgoVehicleState::getXY() const {
    return xy;
}

FrenetCoord EgoVehicleState::getFrenet() const {
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

double EgoVehicleState::getX() const {
    return EgoVehicleState::xy.x;
}

double EgoVehicleState::getY() const {
    return EgoVehicleState::xy.y;
}

void EgoVehicleState::setPos(XYCoord xy, FrenetCoord f) {
    EgoVehicleState::xy = xy;
    EgoVehicleState::frenet = f;
}



