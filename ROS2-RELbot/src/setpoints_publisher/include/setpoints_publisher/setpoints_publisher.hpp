#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <random>
#include <string>
#include "geometry_msgs/msg/point_stamped.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"
#include "custom_msgs/msg/ros2_xeno.hpp"

using namespace std::chrono_literals;
using namespace std::placeholders;

class SetpointsPublisher : public rclcpp::Node {
private:
  
  // create a set point publisher to the Ros2Xeno topic that needs to be send to the bridge
  rclcpp::Publisher<custom_msgs::msg::Ros2Xeno>::SharedPtr vel_setpoint_publisher;

  // create light pose reciever from light pose indicator node
  rclcpp::Subscription<geometry_msgs::msg::PointStamped>::SharedPtr
      light_pose_topic;

  // create callback for coming light position
  void light_pose_callback(
      const geometry_msgs::msg::PointStamped &light_pose);

public:  
  const std::string  TOPIC_NAME_SETPOINTS = "Ros2Xeno";                             
  SetpointsPublisher();
};
