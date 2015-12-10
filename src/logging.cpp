#include <ros/ros.h>
#include <rosbag/bag.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/UInt16.h>
#include <std_msgs/UInt8.h>
#include <std_msgs/Int8.h>
#include <move_base_msgs/MoveBaseAction.h>
//defining standard parameters
#define WheelDiameter 0.15
#define PI 3.1415

int _log_fr_;
int exit_state;
int i;


float x;
float y;
float rot_z;

move_base_msgs::MoveBaseAction _goal;
std_msgs::UInt16 freecells;
nav_msgs::Odometry odom_log;
rosbag::Bag bag;

void odomMsgs(const nav_msgs::Odometry &odom_)
{
	x = odom_.pose.pose.position.x;
	y = odom_.pose.pose.position.y;
	rot_z = odom_.pose.pose.orientation.z;
	ROS_INFO("Received messages");
	odom_log = odom_;
}

void mode_(const std_msgs::Int8 &state)
{	if (state.data== 0){
		i=50;
	}
}

void goal_(const move_base_msgs::MoveBaseAction &data)
{
	_goal = data;
}

int main(int argc, char** argv){

	ros::init(argc, argv, "logging");

	ros::NodeHandle nh;
	ros::NodeHandle nh1("~");
	//initializing the standard nodehandle and the parameters node handle
	nh1.param("logging_freq",_log_fr_, 10);
	//subscribing to parameters
	ros::Subscriber odom = nh.subscribe("odom",1, &odomMsgs);
	ros::Subscriber mode = nh.subscribe("mode",1, &mode_);
	ros::Subscriber goal = nh.subscribe("/move_base_simple/goal",1, &goal_);

	//subscribing to messages
	bag.open("odom_log.bag", rosbag::bagmode::Write);
	//opening a rosbag and give permission to write to it
	ROS_INFO("Bag is opened");
	
	ros::Rate loop_rate(_log_fr_);
	//defining logging frequency to be value from the parameter server
	while (ros::ok()){
		//some function
		
		if (i<50){
			ROS_INFO("Testing");
			bag.write("Odom",ros::Time::now(), freecells);
			bag.write("Goal",ros::Time::now(), _goal);


		}
		else if (i==50){
			ROS_INFO("Bag is closed");
			bag.close();
			system("mv /home/fieldrobot/.ros/odom_log.bag /home/fieldrobot/catkin_ws/logs/odom_log.bag");
			ros::shutdown();
			}
		loop_rate.sleep();
		ros::spinOnce();

	}

}
