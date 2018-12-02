#include <fstream>
#include <math.h>
#include <uWS/uWS.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include "Eigen-3.3/Eigen/Core"
#include "Eigen-3.3/Eigen/QR"
#include "json.hpp"
#include "Trigonometry.h"
#include "trajectory_planner/TrajectoryPlannerFollowLane.h"
#include "trajectory_planner/Map.h"
#include "trajectory_planner/SensorFusionResult.h"
#include "trajectory_planner/WorldConstants.h"

using namespace std;

// for convenience
using json = nlohmann::json;

/**
 * Parses the given event data and return json payload if contained
 * otherwise null is returned.
 * @param s
 * @return
 */
std::string getEventJsonString(string s) {
    auto found_null = s.find("null");
    auto b1 = s.find_first_of('[');
    auto b2 = s.find_first_of('}');
    if (found_null != string::npos) {
        return "";
    } else if (b1 != string::npos && b2 != string::npos) {
        std::string jsonString = s.substr(b1, b2 - b1 + 2);
        return jsonString;
    } else {
        return "";
    }
}

Map* loadMap() {
    return MapLoader().load("../data/highway_map.csv");
}

EgoVehicleState fillEgoState(json &j) {
    EgoVehicleState state;
    state.setPos(j["x"], j["y"], j["s"], j["d"]);
    state.setYaw(deg2rad(j["yaw"]));
    state.setSpeed(j["speed"]);
    return state;
}

TrajectoryFrenetEnd fillTrajectoryPrevious(json &j) {
    FrenetCoord trajectoryEnd(j["end_path_s"], j["end_path_d"]);
    TrajectoryFrenetEnd trajectory(trajectoryEnd);
    json jPathX = j["previous_path_x"];
    json jPathY = j["previous_path_y"];
    for (int i=0; i<jPathX.size(); i++) {
        trajectory.append(jPathX[i], jPathY[i]);
    }
    return trajectory;
}

SensorFusionResult fillSensorFusion(json &j) {
    SensorFusionResult fusion;
    for (auto jVehicle : j) {
        int id = jVehicle[0];
        double x = jVehicle[1];
        double y = jVehicle[2];
        double vx = jVehicle[3];
        double vy = jVehicle[4];
        double s = jVehicle[5];
        double d = jVehicle[6];
        VehicleState vehicle(id, XYCoord(x,y), FrenetCoord(s,d), XYVelocity(vx, vy));
        fusion.addVehicle(vehicle);
    }
    return fusion;
}

void sendTrajectory(uWS::WebSocket<uWS::SERVER> &ws, Trajectory &trajectory) {
    json msgJson;
    std::cout << " next x vals length: " << trajectory.getX().size() << std::endl; // TODO remove
    msgJson["next_x"] = trajectory.getX();
    msgJson["next_y"] = trajectory.getY();

    auto msg = "42[\"control\"," + msgJson.dump() + "]";
    ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
}

bool isEvent(char *data, size_t length) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    return length > 2 && data[0] == '4' && data[1] == '2';
}

int main() {
    uWS::Hub h;

    Map map = *loadMap();
    TrajectoryPlannerFollowLane trajectoryPlanner(map);

    {
        std::string jsonString = "[\"telemetry\",{\"x\":909.48,\"y\":1128.67,\"yaw\":0,\"speed\":0,\"s\":124.8336,\"d\":6.164833,\"previous_path_x\":[],\"previous_path_y\":[],\"end_path_s\":0,\"end_path_d\":0,\"sensor_fusion\":[[0,775.99,1421.6,0,0,6721.839,-277.6729],[1,775.8,1425.2,0,0,6719.219,-280.1494],[2,775.8,1429,0,0,6716.599,-282.9019],[3,775.8,1432.9,0,0,6713.911,-285.7268],[4,775.8,1436.3,0,0,6711.566,-288.1896],[5,775.8,1441.7,0,0,6661.772,-291.7797],[6,762.1,1421.6,0,0,6711.778,-268.0964],[7,762.1,1425.2,0,0,6709.296,-270.7039],[8,762.1,1429,0,0,6663.543,-273.1828],[9,762.1,1432.9,0,0,6660.444,-275.5511],[10,762.1,1436.3,0,0,6657.743,-277.6157],[11,762.1,1441.7,0,0,6653.453,-280.8947]]}]";
        auto j = json::parse(jsonString);
        std::string event = j[0].get<string>();
        std::cout << "event:" << event << std::endl;
    }


    h.onMessage(
            [&map, &trajectoryPlanner](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
                if (isEvent(data, length)) {

                    std::string jsonString = getEventJsonString(data);
                    if (!jsonString.empty()) {
                        json eventJson = json::parse(jsonString);
                        json eventHeaderJson = eventJson[0];
                        string event = eventHeaderJson.get<string>();
                        if (event == "telemetry") {
                            json jsonData = eventJson[1];

                            TrajectoryFrenetEnd trajectoryPrevious = fillTrajectoryPrevious(jsonData);
                            EgoVehicleState egoVehicleState = fillEgoState(jsonData);

                            // Sensor Fusion Data, a list of all other cars on the same side of the road.
                            SensorFusionResult sensorFusion = fillSensorFusion(jsonData["sensor_fusion"]);

                            // TODO record behaviour of other objects as trajectories to build a model that can predict trajectories of other objects

                            Trajectory trajectory = trajectoryPlanner.planTrajectory(egoVehicleState,
                                                                                   sensorFusion,
                                                                                   trajectoryPrevious);

                            sendTrajectory(ws, trajectory);

                            // wait so half the trajectory is driven before the next trajectory is computed
                            //this_thread::sleep_for(chrono::milliseconds(WAYPOINTS_COUNT*WAYPOINT_STEP_TIME_MS/2));
                        }
                    } else {
                        // Manual driving
                        std::string msg = "42[\"manual\",{}]";
                        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
                    }
                }
            });

    // We don't need this since we're not using HTTP but if it's removed the
    // program
    // doesn't compile :-(
    h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data,
                       size_t, size_t) {
        const std::string s = "<h1>Hello world!</h1>";
        if (req.getUrl().valueLength == 1) {
            res->end(s.data(), s.length());
        } else {
            // i guess this should be done more gracefully?
            res->end(nullptr, 0);
        }
    });

    h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
        std::cout << "Connected!!!" << std::endl;
    });

    h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code,
                           char *message, size_t length) {
        ws.close();
        std::cout << "Disconnected" << std::endl;
    });

    int port = 4567;
    if (h.listen(port)) {
        std::cout << "Listening to port " << port << std::endl;
    } else {
        std::cerr << "Failed to listen to port" << std::endl;
        return -1;
    }
    h.run();
}

