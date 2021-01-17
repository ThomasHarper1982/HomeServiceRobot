# HomeServiceRobot
17/01/2020				Thomas Harper

git clone https://github.com/ThomasHarper1982/HomeServiceRobot.git

We apply the following processes/nodes (1) - (7), in each of the activities. I will explain which processes are used and the packages they entail briefly

mapping:
Mapping is used to allow a robot to sample the environment and create a map for navigation tasks.

(1)The turtlebot_gazebo package is part of a parent package turtlebot_simulation and is used to launch the world file and robot within the Turtlebot package
(2)it also launches gmapping_demo which runs the the slam_gmapping package libraries.
Note it applies the slam_gmapping_pr2 parameters to optimise the slam.

(3)turtlebot_rviz_launchers: is a specialised pre-configured RViz package for visualizing mapping and navigation tasks, it is part of the parent package turtlebot interactions.

(4)turtlebot_teleop: is a package in turtlebot for keyboard commands

localisation:
Also applies (1) and (3), 
(5) We launch the amcl_demo.launch within the turtlebot_gazebo to appliy the package "turtlebot_navigation" which is installed into the system:
E.G /opt/ros/kinetic/lib/turtlebot_navigation; ./opt/ros/kinetic/share/turtlebot_navigation
This applies "AMCL localisation" algorithm to generate robot poses against objects.

Home Service && navigation:
Apples (1), (3), (5) and also we introduce our custom packages: add_markers && pick_objects, which we use to create separate nodes for each package. 

(7) add_markers node: 
The marker keeps track of the position of the robot and changes its state from PICKUP - > TRANSIT -> DESTINATION accordingly.
Subscribes to Odom topic using Odometry.h from the nav_msgs package.
And interacts with Markers via the visualization_msgs package

(6) pick_objects  node: 
This node is used to set and instruct the robot to visit the pickup and destination targes.
It applies the move_base_msgs package of ROS to sets goal objects with target positions
actionlib/client/simple_action_client is a client that implements actions/go
