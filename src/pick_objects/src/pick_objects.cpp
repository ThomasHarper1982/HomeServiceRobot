#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
//#include "add_markers/MarkerInfo.h"

// Define a client for to send goal requests to the move_base server through a SimpleActionClient
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv){

	//the pickup and destination points
    float pickup_x = 0;
    float pickup_y = 3;
    float destination_x = 0; 
    float destination_y = -3;

    //option to parameterize the start position as a command line arg or use 0,3,0,-3 as a default
    if (argc == 4){
		try{
			std::istringstream(argv[0]) >> pickup_x;
			std::istringstream(argv[1]) >> pickup_y;
			std::istringstream(argv[2]) >> destination_x;
			std::istringstream(argv[3]) >> destination_y;
		}catch(...){
	        ROS_INFO_STREAM("Inputs received in as pickup_x = 0, pickup_y = 3, destination_x = 0, destination_y = 0 {float64 float64 float64 float64}");
	    }
    }

	// Initialize the simple_navigation_goals node
	ros::init(argc, argv, "pick_objects");

	//tell the action client that we want to spin a thread by default
	MoveBaseClient ac("move_base", true);
	while(!ac.waitForServer(ros::Duration(5.0))){
	ROS_INFO("Waiting for the move_base action server to come up");
	}
	
	
    move_base_msgs::MoveBaseGoal goal;

	// set up the frame parameters
	goal.target_pose.header.frame_id = "base_link";
	goal.target_pose.header.stamp = ros::Time::now();

	// Define a position and orientation for the robot to reach
	goal.target_pose.pose.position.x = pickup_x;
	goal.target_pose.pose.position.y = pickup_y;
	goal.target_pose.pose.orientation.w = 1.0;

	// Send the goal position and orientation for the robot to reach
	ac.sendGoal(goal);

	ac.waitForResult();
	if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
		ROS_INFO("Pickup point is reached");
	else
		ROS_INFO("Failed to reach pickup point");
	sleep(5);

	goal.target_pose.pose.position.x = destination_x;
	goal.target_pose.pose.position.y = destination_y;
	goal.target_pose.pose.orientation.w = 1.0;

	// Send the goal position and orientation for the robot to reach
	ac.sendGoal(goal);

	ac.waitForResult();
	if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
		ROS_INFO("Destination point is reached");
	else
		ROS_INFO("Failed to reach destination point");
			

	return 0;
}






// I had an idea of using SRV message passing to allow the process_marker to provide 
// its position to set the goal position for this node, but I abandoned as it was getting too complex
/**
void handle_marker_info_request(add_markers::MarkerInfo::Request& req,
    ball_chaser::MarkerInfo::Response& res){
	
	state = req.state;
	pickup_x = req.position_x;
	pickup_y = req.position_y;
	destination_x = req.destination_x;
	destination_y = req.destination_y;

	switch(state)
		case (NA);
			sleep(1);
			break;
		case (PICKUP):
			go_to_goal(pickup_x, pickup_y);
			if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
				ROS_INFO("Pickup point is reached");
			else
				ROS_INFO("Failed to reach pickup point");
			sleep(5);
	   		break;
		case (TRANSIT):
			go_to_goal(destination_x, destination_y);
			if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
				ROS_INFO("Destination point is reached");
			else
				ROS_INFO("Failed to reach destination point");
			break;
		case (DESTINATION):
			sleep(1);
			break;
}*/
/*
void go_to_goal(float x, float y){
	move_base_msgs::MoveBaseGoal goal;

	// set up the frame parameters
	goal.target_pose.header.frame_id = "base_link";
	goal.target_pose.header.stamp = ros::Time::now();

	// Define a position and orientation for the robot to reach
	goal.target_pose.pose.position.x = x;
	goal.target_pose.pose.position.y = y;
	goal.target_pose.pose.orientation.w = 1.0;

	// Send the goal position and orientation for the robot to reach
	ac.sendGoal(goal);

	// Wait an infinite time for the results
	ac.waitForResult();

}*/
