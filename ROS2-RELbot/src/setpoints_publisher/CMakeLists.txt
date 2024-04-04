cmake_minimum_required(VERSION 3.8)
project(setpoints_publisher)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(custom_msgs REQUIRED)   # added
add_executable(setpoints_publisher src/setpoints_publisher.cpp)

target_compile_features(setpoints_publisher PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17

ament_target_dependencies( 
  setpoints_publisher
  "rclcpp"
  "std_msgs"
  "geometry_msgs"
  "custom_msgs"    # added
)

target_include_directories(setpoints_publisher  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/setpoints_publisher>)

# Install launch files.
install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}/
)

install(TARGETS setpoints_publisher
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()

