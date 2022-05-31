// Copyright (c) 2021 Horizon Robotics.All Rights Reserved.
//
// The material in this file is confidential and contains trade secrets
// of Horizon Robotics Inc. This is proprietary information owned by
// Horizon Robotics Inc. No part of this work may be disclosed,
// reproduced, copied, transmitted, or used in any way for any purpose,
// without the express written permission of Horizon Robotics Inc.

#include <fstream>
#include <memory>
#include <sstream>

#include "include/body_tracking.h"

int main(int argc, char *argv[]) {
  std::stringstream ss;
  ss << "\n\tThis is body tracking package.\n\n"
     << "\tgesture strategy usage\n"
     << "\nWake up gesture is \"Okay\".\n"
     << "Cancel gesture is \"Palm\".\n"
     << "Control will be reset if body is lost.\n"
     << "============================================\n";
  std::cout << ss.str() << std::endl;

  rclcpp::init(argc, argv);

  rclcpp::executors::SingleThreadedExecutor exec;
  auto nodes = TrackingManager::Instance()->GetNodes();
  for (auto &node : nodes) {
    exec.add_node(node);
  }

  auto smart_msg_subscriber = std::make_shared<SmartMsgSubscriber>(
      "ai_msg_sub_node",
      std::bind(&TrackingManager::FeedSmart,
                TrackingManager::Instance(),
                std::placeholders::_1));

  exec.add_node(smart_msg_subscriber);
  exec.spin();

  // release node before shutdown!
  TrackingManager::Instance()->Release();

  rclcpp::shutdown();

  std::cout << "tracking node exit!\n";
  return 0;
}
