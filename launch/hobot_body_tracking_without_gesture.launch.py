# Copyright (c) 2022，Horizon Robotics.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os

from launch import LaunchDescription
from launch_ros.actions import Node

from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python import get_package_share_directory

def generate_launch_description():
    web_service_launch_include = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(
                get_package_share_directory('websocket'),
                'launch/hobot_websocket_service.launch.py'))
    )

    # 人体跟随
    body_tracking_node = Node(
        package='body_tracking',
        executable='body_tracking',
        output='screen',
        parameters=[
            {"ai_msg_sub_topic_name": "/hobot_mono2d_body_detection"},
            {"activate_wakeup_gesture": 0},
            {"img_width": 960},
            {"img_height": 544},
            {"track_serial_lost_num_thr": 30},
            {"move_step": 0.5},
            {"rotate_step": 0.5},
            {"activate_robot_move_thr": 5}
        ],
        arguments=['--ros-args', '--log-level', 'warn']
    )

    # mipi cam图片发布
    mipi_node = Node(
        package='mipi_cam',
        executable='mipi_cam',
        output='screen',
        parameters=[
            {"out_format": "nv12"},
            {"image_width": 960},
            {"image_height": 544},
            {"io_method": "shared_mem"},
            {"video_device": "F37"}
        ],
        arguments=['--ros-args', '--log-level', 'error']
    )

    # usb cam图片发布
    usb_node = Node(
        package='hobot_usb_cam',
        executable='hobot_usb_cam',
        name='hobot_usb_cam',
        parameters=[
                    {"frame_id": "default_usb_cam"},
                    {"image_height": 480},
                    {"image_width": 640},
                    {"zero_copy": False},
                    {"video_device": "/dev/video8"}
                    ],
        arguments=['--ros-args', '--log-level', 'error']
    )

    # nv12->jpeg图片编码&发布
    jpeg_codec_node = Node(
        package='hobot_codec',
        executable='hobot_codec_republish',
        output='screen',
        parameters=[
            {"channel": 1},
            {"in_mode": "shared_mem"},
            {"in_format": "nv12"},
            {"out_mode": "ros"},
            {"out_format": "jpeg"},
            {"sub_topic": "/hbmem_img"},
            {"pub_topic": "/image"}
        ],
        arguments=['--ros-args', '--log-level', 'error']
    )

    # jpeg->nv12图片解码&发布
    nv12_codec_node = Node(
        package='hobot_codec',
        executable='hobot_codec_republish',
        output='screen',
        parameters=[
            {"channel": 1},
            {"in_mode": "ros"},
            {"in_format": "jpeg"},
            {"out_mode": "shared_mem"},
            {"out_format": "nv12"},
            {"sub_topic": "/image"},
            {"pub_topic": "/hbmem_img"}
        ],
        arguments=['--ros-args', '--log-level', 'error']
    )

    # 单目rgb人体、人头、人脸、人手框和人体关键点检测算法
    mono2d_body_det_node = Node(
        package='mono2d_body_detection',
        executable='mono2d_body_detection',
        output='screen',
        parameters=[
            {"ai_msg_pub_topic_name": "/hobot_mono2d_body_detection"}
        ],
        arguments=['--ros-args', '--log-level', 'error']
    )

    # web展示
    web_node = Node(
        package='websocket',
        executable='websocket',
        output='screen',
        parameters=[
            {"image_topic": "/image"},
            {"image_type": "mjpeg"},
            {"smart_topic": "/hobot_mono2d_body_detection"}
        ],
        arguments=['--ros-args', '--log-level', 'error']
    )

    camera_type = os.getenv('CAM_TYPE')
    print("camera_type is ", camera_type)
    cam_node = mipi_node
    camera_type_mipi = True
    if camera_type == "usb":
        print("using usb cam")
        cam_node = usb_node
        camera_type_mipi = False
    elif camera_type == "mipi":
        print("using mipi cam")
        cam_node = mipi_node
        camera_type_mipi = True
    else:
        print("invalid camera_type ", camera_type, ", which is set with export CAM_TYPE=usb/mipi, using default mipi cam")
        cam_node = mipi_node
        camera_type_mipi = True

    if camera_type_mipi:
        return LaunchDescription([
            web_service_launch_include,
            # 图片发布
            cam_node,
            # 图片编解码&发布
            jpeg_codec_node,
            # 启动人体和关键点检测算法
            mono2d_body_det_node,
            # 人体跟随
            body_tracking_node,
            # 启动web展示
            web_node
        ])
    else:
        return LaunchDescription([
            web_service_launch_include,
            # 图片发布
            cam_node,
            # 图片编解码&发布
            nv12_codec_node,
            # 启动人体和关键点检测算法
            mono2d_body_det_node,
            # 人体跟随
            body_tracking_node,
            # 启动web展示
            web_node
        ])
 