// Copyright (c) 2021 Horizon Robotics.All Rights Reserved.
//
// The material in this file is confidential and contains trade secrets
// of Horizon Robotics Inc. This is proprietary information owned by
// Horizon Robotics Inc. No part of this work may be disclosed,
// reproduced, copied, transmitted, or used in any way for any purpose,
// without the express written permission of Horizon Robotics Inc.

#include "include/util.h"

#include <iostream>
#include <tuple>

// 以pt1为基准
float CalAngelOfTwoVector(const cv::Point2f &c,
                          const cv::Point2f &pt1,
                          const cv::Point2f &pt2,
                          bool clock_wise) {
  float theta =
      atan2(pt1.x - c.x, pt1.y - c.y) - atan2(pt2.x - c.x, pt2.y - c.y);
  if (theta > CV_PI) theta -= 2 * CV_PI;
  if (theta < -CV_PI) theta += 2 * CV_PI;

  theta = theta * 180.0 / CV_PI;

  if (theta < 0) {
    theta = (-1) * theta;
  }

  if (!clock_wise) {
    // anti clock wise
    theta = 360 - theta;
  }
  return theta;
}
