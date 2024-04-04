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

class LightPoseNode : public rclcpp::Node {
private:
  // attributes
  rclcpp::Publisher<geometry_msgs::msg::PointStamped>::SharedPtr publisher;
  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription;
  rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr
      publish_with_mark; // to visually check
  geometry_msgs::msg::PointStamped center_of_gravity;

  // functions
  void callback(const sensor_msgs::msg::Image &cam_frame);

public:
  LightPoseNode();
};