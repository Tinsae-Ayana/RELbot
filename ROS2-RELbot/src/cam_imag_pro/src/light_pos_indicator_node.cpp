#include "light_pos_indicator_node.hpp"

LightPoseNode::LightPoseNode() : Node("light_pose_indicator_node") {
  publisher = this->create_publisher<geometry_msgs::msg::PointStamped>(
      "light_pose", 10);
  publish_with_mark =
      this->create_publisher<sensor_msgs::msg::Image>("image_with_marker", 10);
  subscription = this->create_subscription<sensor_msgs::msg::Image>(
      "image", 10, std::bind(&LightPoseNode::callback, this, _1));
  this->declare_parameter("threshold", 155);
}

void LightPoseNode::callback(const sensor_msgs::msg::Image &cam_frame) {

  // convert it to grayimage
  cv_bridge::CvImagePtr cv_image =
      cv_bridge::toCvCopy(cam_frame, sensor_msgs::image_encodings::BGR8);
  cv::Mat frame = cv_image->image;
  cv::Mat grayScale;
  cv::cvtColor(frame, grayScale, cv::COLOR_BGR2GRAY);

  // convert it to black and white image
  cv::Mat blackn_white;
  cv::threshold(grayScale, blackn_white,
                this->get_parameter("threshold").as_int(), 255,
                cv::THRESH_BINARY);

  // calculate the moment of the image
  cv::Moments m = cv::moments(blackn_white, true);

  // check if there are pixels with non-zero intensity
  if (m.m00 > 0) {
    // calculate the "center of gravity" of the white dot
    int x = int(m.m10 / m.m00);
    int y = int(m.m01 / m.m00);

    // publishe it to the light_pose topic
    geometry_msgs::msg::PointStamped center_of_gravity;
    center_of_gravity.point.x = x;
    center_of_gravity.point.y = y;
    center_of_gravity.point.z = 0.0;
    publisher->publish(center_of_gravity);

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
  rclcpp::spin(std::make_shared<LightPoseNode>());
  rclcpp::shutdown();
  return 0;
}
