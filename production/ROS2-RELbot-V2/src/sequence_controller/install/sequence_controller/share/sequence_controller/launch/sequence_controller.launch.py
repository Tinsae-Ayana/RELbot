from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription(
        [
            Node(
                package="RELbot_simulator",
                executable="RELbot_simulator",
                arguments=["--ros-args", "--log-level", "WARN"],
            ),
            Node(
                package="image_tools",
                executable="cam2image",
                arguments=["--ros-args", "--log-level", "WARN"],
            ),
            Node(
                package="position_node",
                executable="position_node",
                remappings=[("input_image", "/output/moving_camera")],
            ),
            Node(
                package="sequence_controller",
                executable="sequence_controller",
                remappings=[
                    ("camera_position", "/output/camera_position"),
                    ("left_motor_setpoint_vel", "/input/right_motor/setpoint_vel"),
                    ("right_motor_setpoint_vel", "/input/right_motor/setpoint_vel"),
                ],
            ),
        ]
    )
