//
// Created by Marc Neumann on 13.11.18.
//

#ifndef PATH_PLANNING_SENSORFUSIONRESULT_H
#define PATH_PLANNING_SENSORFUSIONRESULT_H


#include <vector>
#include "VehicleState.h"

class SensorFusionResult {
private:
    std::vector<VehicleState> vehicles;
public:
    SensorFusionResult();
    void addVehicle(VehicleState &vehicle);
};


#endif //PATH_PLANNING_SENSORFUSIONRESULT_H
