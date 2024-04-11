from launch import LaunchDescription
from launch_ros.actions import Node

def  generate_launch_description() :
    return LaunchDescription(
        [   
            Node(
                package = "test_sequence_controller",
                executable = "test_sequence_controller",     
                parameters=[
                    {"test_case": 1},  # test mode
                    {"set_value": 100} # velocity setpoint
                    ]  
            ),         
            Node (
                package="ros_xeno_bridge",
                executable="RosXenoBridge",                     
            )
        ]
    )
