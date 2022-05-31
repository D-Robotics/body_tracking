// Copyright (c) 2021 Horizon Robotics.All Rights Reserved.
//
// The material in this file is confidential and contains trade secrets
// of Horizon Robotics Inc. This is proprietary information owned by
// Horizon Robotics Inc. No part of this work may be disclosed,
// reproduced, copied, transmitted, or used in any way for any purpose,
// without the express written permission of Horizon Robotics Inc.

#include "include/smart_subscriber.h"

void SmartMsgSubscriber::topic_callback(
    const ai_msgs::msg::PerceptionTargets::SharedPtr msg) {
  std::stringstream ss;
  ss << "Recved ai msg"
     << ", frame_id: " << msg->header.frame_id
     << ", stamp: " << msg->header.stamp.sec << "_" << msg->header.stamp.nanosec
     << ", targets size: " << msg->targets.size() << "\n";
  for (const auto& tar : msg->targets) {
    ss << "\t track_id: " << tar.track_id << ", type: " << tar.type;
    ss << " has roi num: " << tar.rois.size();
    for (const auto& roi : tar.rois) {
      ss << ", roi type: " << roi.type;
    }
    ss << ", has attr num: " << tar.attributes.size();
    for (const auto& attr : tar.attributes) {
      ss << ", attr type: " << attr.type << ", val: " << attr.value;
    }
    ss << "\n";
  }
  RCLCPP_INFO(rclcpp::get_logger("SmartMsgSubscriber"), "%s", ss.str().c_str());

  if (smart_cb_) {
    smart_cb_(msg);
  } else {
    RCLCPP_WARN(rclcpp::get_logger("SmartMsgSubscriber"),
                "smart_cb_ was not set");
  }
}
