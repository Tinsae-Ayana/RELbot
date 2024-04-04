#include "setpoints_publisher.hpp"

SetpointsPublisher::SetpointsPublisher() : Node("setpoints_publisher") {

  // create the velocity publisher    
  vel_setpoint_publisher = this->create_publisher<custom_msgs::msg::Ros2Xeno>(TOPIC_NAME_SETPOINTS,10);
  
  //  create the subsriber to the light position topic
  light_pose_topic =
      this->create_subscription<geometry_msgs::msg::PointStamped>(
          "light_pose", 10,
          std::bind(&SetpointsPublisher::light_pose_callback, this, _1));
}

void SetpointsPublisher::light_pose_callback(
    const geometry_msgs::msg::PointStamped &light_pose) {

  double x_difference = (double)(500 - light_pose.point.x); // TODO: to be changed
 
  auto left_motor_vel  = -0.01 * x_difference; 
  auto right_motor_vel =  0.01 * x_difference; 

  auto vel_setpoints = custom_msgs::msg::Ros2Xeno();
  
  vel_setpoints.x = right_motor_vel;
  vel_setpoints.y = left_motor_vel;
  RCLCPP_INFO_STREAM(this->get_logger(), "Publishing: '" << vel_setpoints.x << "'");
  RCLCPP_INFO_STREAM(this->get_logger(), "Publishing: '" << vel_setpoints.y << "'");
  vel_setpoint_publisher->publish(vel_setpoints);
}


int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SetpointsPublisher>());
  rclcpp::shutdown();
  return 0;
}
