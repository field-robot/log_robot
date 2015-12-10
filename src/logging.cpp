#include <ros/ros.h>
#include <rosbag/bag.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/UInt16.h>
#include <std_msgs/UInt8.h>

//defining standard parameters
#define WheelDiameter 0.15
#define PI 3.1415

int _log_fr_;
int exit_state;

float x;
float y;
float rot_z;

void odomMsgs(const nav_msgs::Odometry &odom_)
{
	x = odom_.pose.pose.position.x;
	y = odom_.pose.pose.position.y;
	rot_z = odom_.pose.pose.orientation.z;
}

void exiting(const std_msgs::UInt8 &state)
{	if (exit_state != state.data){
		exit_state = state.data;
	}
}

int main(int argc, char** argv){

	ros::init(argc, argv, "logging");

	ros::NodeHandle nh;
	ros::NodeHandle nh1("~");
	//initializing the standard nodehandle and the parameters node handle
	nh1.param("logging_freq",_log_fr_, 10);
	//subscribing to parameters
	ros::Subscriber odom = nh.subscribe("odom",1, &odomMsgs);
	ros::Subscriber stop = nh.subscribe("exit",1, &exiting);
	//subscribing to messages
	ros::Rate loop_rate(_log_fr_);
	//defining logging frequency to be value from the parameter server
	while (ros::ok()){
		//some function
		ROS_INFO("Testing");
		loop_rate.sleep();
		ros::spinOnce();
	}

}
