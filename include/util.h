// Copyright (c) 2021 Horizon Robotics.All Rights Reserved.
//
// The material in this file is confidential and contains trade secrets
// of Horizon Robotics Inc. This is proprietary information owned by
// Horizon Robotics Inc. No part of this work may be disclosed,
// reproduced, copied, transmitted, or used in any way for any purpose,
// without the express written permission of Horizon Robotics Inc.

#ifndef UTIL_H_
#define UTIL_H_
#include <algorithm>
#include <tuple>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

// cal the angel of lines pt1-c and pt2-c using pt1 as base
float CalAngelOfTwoVector(const cv::Point2f &c,
                          const cv::Point2f &pt1,
                          const cv::Point2f &pt2,
                          bool clock_wise = true);
#endif
