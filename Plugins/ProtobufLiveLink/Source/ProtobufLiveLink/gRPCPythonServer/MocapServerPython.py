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
"""The Python implementation of the gRPC route guide server."""

from concurrent import futures
import time
import math
import logging

import grpc
import sys
import os

from proto_python import MocapExchange_pb2
from proto_python import MocapExchange_pb2_grpc
from proto_python import MocapExchange_resources


class MocapServerServicer(MocapExchange_pb2_grpc.MocapServerServicer):
    def __init__(self):
        self.poses = MocapExchange_resources.read_poses_data()

    def GetMocapStream(self, request, context):
        for pose in self.poses:
            yield pose

def serve():
    print('Starting gRPC Python server')
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    MocapExchange_pb2_grpc.add_MocapServerServicer_to_server(
        MocapServerServicer(), server)
    server.add_insecure_port('[::]:50051')
    server.start()
    server.wait_for_termination()


if __name__ == '__main__':
    logging.basicConfig()
    serve()