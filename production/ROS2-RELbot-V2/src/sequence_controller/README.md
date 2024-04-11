# Sequence controller

This node implements the closed-loop sequence controller for CBL students. To interface with this node, the following input topic is used:

- `light_position` (`geometry_msgs::msg::Point`): used to get the position of the light in the camera image (in pixels, top left is 0,0) from the node that you created yourselves in the first assignment.

And the following output topics are used:

- `left_motor_setpoint_vel` (`std_msgs::msg::Float64`): velocity for the left motor.
- `right_motor_setpoint_vel` (`std_msgs::msg::Float64`): velocity for the right motor.

This node has two parameters which can be used for tuning the behaviour of the controller:

- `gain`: this is the gain (related to but not equal to tau in exercise 1.2.3) of the P-controller. The initial value is 0.25.
- `width`: this is the width of the camera image. For the simulator, this is 360 pixels. This is also the default value.

To use this node with the simulator, the following line can be used in your launch file to set all the remappings (plus whatever topic name you use for your light position):

```python
remappings=[
    ("left_motor_setpoint_vel", "/input/right_motor/setpoint_vel"),
    ("right_motor_setpoint_vel", "/input/right_motor/setpoint_vel"),
]
```

Note that you need to use `/output/moving_camera` (the output of the simulator) as the input topic for your light position node. If your light position node cannot find a light position, you can set the x-value of the position to -1; then the sequence controller will ignore this value. It will then use the previously known value.

Last but not least, this node does quite a bit of debug printing. If you want to disable this, you can use the following line in your launch file:

```python
arguments=["--ros-args", "--log-level", "WARN"]
```
