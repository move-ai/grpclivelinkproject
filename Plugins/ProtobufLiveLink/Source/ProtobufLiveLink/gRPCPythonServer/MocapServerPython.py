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
        self.mocap_stream = MocapExchange_resources.read_frame_data_pkl("proto_python/mocap.pkl") # list type
        self.structures_no_names = MocapExchange_resources.read_structure_data_pkl("proto_python/mocap.pkl")
        self.structure_response = MocapExchange_pb2.StructureResponse()

        # print(type(self.structures))
        # for i in range(10000):
        #     new_response = MocapExchange_pb2.MocapStreamResponse()
        #     new_response.CopyFrom(self.mocap_stream[0])
        #     new_response.poses[0].joints[5].transform.orientation.rotationValues[0] = i*10
        #     new_response.poses[0].joints[5].transform.translation.x = i*10
        #     new_response.poses[0].joints[0].transform.translation.x = i
        #     self.mocap_stream.append(new_response)
        
        structures_with_names = []
        for i, structure in enumerate(self.structures_no_names.structures):
            new_structure = MocapExchange_pb2.Structure()
            new_structure.CopyFrom(structure)
            new_structure.structureId = i
            new_structure.name = f"Test_{i}"
            structures_with_names.append(new_structure)
        
        self.structure_response.structures.extend(structures_with_names)
        # print(type(self.structures))

    def GetMocapStream(self, request, context):
        # print(request)
        times_to_repeat = 100
        for response in (self.mocap_stream * times_to_repeat):
            time.sleep(1/60)
            yield response

    def GetStructure(self, request, context):
        return(self.structure_response)


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