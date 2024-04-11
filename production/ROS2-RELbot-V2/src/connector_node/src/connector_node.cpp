#include "custom_msgs/msg/ros2_xeno.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"
#include <chrono>
#include <functional>
#include <memory>
#include <random>
#include <string>

using namespace std::chrono_literals;
using namespace std::placeholders;

class ConnectorNode : public rclcpp::Node {
public:
  const std::string TOPIC_NAME_SETPOINTS = "Ros2Xeno";
  const std::string LEFT_MOTOR_TOPIC = "/input/left_motor/setpoint_vel";
  const std::string RIGHT_MOTOR_TOPIC = "/input/right_motor/setpoint_vel";
  ConnectorNode() : Node("sequence_to__xenobridge_connector") {

    // create the velocity publisher to ros2xeno
    vel_setpoints_publisher =
        this->create_publisher<custom_msgs::msg::Ros2Xeno>(TOPIC_NAME_SETPOINTS,
                                                           10);

    // incoming velocites from sequence controller
    left_motor_vel_pub = this->create_subscription<std_msgs::msg::Float64>(
        LEFT_MOTOR_TOPIC, 1,
        std::bind(&ConnectorNode::left_callback, this, _1));

    right_motor_vel_pub = this->create_subscription<std_msgs::msg::Float64>(
        LEFT_MOTOR_TOPIC, 1,
        std::bind(&ConnectorNode::left_callback, this, _1));
    ;
  }

  // callback when left motor setpoint published by sequence controller
  void left_callback(const std_msgs::msg::Float64 left_vel) {
    this->left_vel = left_vel.data;

    // set setpoints and publish TODO: remove this
    // auto vel_setpoints = custom_msgs::msg::Ros2Xeno();
    // vel_setpoints.x = this->left_vel;
    // vel_setpoints.y = this->right_vel;
    // vel_setpoints_publisher->publish(vel_setpoints);
  }

  // callback when left motor setpoint published by sequence controller
  void right_callback(const std_msgs::msg::Float64 right_vel) {
    this->right_vel = right_vel.data;
    // set setpoints and publish
    auto vel_setpoints = custom_msgs::msg::Ros2Xeno();
    vel_setpoints.x = this->left_vel;
    vel_setpoints.y = this->right_vel;
    vel_setpoints_publisher->publish(vel_setpoints);
  }

private:
  double left_vel = 0;
  double right_vel = 0;
  rclcpp::Publisher<custom_msgs::msg::Ros2Xeno>::SharedPtr
      vel_setpoints_publisher;
  rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr left_motor_vel_pub;
  rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr right_motor_vel_pub;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ConnectorNode>());
  rclcpp::shutdown();
  return 0;
}
