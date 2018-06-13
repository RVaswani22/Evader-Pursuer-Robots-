#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "std_msgs/String.h"
#include "iomanip"
#include "geometry_msgs/Twist.h"
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>


 geometry_msgs::Twist msg;

void chatterCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
ros::NodeHandle n;
	
  ros::Publisher pursuedata = n.advertise<geometry_msgs::Twist>("robot_0/cmd_vel", 1000);  
   geometry_msgs::Twist msg; 
//ROS_INFO("I heard: [%f]", scan->ranges[0]);  
 ROS_INFO("MAIN");
  int i;
  int flag=0;
  for(i=0;i<360;i++)
  {
	if (scan->ranges[i]<1.75)
		{
		
		msg.linear.x=0;
		
msg.angular.z=+1;
	flag=1;
break;
		}
  }
if (flag==0)
  msg.linear.x=2.0;
else 
msg.linear.x=0;
  pursuedata.publish(msg);
 ros::spin();
  //ROS_INFO("%f", msg.linear.x);
  }

// %EndTag(CALLBACK)%
void odompublisher(const nav_msgs::Odometry::ConstPtr& msg)
{
  static tf::TransformBroadcaster br;
  tf::Transform transform;
  transform.setOrigin( tf::Vector3(msg->pose.pose.position.x, msg->pose.pose.position.y, 0.0) );
  //tf::Quaternion q;
  //q.setRPY(0, 0, 0);
  transform.setRotation(tf::Quaternion(	  msg->pose.pose.orientation.x,
		msg->pose.pose.orientation.y,
		msg->pose.pose.orientation.z,
		msg->pose.pose.orientation.w));
  br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "/world", "robot_0/odom"));
	//ROS_INFO("I heard:");
}

void odompublisherw(const nav_msgs::Odometry::ConstPtr& msg)
{
 
  static tf::TransformBroadcaster br;
  tf::Transform transform;
  transform.setOrigin( tf::Vector3(msg->pose.pose.position.x, msg->pose.pose.position.y, 0.0) );
    transform.setRotation(tf::Quaternion( msg->pose.pose.orientation.x,
		msg->pose.pose.orientation.y,
		msg->pose.pose.orientation.z,
		msg->pose.pose.orientation.w));
  br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "/world", "robot_1/odom"));
}

int main(int argc, char **argv)
{
  //ROS_INFO("MAIN");
  ros::init(argc, argv, "listener");

  ros::NodeHandle n;
	
  //ros::Publisher pursuedata = n.advertise<geometry_msgs::Twist>("robot_0/cmd_vel", 1000);  
  ros::Subscriber sub = n.subscribe<sensor_msgs::LaserScan>("robot_0/base_scan", 1000,chatterCallback);
  ros::Subscriber osubr = n.subscribe<nav_msgs::Odometry>("robot_0/odom", 1000,odompublisher);
  ros::Subscriber osubw = n.subscribe<nav_msgs::Odometry>("robot_1/odom", 1000,odompublisherw);
  ros::spinOnce();
  


ros::Rate loop_rate(10);
while (ros::ok())
{  

ros::spinOnce();

loop_rate.sleep();
}
  return 0;
}
// %EndTag(FULLTEXT)%

