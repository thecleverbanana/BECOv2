from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import FindExecutable, LaunchConfiguration
from launch_ros.substitutions import FindPackageShare

import os

def generate_launch_description():
    pkg_share = FindPackageShare(package='BECOV2_description').find('BECOV2_description')

    urdf_file = os.path.join(pkg_share, 'urdf', 'BECOV2.urdf')

    return LaunchDescription([
        Node(
            package='gazebo_ros',
            executable='spawn_entity.py',
            arguments=['-entity', 'becov2', '-file', urdf_file],
            output='screen'
        ),
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(
                [FindPackageShare('gazebo_ros'), '/launch', '/gazebo.launch.py']
            )
        )
    ])
