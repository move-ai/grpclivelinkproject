# Generated by the gRPC Python protocol compiler plugin. DO NOT EDIT!
"""Client and server classes corresponding to protobuf-defined services."""
import grpc

from proto_python import MocapExchange_pb2 as proto__python_dot_MocapExchange__pb2


class MocapServerStub(object):
    """Missing associated documentation comment in .proto file."""

    def __init__(self, channel):
        """Constructor.

        Args:
            channel: A grpc.Channel.
        """
        self.GetEnvironment = channel.unary_unary(
                '/Mocap.MocapServer/GetEnvironment',
                request_serializer=proto__python_dot_MocapExchange__pb2.EnvironmentRequest.SerializeToString,
                response_deserializer=proto__python_dot_MocapExchange__pb2.Environment.FromString,
                )
        self.GetStructure = channel.unary_unary(
                '/Mocap.MocapServer/GetStructure',
                request_serializer=proto__python_dot_MocapExchange__pb2.StructureRequest.SerializeToString,
                response_deserializer=proto__python_dot_MocapExchange__pb2.StructureResponse.FromString,
                )
        self.GetMocapStream = channel.unary_stream(
                '/Mocap.MocapServer/GetMocapStream',
                request_serializer=proto__python_dot_MocapExchange__pb2.MocapStreamRequest.SerializeToString,
                response_deserializer=proto__python_dot_MocapExchange__pb2.MocapStreamResponse.FromString,
                )


class MocapServerServicer(object):
    """Missing associated documentation comment in .proto file."""

    def GetEnvironment(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def GetStructure(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def GetMocapStream(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')


def add_MocapServerServicer_to_server(servicer, server):
    rpc_method_handlers = {
            'GetEnvironment': grpc.unary_unary_rpc_method_handler(
                    servicer.GetEnvironment,
                    request_deserializer=proto__python_dot_MocapExchange__pb2.EnvironmentRequest.FromString,
                    response_serializer=proto__python_dot_MocapExchange__pb2.Environment.SerializeToString,
            ),
            'GetStructure': grpc.unary_unary_rpc_method_handler(
                    servicer.GetStructure,
                    request_deserializer=proto__python_dot_MocapExchange__pb2.StructureRequest.FromString,
                    response_serializer=proto__python_dot_MocapExchange__pb2.StructureResponse.SerializeToString,
            ),
            'GetMocapStream': grpc.unary_stream_rpc_method_handler(
                    servicer.GetMocapStream,
                    request_deserializer=proto__python_dot_MocapExchange__pb2.MocapStreamRequest.FromString,
                    response_serializer=proto__python_dot_MocapExchange__pb2.MocapStreamResponse.SerializeToString,
            ),
    }
    generic_handler = grpc.method_handlers_generic_handler(
            'Mocap.MocapServer', rpc_method_handlers)
    server.add_generic_rpc_handlers((generic_handler,))


 # This class is part of an EXPERIMENTAL API.
class MocapServer(object):
    """Missing associated documentation comment in .proto file."""

    @staticmethod
    def GetEnvironment(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/Mocap.MocapServer/GetEnvironment',
            proto__python_dot_MocapExchange__pb2.EnvironmentRequest.SerializeToString,
            proto__python_dot_MocapExchange__pb2.Environment.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def GetStructure(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/Mocap.MocapServer/GetStructure',
            proto__python_dot_MocapExchange__pb2.StructureRequest.SerializeToString,
            proto__python_dot_MocapExchange__pb2.StructureResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def GetMocapStream(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_stream(request, target, '/Mocap.MocapServer/GetMocapStream',
            proto__python_dot_MocapExchange__pb2.MocapStreamRequest.SerializeToString,
            proto__python_dot_MocapExchange__pb2.MocapStreamResponse.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)
