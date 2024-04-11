#include "color_pose_indicator_node.hpp"

ColorPoseNode::ColorPoseNode() : Node("color_pose_indicator_node") {
  pose_publisher =
      this->create_publisher<geometry_msgs::msg::Point>("light_position", 1);
  publish_with_mark =
      this->create_publisher<sensor_msgs::msg::Image>("image_with_marker", 10);
  subscription = this->create_subscription<sensor_msgs::msg::Image>(
      "image", 10, std::bind(&ColorPoseNode::callback, this, _1));
  this->declare_parameter("threshold", 155);
}

void ColorPoseNode::callback(const sensor_msgs::msg::Image &cam_frame) {

  // convert it to hsv image
  cv_bridge::CvImagePtr cv_image =
      cv_bridge::toCvCopy(cam_frame, sensor_msgs::image_encodings::BGR8);
  cv::Mat frame = cv_image->image;

  cv::Mat hsv_image;
  cv::cvtColor(frame, hsv_image, cv::COLOR_BGR2HSV);

  // convert to binary image

  cv::Scalar lower_bound = cv::Scalar(35, 100, 100);
  cv::Scalar upper_bound = cv::Scalar(85, 255, 255);

  cv::Mat green_mask;
  cv::inRange(hsv_image, lower_bound, upper_bound, green_mask);

  cv::Mat blackn_white;
  cv::threshold(green_mask, blackn_white, 0, 255, cv::THRESH_BINARY);

  // calculate the moment of the image
  cv::Moments m = cv::moments(blackn_white, true);

  // check if there are pixels with non-zero intensity
  if (m.m00 > 0) {
    // calculate the "center of gravity" of the green color
    int x = int(m.m10 / m.m00);
    int y = int(m.m01 / m.m00);

    // publish it to the light_pose topic
    geometry_msgs::msg::Point center_of_gravity;
    center_of_gravity.x = x;
    center_of_gravity.y = y;

    pose_publisher->publish(center_of_gravity);

    // prepare and send image with box around the "center of gravity" for visual
    // inspection
    cv::Mat colored_image;
    cv::cvtColor(blackn_white, colored_image, cv::COLOR_GRAY2BGR);
    cv::rectangle(colored_image, cv::Point(x - 10, y - 10),
                  cv::Point(x + 10, y + 10), cv::Scalar(0, 255, 0), 3);

    cv_bridge::CvImage colored_image_bridge(cam_frame.header, "bgr8",
                                            colored_image);
    sensor_msgs::msg::Image ros_image;
    colored_image_bridge.toImageMsg(ros_image);
    publish_with_mark->publish(ros_image);
  };
}

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ColorPoseNode>());
  rclcpp::shutdown();
  return 0;
}
