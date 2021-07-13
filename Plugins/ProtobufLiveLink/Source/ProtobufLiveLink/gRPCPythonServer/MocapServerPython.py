from concurrent import futures
import logging

import grpc

# from proto_python import MocapExchange_pb2
from proto_python import MocapExchange_pb2_grpc
from proto_python import MocapExchange_resources


class MocapServerServicer(MocapExchange_pb2_grpc.MocapServerServicer):
    def __init__(self):
        self.responses = MocapExchange_resources.read_poses_data() # list type
        # print(type(self.responses))
        # for response in self.responses:
        #     print('type', type(response)) # proto Pose type
        #     # print(response)

    def GetMocapStream(self, request, context):
        # print(request)
        for response in self.responses:
            yield response

def serve():
    print('Starting gRPC Python server')
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    MocapExchange_pb2_grpc.add_MocapServerServicer_to_server(
        MocapServerServicer(), server)
    server.add_insecure_port('[::]:50052')
    server.start()
    server.wait_for_termination()


if __name__ == '__main__':
    logging.basicConfig()
    serve()