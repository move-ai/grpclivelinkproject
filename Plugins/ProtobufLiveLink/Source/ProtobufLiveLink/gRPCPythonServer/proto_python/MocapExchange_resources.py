import yaml
import json

from proto_python import MocapExchange_pb2

def read_poses_data():
    poses_list = []
    responses = []
    with open("proto_python/poses_data.yaml") as poses_data:
        # print(json.load(poses_data))
        for pose_from_file in yaml.safe_load(poses_data):
            pose = MocapExchange_pb2.Pose(
                subjectId = pose_from_file["subjectId"],
                timestamp = pose_from_file["timestamp"],
                joints = [
                    MocapExchange_pb2.Joint(
                        jointId = joint_from_file["jointId"],
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

        response = MocapExchange_pb2.MocapStreamResponse(poses = poses_list,
                                                        serverTime = 10)

        responses.append(response)

        return responses

def read_structure_data():
    structure_list = []
    with open("proto_python/structure_data.yaml") as structure_data:
        for structure_from_file in yaml.safe_load(structure_data):
            structure = MocapExchange_pb2.Structure(
                structureId = structure_from_file["structureId"],
                structureType = structure_from_file["structureType"],
                name = structure_from_file["name"],
                links = [
                    MocapExchange_pb2.Link(
                        name = link_from_file["name"],
                        linkId = link_from_file["linkId"],
                        parentLinkId = link_from_file["parentLinkId"],
                        offset = MocapExchange_pb2.Transform(
                            translation = MocapExchange_pb2.Translation(
                                x = link_from_file["offset"]["translation"]["x"],
                                y = link_from_file["offset"]["translation"]["y"],
                                z = link_from_file["offset"]["translation"]["z"],
                            ),
                            orientation = MocapExchange_pb2.Orientation(
                                type = MocapExchange_pb2.Orientation.RotationType.Value(link_from_file["offset"]["orientation"]["type"]),
                                values = link_from_file["offset"]["orientation"]["values"]
                            )
                        ),
                        mass = link_from_file["mass"],
                        size = [size_from_file for size_from_file in link_from_file["size"]],
                        inertialMatrix = [inertialMatrix_from_file for inertialMatrix_from_file in link_from_file["inertialMatrix"]],
                    ) for link_from_file in structure_from_file["links"]
                ],
                joints = [
                    MocapExchange_pb2.JointMeta(
                        jointId = joint_from_file["jointId"],
                        linkId = joint_from_file["linkId"],
                        name = joint_from_file["name"]
                    ) for joint_from_file in structure_from_file["joints"]
                ]
            )

            structure_list.append(structure)

        response = MocapExchange_pb2.StructureResponse(structures = structure_list)

        return response