// Copyright (c) 2021 Horizon Robotics.All Rights Reserved.
//
// The material in this file is confidential and contains trade secrets
// of Horizon Robotics Inc. This is proprietary information owned by
// Horizon Robotics Inc. No part of this work may be disclosed,
// reproduced, copied, transmitted, or used in any way for any purpose,
// without the express written permission of Horizon Robotics Inc.

#ifndef ROBOT_CTRL_NODE_H_
#define ROBOT_CTRL_NODE_H_
#include <string>

#include "include/common.h"

class RobotCmdVelNode : public rclcpp::Node {
 public:
  explicit RobotCmdVelNode(const std::string &node_name) : Node(node_name) {
    RCLCPP_INFO(rclcpp::get_logger("RobotCmdVelNode"),
                "RobotCmdVelNode construct");
    twist_publisher_ = this->create_publisher<Twist>(twist_pub_topic_name_, 10);
  }

  void RobotCtl(const Twist &msg) const {
    std::stringstream ss;
    ss << "RobotCtl"
       << ", angular: " << msg.angular.x << " " << msg.angular.y << " "
       << msg.angular.z << ", linear: " << msg.linear.x << " " << msg.linear.y
       << " " << msg.linear.z
       << ", pub twist ts: " << TimeHelper::GetCurrentTimestampMicroSec();
    RCLCPP_WARN(rclcpp::get_logger("RobotCmdVelNode"), "%s", ss.str().data());

    twist_publisher_->publish(msg);
  }

 private:
  rclcpp::Publisher<Twist>::SharedPtr twist_publisher_ = nullptr;
  // topic name for turtle sim is "turtle1/cmd_vel" and for robot is "/cmd_vel"
  const std::string twist_pub_topic_name_ = "/cmd_vel";
};

#endif
