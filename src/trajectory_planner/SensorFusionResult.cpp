//
// Created by Marc Neumann on 13.11.18.
//

#include "SensorFusionResult.h"

SensorFusionResult::SensorFusionResult() : vehicles(std::vector<VehicleState>()) {

}

void SensorFusionResult::addVehicle(VehicleState &vehicle) {
    SensorFusionResult::vehicles.push_back(vehicle);
}
