from __future__ import print_function

import logging

import grpc

from proto_python import MocapExchange_pb2
from proto_python import MocapExchange_pb2_grpc
# from proto_python import MocapExchange_resources

def print_response(stub):
    request = MocapExchange_pb2.MocapStreamRequest(
        fps = 24
    )

    responses = stub.GetMocapStream(request)

    for response in responses:
        print(response)

def run():
    with grpc.insecure_channel('localhost:50052') as channel:
        stub = MocapExchange_pb2_grpc.MocapServerStub(channel)
        print("-------------- Response --------------")
        print_response(stub)

if __name__ == '__main__':
    logging.basicConfig()
    run()
