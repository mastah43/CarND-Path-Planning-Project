project(Path_Planning)

cmake_minimum_required (VERSION 3.5)

add_definitions(-std=c++11)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

set(sources src/main.cpp src/trajectory_planner/TrajectoryPlanner.cpp src/trajectory_planner/TrajectoryPlanner.h src/trajectory_planner/EgoVehicleState.cpp src/trajectory_planner/EgoVehicleState.h src/trajectory_planner/Trajectory.cpp src/trajectory_planner/Trajectory.h src/trajectory_planner/TrajectoryFrenetEnd.cpp src/trajectory_planner/TrajectoryFrenetEnd.h src/trajectory_planner/FrenetCoord.h src/trajectory_planner/Map.cpp src/trajectory_planner/Map.h src/trajectory_planner/XYCoord.h src/trajectory_planner/XYCoord.cpp src/trajectory_planner/FrenetCoord.cpp src/trajectory_planner/MapBuilder.cpp src/trajectory_planner/MapCoord.cpp src/trajectory_planner/MapLoader.cpp src/Trigonometry.h src/trajectory_planner/TrajectoryPlannerFollowLane.cpp src/trajectory_planner/TrajectoryPlannerFollowLane.h src/trajectory_planner/SensorFusionResult.cpp src/trajectory_planner/SensorFusionResult.h src/trajectory_planner/XYVelocity.cpp src/trajectory_planner/XYVelocity.h src/trajectory_planner/VehicleState.cpp src/trajectory_planner/VehicleState.h src/spline/spline.h)
set(sources_wo_main src/trajectory_planner/TrajectoryPlanner.cpp src/trajectory_planner/TrajectoryPlanner.h src/trajectory_planner/EgoVehicleState.cpp src/trajectory_planner/EgoVehicleState.h src/trajectory_planner/Trajectory.cpp src/trajectory_planner/Trajectory.h src/trajectory_planner/TrajectoryFrenetEnd.cpp src/trajectory_planner/TrajectoryFrenetEnd.h src/trajectory_planner/FrenetCoord.h src/trajectory_planner/Map.cpp src/trajectory_planner/Map.h src/trajectory_planner/XYCoord.h src/trajectory_planner/XYCoord.cpp src/trajectory_planner/FrenetCoord.cpp src/trajectory_planner/MapBuilder.cpp src/trajectory_planner/MapCoord.cpp src/trajectory_planner/MapLoader.cpp src/Trigonometry.h src/trajectory_planner/TrajectoryPlannerFollowLane.cpp src/trajectory_planner/TrajectoryPlannerFollowLane.h src/trajectory_planner/SensorFusionResult.cpp src/trajectory_planner/SensorFusionResult.h src/trajectory_planner/XYVelocity.cpp src/trajectory_planner/XYVelocity.h src/trajectory_planner/VehicleState.cpp src/trajectory_planner/VehicleState.h src/spline/spline.h)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") 

include_directories(/usr/local/include)
include_directories(/usr/local/opt/openssl/include)
link_directories(/usr/local/lib)
link_directories(/usr/local/opt/openssl/lib)
link_directories(/usr/local/Cellar/libuv/1.11.0/lib)

endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") 

add_executable(path_planning ${sources})
target_link_libraries(path_planning z ssl uv uWS)

file(GLOB test_sources "tests/*.cpp")
add_subdirectory(gtests)
find_package(Threads REQUIRED)
enable_testing()
include_directories(${GTEST_SOURCE_DIR}/include ${GTEST_SOURCE_DIR})
add_executable(runAllTests ${test_sources} ${sources_wo_main})
target_link_libraries(runAllTests gtest gtest_main z ssl uv uWS)
add_test(AllTests runAllTests)
