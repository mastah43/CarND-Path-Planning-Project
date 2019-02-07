# CarND-Path-Planning-Project
Self-Driving Car Engineer Nanodegree Program

# [Rubic](https://review.udacity.com/#!/rubrics/1020/view) points

## Compilation

### The code compiles correctly.

All code compiles correctly.
The cmake configuration was strongly adapted in order to extend by unit tests using the google test framework.

## Valid trajectories

### The car is able to drive at least 4.32 miles without incident.
TODO I ran the simulator for 15 and 20 miles without incidents:

![15 miles](images/15_miles.png)

![20 miles](images/20_miles.png)

### The car drives according to the speed limit.
Speed was always below 50 miles per hour, no speed limit violation was displayed.

### Max Acceleration and Jerk are not Exceeded.
No maximum jerk violation warning was displayed in simulator.

### Car does not have collisions.
No collision occurred on driven mileage in simulator.

### The car stays in its lane, except for the time between changing lanes.
The car stays in its lane except when changing lanes to avoid other vehicles or when changing back to center lane.

### The car is able to change lanes
The car changes its lane to the to avoid an upfront vehicle in the same lane that is slower.
The lane change is only conducted if there is no other vehicle 30 meters backwards and forwards on the target lane.
The car also changes the lane if its on the left or right most lane and the center lane is free. It proved to be
a good strategy to drive in the center lane so there are two alternatives (left and right) to avoid
slow vehicles in the same lane.

## Reflection

I refactored the provided code to incorporate a structure with separation of responsibilities
in order to avoid redundancy. This allowed the implementation unit tests which helped to
test the trajectory planning for e.g. a trajectory planner that simply follows the lane.



TBD change
The code consist of three parts:

### Prediction [line 255 to line 290](./src/main.cpp#L255)
This part of the code deal with the telemetry and sensor fusion data. It intents to reason about the environment. In the case, we want to know three aspects of it:

- Is there a car in front of us blocking the traffic.
- Is there a car to the right of us making a lane change not safe.
- Is there a car to the left of us making a lane change not safe.

These questions are answered by calculating the lane each other car is and the position it will be at the end of the last plan trajectory. A car is considered "dangerous" when its distance to our car is less than 30 meters in front or behind us.

### Behavior [line 292 to line 314](./src/main.cpp#L293)
This part decides what to do:
  - If we have a car in front of us, do we change lanes?
  - Do we speed up or slow down?

Based on the prediction of the situation we are in, this code increases the speed, decrease speed, or make a lane change when it is safe. Instead of increasing the speed at this part of the code, a `speed_diff` is created to be used for speed changes when generating the trajectory in the last part of the code. This approach makes the car more responsive acting faster to changing situations like a car in front of it trying to apply breaks to cause a collision.

### Trajectory [line 317 to line 416](./src/main.cpp#L313)
This code does the calculation of the trajectory based on the speed and lane output from the behavior, car coordinates and past path points.

First, the last two points of the previous trajectory (or the car position if there are no previous trajectory, lines 321 to 345) are used in conjunction three points at a far distance (lines 348 to 350) to initialize the spline calculation (line 370 and 371). To make the work less complicated to the spline calculation based on those points, the coordinates are transformed (shift and rotation) to local car coordinates (lines 361 to 367).

In order to ensure more continuity on the trajectory (in addition to adding the last two point of the pass trajectory
to the spline adjustment), the pass trajectory points are copied to the new trajectory (lines 374 to 379).
The rest of the points are calculated by evaluating the spline and transforming the output coordinates to not
local coordinates (lines 388 to 407). Worth noticing the change in the velocity of the car from line 393 to 398.
The speed change is decided on the behavior part of the code, but it is used in that part to increase/decrease speed
on every trajectory points instead of doing it for the complete trajectory.


## Build & Run Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Download & run simulator: You can download the Term3 Simulator which contains the Path Planning Project from the [releases tab (https://github.com/udacity/self-driving-car-sim/releases/tag/T3_v1.2).
5. Run it: `./path_planning`.

## Dependencies

* cmake >= 3.5
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `install-mac.sh` or `install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```

