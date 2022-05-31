// Copyright (c) 2021 Horizon Robotics.All Rights Reserved.
//
// The material in this file is confidential and contains trade secrets
// of Horizon Robotics Inc. This is proprietary information owned by
// Horizon Robotics Inc. No part of this work may be disclosed,
// reproduced, copied, transmitted, or used in any way for any purpose,
// without the express written permission of Horizon Robotics Inc.

#include <sensor_msgs/msg/image.hpp>
#include <string>

#include "ai_msgs/msg/capture_targets.hpp"
#include "ai_msgs/msg/perception_targets.hpp"
#include "functional"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#ifndef SMART_SUBSCRIBER_H_
#define SMART_SUBSCRIBER_H_

using SmartCbType = std::function<void(
    const ai_msgs::msg::PerceptionTargets::ConstSharedPtr &msg)>;

class SmartMsgSubscriber : public rclcpp::Node {
 public:
  SmartMsgSubscriber(const std::string &node_name,
                     SmartCbType smart_cb = nullptr)
      : Node(node_name), smart_cb_(smart_cb) {
    this->declare_parameter<std::string>("ai_msg_sub_topic_name",
                                         ai_msg_sub_topic_name_);
    this->get_parameter<std::string>("ai_msg_sub_topic_name",
                                     ai_msg_sub_topic_name_);

    RCLCPP_INFO_STREAM(rclcpp::get_logger("SmartMsgSubscriber"),
                       "Parameter:"
                           << "\n ai_msg_sub_topic_name: "
                           << ai_msg_sub_topic_name_);
    subscription_ = this->create_subscription<ai_msgs::msg::PerceptionTargets>(
        ai_msg_sub_topic_name_,
        10,
        std::bind(
            &SmartMsgSubscriber::topic_callback, this, std::placeholders::_1));
  }

 private:
  void topic_callback(const ai_msgs::msg::PerceptionTargets::SharedPtr msg);

  std::string ai_msg_sub_topic_name_ = "/hobot_hand_gesture_detection";
  rclcpp::Subscription<ai_msgs::msg::PerceptionTargets>::SharedPtr
      subscription_;

  SmartCbType smart_cb_ = nullptr;
};

#endif
