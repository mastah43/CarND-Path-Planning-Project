//
// Created by Marc Neumann on 20.10.18.
//

#include "EgoVehicleState.h"

double EgoVehicleState::getX() const {
    return x;
}

double EgoVehicleState::getY() const {
    return y;
}

double EgoVehicleState::getS() const {
    return s;
}

double EgoVehicleState::getD() const {
    return d;
}

double EgoVehicleState::getYaw() const {
    return yaw;
}

double EgoVehicleState::getSpeed() const {
    return speed;
}

void EgoVehicleState::setX(double x) {
    EgoVehicleState::x = x;
}

void EgoVehicleState::setY(double y) {
    EgoVehicleState::y = y;
}

void EgoVehicleState::setS(double s) {
    EgoVehicleState::s = s;
}

void EgoVehicleState::setD(double d) {
    EgoVehicleState::d = d;
}

void EgoVehicleState::setYaw(double yaw) {
    EgoVehicleState::yaw = yaw;
}

void EgoVehicleState::setSpeed(double speed) {
    EgoVehicleState::speed = speed;
}
