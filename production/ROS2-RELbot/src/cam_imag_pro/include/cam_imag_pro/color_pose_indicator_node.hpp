#pragma once
#include <chrono>
#include <memory>
#include <string>

#include "cv_bridge/cv_bridge.h"
#include "geometry_msgs/msg/point_stamped.hpp"
#include "opencv2/opencv.hpp"
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;

class ColorPoseNode : public rclcpp::Node {
private:
  // center of the image capture by the camera
  int frame_center =
      100; // some random value, it will be updated in the color_pose_callback
  // attributes
  geometry_msgs::msg::PointStamped center_of_gravity;
  rclcpp::Publisher<geometry_msgs::msg::PointStamped>::SharedPtr pose_publisher;
  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription;
  rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr
      publish_with_mark; // to visually check the detected color

  // functions
  void callback(const sensor_msgs::msg::Image &cam_frame);

public:
  ColorPoseNode();
};
