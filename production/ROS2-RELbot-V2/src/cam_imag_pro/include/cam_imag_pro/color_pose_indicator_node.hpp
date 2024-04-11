#pragma once
#include <chrono>
#include <memory>
#include <string>

#include "cv_bridge/cv_bridge.h"
#include "geometry_msgs/msg/point.hpp"
#include "opencv2/opencv.hpp"
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;

class ColorPoseNode : public rclcpp::Node {
private:
  // attributes
  geometry_msgs::msg::Point center_of_gravity;
  rclcpp::Publisher<geometry_msgs::msg::Point>::SharedPtr pose_publisher;
  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription;
  rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publish_with_mark;

  // functions
  void callback(const sensor_msgs::msg::Image &cam_frame);

public:
  ColorPoseNode();
};
