from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess

def generate_launch_description() :
    return LaunchDescription(
        [   
        Node(
            package="image_tools",
            executable="cam2image"
        ),
        Node(
            package="image_tools",
            executable="showimage"
        ),
        Node(
            package="cam_imag_pro",
            executable="light_pos_indicator_node"
            ),
        Node (
            package="image_tools",
            executable="showimage",
            name = 'image_with_marker_display',
            remappings=[
                ("/image", "/image_with_marker")
            ]
            )
        ]
    )