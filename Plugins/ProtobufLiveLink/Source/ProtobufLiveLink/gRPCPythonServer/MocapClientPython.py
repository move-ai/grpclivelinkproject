from __future__ import print_function

import logging
import copy
import grpc
import argparse
from pathlib import Path
import pickle
from proto_python import MocapExchange_pb2
from proto_python import MocapExchange_pb2_grpc
from google.protobuf.json_format import MessageToJson
from google.protobuf.json_format import MessageToDict

data = {}

def get_structure(stub):
    request = MocapExchange_pb2.StructureRequest(
        structureId = [0, 1, 2]
    )
    response = stub.GetStructure(request)
    print("______________")
    print(response)
    return response.SerializeToString()


def get_mocap_stream(stub):
    request = MocapExchange_pb2.MocapStreamRequest(
        fps = 12
    )

    responses = stub.GetMocapStream(request)

    frames = []
    try:
        for response in responses:
            print(f"Received frame. n_poses = {len(response.poses)}" )
            # print(response.poses)
            frames.append(response.SerializeToString())
    except Exception as exp:
        print(exp)
        pass
    return frames

def run(args):

    out_path = Path(args.out_file)

    with grpc.insecure_channel('localhost:54321') as channel:
        stub = MocapExchange_pb2_grpc.MocapServerStub(channel)
        print("-------------- get_structure --------------")
        model = get_structure(stub)
        print("-------------- get_mocap_stream --------------")
        frames_poses = get_mocap_stream(stub)

        data = {"model": model, "frames_poses": frames_poses}
        with open(out_path, 'wb') as file:
            pickle.dump(data, file)

def parse_args():
    parser = argparse.ArgumentParser(description='Process some integers.')
    parser.add_argument('--out_file', type=str, default='./grpc_mocap_data.pkl', help='an integer for the accumulator')
    return parser.parse_args()
    
if __name__ == '__main__':
    logging.basicConfig()
    run(parse_args())
