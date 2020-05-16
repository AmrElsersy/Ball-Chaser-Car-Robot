#include <ros/ros.h>
#include "ball_chaser/DriveToTarget.h"
#include <bits/stdc++.h>
using namespace std;
#include "sensor_msgs/Image.h"

ros::ServiceClient client;
void drive_robot(float linear_x, float angular_z)
{
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = (float)linear_x;
    srv.request.angular_z = (float)angular_z;

    if (!client.call(srv))
        ROS_ERROR("Failed to call service command_robot");
}


void detect_white_ball(const sensor_msgs::Image image)
{
    int white = 255;
    float x_speed = 1;
    float rotation = 3;

    int left = image.step/4;
    int right = 3 * image.step/4;

    for (int i = 0 ; i < image.height * image.step; i += 4)
    {
            if(image.data[i] == white && image.data[i+1] == white && image.data[i+2] == white && image.data[i+3] == white )
            {
                int column = i % image.step;
                cout << "column " << column << " left " << left << "  right" << right << endl;
                if (column < left)
                {
                    drive_robot(0,  rotation);
                }
                else if (column > right)
                {
                    drive_robot(0,  -rotation);
                }
                else
                {
                    drive_robot(x_speed, 0);
                }
                return;
            }
    }

    drive_robot(0,0);
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "image_process");
    ros::NodeHandle n;

    client = n.serviceClient<ball_chaser::DriveToTarget>("ball_chaser/command_robot");
    ros::Subscriber subscriber = n.subscribe("/camera/rgb/image_raw",10,detect_white_ball);

    ros::spin();
    return 0;
}