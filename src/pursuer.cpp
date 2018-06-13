#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "std_msgs/String.h"
#include "iomanip"
#include <geometry_msgs/Twist.h>
#include <tf/transform_listener.h>
#include <nav_msgs/Odometry.h>

int main(int argc, char** argv){
  ros::init(argc, argv, "my_tf_listener");
  ros::NodeHandle node;
  ros::Publisher turtle_vel = node.advertise<geometry_msgs::Twist>("robot_1/cmd_vel", 10);
  tf::TransformListener listener;
  ros::Rate rate(10.0);
  while (node.ok()){
    tf::StampedTransform transform;
   try {
    listener.waitForTransform("robot_1/odom", "robot_0/odom", ros::Time(0), ros::Duration(10.0) );
    listener.lookupTransform("robot_1/odom", "robot_0/odom", ros::Time(0), transform);
} catch (tf::TransformException ex) {
    ROS_ERROR("%s",ex.what());
}

    geometry_msgs::Twist vel_msg;
    vel_msg.angular.z = 4.5 * atan2(transform.getOrigin().y(),transform.getOrigin().x());
    vel_msg.linear.x =  0.99*sqrt(pow(transform.getOrigin().x(), 2)+pow(transform.getOrigin().y(), 2));
    ROS_INFO("linear %f",transform.getOrigin().x());
    turtle_vel.publish(vel_msg);
    //ros::spinOnce();
    rate.sleep();
  }
  return 0;
};
