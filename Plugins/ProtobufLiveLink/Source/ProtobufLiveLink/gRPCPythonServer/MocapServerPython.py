from concurrent import futures
import logging
import time
import copy
import argparse
import grpc

from proto_python import MocapExchange_pb2
from proto_python import MocapExchange_pb2_grpc
from proto_python import MocapExchange_resources


class MocapServerServicer(MocapExchange_pb2_grpc.MocapServerServicer):
    def __init__(self):
        self.mocap_stream = MocapExchange_resources.read_poses_data() # list type
        self.structures = MocapExchange_resources.read_structure_data()

        # print(type(self.mocap_stream[0].poses[0].joints[0].transform))
        # print(type(self.mocap_stream[0]))
        # print(self.mocap_stream[0].poses[0].joints[0].transform.translation.x)
        for i in range(10000):
            new_response = MocapExchange_pb2.MocapStreamResponse()
            new_response.CopyFrom(self.mocap_stream[0])
            new_response.poses[0].joints[5].transform.orientation.rotationValues[0] = i*10
            new_response.poses[0].joints[5].transform.translation.x = i*10
            new_response.poses[0].joints[0].transform.translation.x = i
            self.mocap_stream.append(new_response)
        
        for i in range(1, 2):
            new_structure = MocapExchange_pb2.Structure()
            new_structure.CopyFrom(self.structures.structures[0])
            new_structure.structureId = i
            new_structure.name = f"Test_{i}"
            self.structures.structures.append(new_structure)
        
        # print(type(self.structures))

    def GetMocapStream(self, request, context):
        # print(request)
        for response in self.mocap_stream:
            time.sleep(1)
            yield response

    def GetStructure(self, request, context):
        return(self.structures)


def serve():
    print('Starting gRPC Python server')
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    MocapExchange_pb2_grpc.add_MocapServerServicer_to_server(
        MocapServerServicer(), server)
    server.add_insecure_port('[::]:54321')
    server.start()
    server.wait_for_termination()


if __name__ == '__main__':
    logging.basicConfig()
    serve()