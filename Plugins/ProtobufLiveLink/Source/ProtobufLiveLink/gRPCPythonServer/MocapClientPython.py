from __future__ import print_function

import logging

import grpc

from proto_python import MocapExchange_pb2
from proto_python import MocapExchange_pb2_grpc
# from proto_python import MocapExchange_resources

def get_structure(stub):
    request = MocapExchange_pb2.StructureRequest(
        id = [0, 1, 2]
    )

    response = stub.GetStructure(request)
    print("______________")
    print(response)


def get_mocap_stream(stub):
    request = MocapExchange_pb2.MocapStreamRequest(
        fps = 12
    )

    responses = stub.GetMocapStream(request)

    for response in responses:
        print("______________")
        print(response)

def run():
    with grpc.insecure_channel('localhost:50051') as channel:
        stub = MocapExchange_pb2_grpc.MocapServerStub(channel)
        print("-------------- get_structure --------------")
        get_structure(stub)
        print("-------------- get_mocap_stream --------------")
        get_mocap_stream(stub)


if __name__ == '__main__':
    logging.basicConfig()
    run()
