#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/PoseStamped.h>
#include <sensor_msgs/Joy.h>
#include "tf/tf.h"

class TeleopQuad
{
	public:
		TeleopQuad();

	private:
		void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);
		ros::NodeHandle nh;

		double gainx=0.1,gainy=0.1,gainz=0.01,gainr=1,gainp=1,gainyaw=0.1;
		double x=0,y=0,z=1,roll=0,pitch=0,yaw=0;
		tf::Quaternion q;
  		ros::Subscriber joy_sub;
		ros::Publisher goal_pub;

};

TeleopQuad::TeleopQuad()
{
	joy_sub= nh.subscribe<sensor_msgs::Joy>("joy",10,&TeleopQuad::joyCallback, this);
	goal_pub = nh.advertise<geometry_msgs::PoseStamped>("/pelican/command/pose",1);
}

void TeleopQuad::joyCallback(const sensor_msgs::Joy::ConstPtr &joy)
{
	geometry_msgs::PoseStamped newpose;
	
	x=x+gainx*joy->axes[1];
	y=y+gainy*joy->axes[0];
	z=z+gainz*joy->axes[4];
	if (z<=0)
		z=0;
	yaw=yaw+gainyaw*joy->axes[3];
	// q.setRPY(0, 0, yaw);
	q=tf::createQuaternionFromYaw(yaw);
	q.normalize();
	newpose.pose.position.x=x;
	newpose.pose.position.y=y;
	newpose.pose.position.z=z;

	newpose.pose.orientation.z=q.z();
	newpose.pose.orientation.w=q.w();

	// newpose.pose.orientation=q;

	goal_pub.publish(newpose);


}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "quad_turtle");
  TeleopQuad teleop_quad;




  ros::spin();
}
