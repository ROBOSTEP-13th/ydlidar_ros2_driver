/*
 *  YDLIDAR SYSTEM
 *  YDLIDAR ROS 2 Node Client
 *
 *  Copyright 2017 - 2020 EAI TEAM
 *  http://www.eaibot.com
 *
 */

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include <cmath>
#include <algorithm>

#define RAD2DEG(x) ((x)*180./M_PI)

static void scanCb(sensor_msgs::msg::LaserScan::SharedPtr scan) {
  const auto count = scan->time_increment > 0.0
    ? std::min(scan->ranges.size(), static_cast<std::size_t>(
        std::ceil(scan->scan_time / scan->time_increment)))
    : scan->ranges.size();
  printf("[YDLIDAR INFO]: I heard a laser scan %s[%ld]:\n", scan->header.frame_id.c_str(), count);
  printf("[YDLIDAR INFO]: angle_range : [%f, %f]\n", RAD2DEG(scan->angle_min),
         RAD2DEG(scan->angle_max));

  for (std::size_t i = 0; i < count; i++) {
    float degree = RAD2DEG(scan->angle_min + scan->angle_increment * i);
    printf("[YDLIDAR INFO]: angle-distance : [%f, %f]\n", degree, scan->ranges[i]);
  }
}

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);

  auto node = rclcpp::Node::make_shared("ydlidar_ros2_driver_client");

  auto lidar_info_sub = node->create_subscription<sensor_msgs::msg::LaserScan>(
                        "scan", rclcpp::SensorDataQoS(), scanCb);

  rclcpp::spin(node);

  rclcpp::shutdown();


  return 0;
}
