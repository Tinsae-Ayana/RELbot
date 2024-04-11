#include "setpoints_publisher.hpp"

SetpointsPublisher::SetpointsPublisher() : Node("setpoints_publisher") {

  // create the velocity publisher
  vel_setpoint_publisher = this->create_publisher<custom_msgs::msg::Ros2Xeno>(
      TOPIC_NAME_SETPOINTS, 10);

  //  create the subsriber to the light position topic
  color_pose_topic_sub =
      this->create_subscription<geometry_msgs::msg::PointStamped>(
          "colored_shape_pose", 10,
          std::bind(&SetpointsPublisher::color_pose_callback, this, _1));
}

void SetpointsPublisher::color_pose_callback(
    const geometry_msgs::msg::PointStamped &color_pose) {

  double x_difference =
      (double)(color_pose.point.z -
               color_pose.point
                   .x); // color_pose.point.z is the center of image in x axis
  double some_forward_motion = 10; // TODO: experiment with this

  auto left_motor_vel =
      0.01 * x_difference + some_forward_motion; // TODO: check the signs
  auto right_motor_vel = -0.01 * x_difference + some_forward_motion;

  // set setpoints
  auto vel_setpoints = custom_msgs::msg::Ros2Xeno();
  vel_setpoints.x = left_motor_vel;
  vel_setpoints.y = right_motor_vel;

  RCLCPP_INFO_STREAM(this->get_logger(),
                     "Publishing: '" << vel_setpoints.x << "'");
  RCLCPP_INFO_STREAM(this->get_logger(),
                     "Publishing: '" << vel_setpoints.y << "'");

  // publishe setpoints
  vel_setpoint_publisher->publish(vel_setpoints);
}

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SetpointsPublisher>());
  rclcpp::shutdown();
  return 0;
}
