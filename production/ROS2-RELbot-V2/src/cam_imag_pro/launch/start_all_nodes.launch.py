from launch import LaunchDescription
from launch_ros.actions import Node

def  generate_launch_description() :
    return LaunchDescription(
        [   Node(
                package = "image_tools",
                executable = "cam2image",                   
            ),
            Node(
                package = "cam_imag_pro",
                executable= "color_pose_indicator_node",
                remappings= [
                    ("colored_shape_pose", "light_position")]
            ),
            Node(
                package =    "sequence_controller",
                executable = "sequence_controller", 
                remappings=[
                    ("left_motor_setpoint_vel", "/input/left_motor/setpoint_vel"),
                    ("right_motor_setpoint_vel", "/input/right_motor/setpoint_vel"),]
            ), 
            Node (
                package= "connector_node",
                executable= "connector_node"
            ),
            #  Node (
            #     package="ros_xeno_bridge",
            #     executable="RosXenoBridge",                   
            # ),
             Node (
                package="image_tools",
                executable="showimage",                    # original camera output
            ),
            Node (
                package="image_tools",
                executable="showimage",
                remappings=[
                    ("/image", "/image_with_marker")      # image with center of gravity 
            ]
            ),
        ]
    )

