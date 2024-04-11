#include "custom_msgs/msg/ros2_xeno.hpp"
#include "custom_msgs/msg/xeno2_ros.hpp"
#include "rclcpp/rclcpp.hpp"
#include <chrono>
#include <functional>
#include <memory>
#include <random>
#include <string>

using namespace std::chrono_literals;
using namespace std::placeholders;

class TestSeqController : public rclcpp::Node {
public:
  // topic names
  const std::string TOPIC_NAME_SETPOINTS = "Ros2Xeno";
  const std::string TOPIC_NAME_ENCODER = "Xeno2Ros";

  // constructor
  TestSeqController() : Node("test_sequence_controller") {

    // create the velocity publisher to ros2xeno
    vel_setpoints_publisher =
        this->create_publisher<custom_msgs::msg::Ros2Xeno>(TOPIC_NAME_SETPOINTS,
                                                           10);
    // encoder reading
    encoder_reading = this->create_subscription<custom_msgs::msg::Xeno2Ros>(
        TOPIC_NAME_ENCODER, 10,
        std::bind(&TestSeqController::encoder_callback, this, _1));

    // create walltimer
    timer = this->create_wall_timer(
        100ms, std::bind(&TestSeqController::timer_callback, this));

    // declare test_case parameter
    this->declare_parameter("test_case", 1);
    this->declare_parameter("set_value", 100)
  }

private:
  // setpoint publisher
  rclcpp::Publisher<custom_msgs::msg::Ros2Xeno>::SharedPtr
      vel_setpoints_publisher;

  // subscriber to encoder reading
  rclcpp::Subscription<custom_msgs::msg::Xeno2Ros>::SharedPtr encoder_reading;

  //  timer
  rclcpp::TimerBase::SharedPtr timer;

  void encoder_callback(const custom_msgs::msg::Xeno2Ros &reading) {
    RCLCPP_INFO_STREAM(this->get_logger(),
                       "left encoder reading: '" << reading.x << "'");
    RCLCPP_INFO_STREAM(this->get_logger(),
                       "right encoder reading: '" << reading.y << "'");
  }

  // timer callback
  void timer_callback() {

    // get the test case
    auto test_case = this->get_parameter("test_case").as_int();
    auto test_value = this->get_parameter("set_value").as_int();

    // set setpoints and publish
    auto vel_setpoints = custom_msgs::msg::Ros2Xeno();

    switch (test_case) {
    case 1:
      vel_setpoints.x = test_value;
      vel_setpoints.y = test_value;
      break;
    case 2:
      vel_setpoints.x = -test_value;
      vel_setpoints.y = -test_value;
      break;
    case 3:
      vel_setpoints.x = test_value;
      vel_setpoints.y = -test_value;
      break;
    case 4:
      vel_setpoints.x = -test_value;
      vel_setpoints.y = test_value;
    }
    // publish
    vel_setpoints_publisher->publish(vel_setpoints);
  }
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TestSeqController>());
  rclcpp::shutdown();
  return 0;
}
