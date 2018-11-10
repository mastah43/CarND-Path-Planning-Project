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
#include "trajectory_planner/TrajectoryPlanner.h"
#include "trajectory_planner/Map.h"

using namespace std;

// for convenience
using json = nlohmann::json;

/**
 * Parses the given event data and return json payload if contained
 * otherwise null is returned.
 * @param s
 * @return
 */
std::shared_ptr<json> getEventJson(string s) {
    auto found_null = s.find("null");
    auto b1 = s.find_first_of('[');
    auto b2 = s.find_first_of('}');
    if (found_null != string::npos) {
        return nullptr;
    } else if (b1 != string::npos && b2 != string::npos) {
        json::parse(s.substr(b1, b2 - b1 + 2));
        return shared_ptr<json>();
    } else {
        return nullptr;
    }
}

Map &loadMap() {
    return MapLoader().load("../data/highway_map.csv");
}

EgoVehicleState fillEgoState(json &j) {
    EgoVehicleState state;
    state.setPos(j["x"], j["y"], j["s"], j["d"]);
    state.setYaw(j["yaw"]);
    state.setSpeed(j["speed"]);
    return state;
}

TrajectoryFrenetEnd fillTrajectoryPrevious(json &j) {
    FrenetCoord trajectoryEnd(j["end_path_s"], j["end_path_d"]);
    TrajectoryFrenetEnd trajectory(trajectoryEnd);

    // TODO iterate on x and y path coordindates and append
    trajectory.append(j["previous_path_x"], j["previous_path_y"]);
    return trajectory;
}

void sendTrajectory(uWS::WebSocket<uWS::SERVER> &ws, Trajectory &trajectory) {
    json msgJson;
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

    // TODO
    std::string file = "TODO";
    std::ifstream in_map_(file.c_str(), std::ifstream::in);

    string line;
    while (getline(in_map_, line)) {
        istringstream iss(line);
        double x;
        double y;
        float s;
        float d_x;
        float d_y;
        iss >> x;
        iss >> y;
        iss >> s;
        iss >> d_x;
        iss >> d_y;
    }

    TrajectoryPlanner trajectoryPlanner;
    Map map = loadMap();

    h.onMessage(
            [&map, &trajectoryPlanner](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
                if (isEvent(data, length)) {
                    auto j = getEventJson(data);
                    if (j) {
                        json eventJson = j.get();
                        string event = eventJson[0].get<string>();
                        if (event == "telemetry") {
                            json jsonData = eventJson[1];

                            TrajectoryFrenetEnd trajectoryPrevious = fillTrajectoryPrevious(jsonData);
                            EgoVehicleState egoVehicleState = fillEgoState(jsonData);

                            // Sensor Fusion Data, a list of all other cars on the same side of the road.
                            auto sensor_fusion = eventJson[1]["sensor_fusion"];
                            // TODO convert to object hierarchy

                            // TODO record behaviour of other objects as trajectories to build a model that can predict trajectories of other objects

                            // TODO the trajectory planner should be aware already of the previous trajectory so remove the argument
                            const Trajectory trajectory = trajectoryPlanner.planTrajectory(egoVehicleState,
                                                                                           trajectoryPrevious);

                            //this_thread::sleep_for(chrono::milliseconds(1000));

                            // TODO sendTrajectory(&ws, &trajectory);
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

