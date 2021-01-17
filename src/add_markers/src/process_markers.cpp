#include "ros/ros.h"
#include <iostream>
#include <sstream>
#include <string>
//#include "add_markers/MarkerInfo.h"
#include <nav_msgs/Odometry.h>
#include <visualization_msgs/Marker.h>

// Define a global client that can request services
ros::ServiceClient client;

visualization_msgs::Marker marker;
ros::Publisher marker_pub;

const int NA = -1;
const int PICKUP = 0;
const int TRANSIT = 1;
const int DESTINATION = 2;
int state;
float pickup_x;
float pickup_y;
float destination_x;
float destination_y;

const float e = 0.2; //about 10cm distance between the robot and the object will result in pickup/drop

//wait for subscribers for the marker - means wait for a marker to be selected in rviz
//note that if the object is NA (meaning no subscribers) then the object state becomes pickup point again
int waitForSubscribers(){
    while (marker_pub.getNumSubscribers() < 1)
    {
		if (!ros::ok())
		{
			return 0;
		}
		ROS_WARN_ONCE("Please create a subscriber to the marker");
		sleep(1);
      
    }
    
	
	std::stringstream fmt;
	fmt << "Number of subscribers " << marker_pub.getNumSubscribers();
	std::string s = fmt.str();        
	ROS_INFO_STREAM(s);
    
	state = PICKUP;
	return 1;
}

// This callback function continuously executes and reads the odom data and 
void update_marker_callback(const nav_msgs::Odometry odom)
{
	if (marker_pub.getNumSubscribers() < 1){
		state = NA;
	}
	float robot_x = odom.pose.pose.position.x;
	float robot_y = odom.pose.pose.position.y;

	/**std::stringstream fmt;
	fmt <<"state "<< state << "x : " << robot_x << " y : " << robot_y << " del pickup x" << abs(robot_x - marker.pose.position.x) <<  "del pickup y" << abs(robot_y - marker.pose.position.y);
	std::string s = fmt.str();        
	ROS_INFO_STREAM(s);**/

	//in my implementation I hide the marker by making it invisible while in transit
	switch (state)
	{
		case NA:
			waitForSubscribers();
			break;
		case PICKUP:{
			
			if (abs(robot_x - marker.pose.position.x) < e && abs(robot_y - marker.pose.position.y) < e){
				state = TRANSIT;     
			}
			break;}
		case TRANSIT:{
			//hide the marker - make it invisible
			marker.color.a = 0.0; 
			//set the marker to the robot position
			marker.pose.position.x = odom.pose.pose.position.x;
			marker.pose.position.y = odom.pose.pose.position.y;
			//check if destination position is reached
			if (abs(robot_x - destination_x) < e && abs(robot_y - destination_y) < e){
				state = DESTINATION;    
			}
			break;}
		case DESTINATION:{
			marker.color.a = 1.0;
		    marker.pose.position.x = destination_x;
			marker.pose.position.y = destination_y;
			//show the object in the destination position
			break;}
	}
	marker_pub.publish(marker); 
    ros::Duration(0.5).sleep();
}

void addMarker(){
    
    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    marker.header.frame_id = "map"; 
    //other frams: "map", "odom_combined", "base_footprint", "base_link"
    marker.header.stamp = ros::Time::now();

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    marker.ns = "add_markers";
    marker.id = 0;
    uint32_t shape = visualization_msgs::Marker::CUBE;
    // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
    marker.type = shape;

    // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
    marker.action = visualization_msgs::Marker::ADD;

    // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
    marker.pose.position.y = pickup_y;
    marker.pose.position.x = pickup_x;
    marker.pose.position.z = 0;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;

    // Set the scale of the marker -- 1x1x1 here means 1m on a side
    marker.scale.x = 0.5;
    marker.scale.y = 0.5;
    marker.scale.z = 0.5;

    // Set the color -- be sure to set alpha to something non-zero!
    marker.color.r = 0.0f;
    marker.color.g = 1.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0;

    state = PICKUP;
    
}

int main(int argc, char** argv)
{

    // Initializing the add_markers node to perform adding and updating
    ros::init(argc, argv, "add_markers");
    ros::NodeHandle n;


 	ros::Rate r(1);
 	marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);

    //option to parameterize the start position as a command line arg or use 0,3,0,0 as a default
    pickup_x = 0;
    pickup_y = 3;
    destination_x = 0; 
    destination_y = 0;
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
	state = NA;

    //initialize and add the marker
    addMarker();

	//we subscribe to odom values of the robot and use to update the marker state
    ros::Subscriber sub1 = n.subscribe("odom", 100, update_marker_callback);

    ros::spin();

    return 0;
}





// This function provides Marker information including marker's current position and goal position
//I canot use it 
/*void provide_marker_info()
{
   // ROS_INFO_STREAM("Drive robot");

    add_markers::MarkerInfo srv;
    srv.request.position_x = pickup_x;
    srv.request.position_y = position_y;
    srv.request.destination_x = destination_x;
    srv.request.destination_y = destination_y;
    srv.request.state = state;

    // Call the safe_move service and pass the requested joint angles
    if (!client.call(srv))
        ROS_ERROR("Failed to call service");

}*/


