from concurrent import futures
import logging
import time
import copy
import argparse
import grpc
from pathlib import Path
import MocapExchange_pb2
import MocapExchange_pb2_grpc
import MocapExchange_resources
from typing import Dict
import datetime


class MocapServerServicer(MocapExchange_pb2_grpc.MocapServerServicer):
    def __init__(self, data_path: Path):
        self.mocap_stream = MocapExchange_resources.read_frame_data_pkl(str(data_path)) # list type
        self.id_2_structures: Dict[int, MocapExchange_pb2.Structure] = MocapExchange_resources.read_structure_data_pkl(str(data_path))

    
    def GetMocapStream(self, request, context):
        log = Path("/mnt/Shared/moveai/repos/grpclivelinkproject_win_427/Plugins/ProtobufLiveLink/Source/ProtobufLiveLink/gRPCPythonServer/time_log.txt")
        with open(log, "w") as file:
            # print(request)
            times_to_repeat = 100
            for response in (self.mocap_stream * times_to_repeat):
                time.sleep(1)
                print("Timestamp", response.poses[0].timestamp)
                current_time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")
                print("Server time", current_time)
                file.write(f"{current_time}\n")
                file.write(f"Timestamp: {response.poses[0].timestamp}\n")
                yield response

    def GetStructure(self, request, context):
        structures = []
        print(f'GetStructure request: {request}')
        for s_id in request.structureId:
            if s_id not in self.id_2_structures:
                print(f'Client requested for unavailable structure with ID {s_id}')
                continue
            structures.append(self.id_2_structures[s_id])
        print(f'sent the structures to client: {structures}')
        response = MocapExchange_pb2.StructureResponse(structures=structures)
        return response


def serve(data_path: Path):
    print('Starting gRPC Python server')
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    MocapExchange_pb2_grpc.add_MocapServerServicer_to_server(MocapServerServicer(data_path), server)
    server.add_insecure_port('0.0.0.0:54321')
    server.start()
    server.wait_for_termination()


def parse_args():
    parser = argparse.ArgumentParser(description='Process some integers.')
    parser.add_argument('-i', '--input', type=str)
    return parser.parse_args()


if __name__ == '__main__':
    logging.basicConfig()
    args = parse_args()
    serve(args.input)
