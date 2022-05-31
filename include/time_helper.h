// Copyright (c) 2021 Horizon Robotics.All Rights Reserved.
//
// The material in this file is confidential and contains trade secrets
// of Horizon Robotics Inc. This is proprietary information owned by
// Horizon Robotics Inc. No part of this work may be disclosed,
// reproduced, copied, transmitted, or used in any way for any purpose,
// without the express written permission of Horizon Robotics Inc.

#ifndef TIMER_HELPER__H_
#define TIMER_HELPER__H_

#include <chrono>

class TimeHelper {
 public:
  static uint64_t GetCurrentTimestampMicroSec() {
    auto time_now = std::chrono::system_clock::now();
    auto duration_in_micro_sec =
        std::chrono::duration_cast<std::chrono::microseconds>(
            time_now.time_since_epoch());
    return static_cast<uint64_t>(duration_in_micro_sec.count());
  }

  static uint64_t GetCurrentTimestampMillSec() {
    auto time_now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        time_now.time_since_epoch());
    return static_cast<uint64_t>(duration.count());
  }
};
#endif  // TIMER_HELPER__H_
