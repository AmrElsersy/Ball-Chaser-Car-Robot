#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ball_chaser/DriveToTarget.h"
#include <string>
using namespace std;
ros::Publisher motor_command_publisher;

bool callback(ball_chaser::DriveToTarget::Request& req, ball_chaser::DriveToTarget::Response& res)
{
    geometry_msgs::Twist car_control;
    car_control.linear.x = req.linear_x;
    car_control.angular.z = req.angular_z;

    res.msg_feedback="you successfully command the car";
    motor_command_publisher.publish(car_control);

    std::cout<< car_control.linear.x << " " << car_control.angular.z << endl;

    return true;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "drive_bot");
    ros::NodeHandle n;

    motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    ros::ServiceServer server = n.advertiseService("ball_chaser/command_robot",callback);

    ros::spin();
    return 0;
}
