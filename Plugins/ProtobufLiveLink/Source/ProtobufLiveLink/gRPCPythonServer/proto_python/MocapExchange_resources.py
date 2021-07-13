# Copyright 2015 gRPC authors.
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
"""Common resources used in the gRPC route guide example."""

import yaml

from proto_python import MocapExchange_pb2

def read_poses_data():
    poses_list = []
    with open("proto_python/poses_data.yaml") as route_guide_db_file:
        for pose_from_file in yaml.safe_load(route_guide_db_file):
          pose = MocapExchange_pb2.Pose(subjectId = pose_from_file["subjectId"],
                                        timestamp = pose_from_file["timestamp"],
                                        joints = [
                                          MocapExchange_pb2.Joint(jointId = joint_from_file["jointId"],
                                                                  transform = MocapExchange_pb2.Transform(
                                                                    translation = MocapExchange_pb2.Translation(
                                                                      x = joint_from_file["transform"]["translation"]["x"],
                                                                      y = joint_from_file["transform"]["translation"]["y"],
                                                                      z = joint_from_file["transform"]["translation"]["z"],
                                                                    ),
                                                                    orientation = MocapExchange_pb2.Orientation(
                                                                      type = MocapExchange_pb2.Orientation.RotationType.Value(joint_from_file["transform"]["orientation"]["type"]),
                                                                      values = joint_from_file["transform"]["orientation"]["values"]
                                                                    )
                                                                  )

                                          ) for joint_from_file in pose_from_file["joints"]
                                        ]
                                        )
          poses_list.append(pose)
    return poses_list