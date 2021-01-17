#!/bin/sh
xterm  -e  "roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=$(rospack find turtlebot_gazebo)/worlds/maze_world" &
sleep 10
xterm  -e  "roslaunch turtlebot_gazebo gmapping_demo.launch" &
sleep 10
xterm  -e  "roslaunch turtlebot_rviz_launchers view_navigation.launch" &
sleep 10
xterm  -e  "roslaunch turtlebot_teleop keyboard_teleop.launch" &
sleep 10
