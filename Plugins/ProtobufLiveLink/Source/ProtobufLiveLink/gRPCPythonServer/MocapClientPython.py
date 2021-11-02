from __future__ import print_function

import logging
import copy
import grpc
import argparse
import time
from pathlib import Path
import pickle
from proto_python import MocapExchange_pb2
from proto_python import MocapExchange_pb2_grpc
from google.protobuf.json_format import MessageToJson
from google.protobuf.json_format import MessageToDict

data = {}

def get_structure(stub):
    try:
        request = MocapExchange_pb2.StructureRequest(
            structureId = [0, 1, 2]
        )
        response = stub.GetStructure(request)
        if(len(response.structures) > 0):
            return response.SerializeToString()
    except:
        pass
    return None

def get_mocap_stream(stub):
    request = MocapExchange_pb2.MocapStreamRequest(
        fps = 12
    )

    responses = stub.GetMocapStream(request)
    # print('responses', responses)
    
    frames = []
    model = None
    try:
        for response in responses:
            print(f"Received frame. n_poses = {len(response.poses)}" )
            # for jnt in response.poses[0].joints:
            #    print(jnt.transform)
            # print(response.poses[0].joints[0].transform.translation)
            frames.append(response.SerializeToString())

            if model is None:
                print("-------------- get_structure --------------")
                model = get_structure(stub)
            
    except Exception as exp:
        pass
    except KeyboardInterrupt:
        pass
    return model, frames


def export_data(model, frames_poses, out_path):
    print(f'exported data to file path: {out_path}')
    data = {"model": model, "frames_poses": frames_poses}
    with open(out_path, 'wb') as file:
        pickle.dump(data, file)

def run(args):
    out_path = Path(args.out_file)
    print('start trying to connect to ip: ' + args.ip)
    model = None
    frames_poses = None
    while True:
        try:
            with grpc.insecure_channel(f'{args.ip}:54321') as channel:
                stub = MocapExchange_pb2_grpc.MocapServerStub(channel)

                print("-------------- get_mocap_stream --------------")
                model, frames_poses = get_mocap_stream(stub)
                export_data(model, frames_poses, out_path)
                break
        except Exception as exp:
            # print(f'failed to connect to IP {args.ip}. Keep connecting')
            # time.sleep(1)
            continue
        except KeyboardInterrupt:
            # quit
            export_data(model, frames_poses, out_path)
            print('quitting')
            return


def parse_args():
    parser = argparse.ArgumentParser(description='Process some integers.')
    parser.add_argument('--ip', type=str)
    parser.add_argument('--out_file', type=str, default='./grpc_mocap_data.pkl', help='an integer for the accumulator')
    return parser.parse_args()
    
if __name__ == '__main__':
    logging.basicConfig()
    run(parse_args())

