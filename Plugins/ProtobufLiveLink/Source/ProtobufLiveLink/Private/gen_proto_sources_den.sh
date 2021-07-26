/mnt/Shared/moveai/repos/grpclivelinkproject/Plugins/UE_gRPC/Source/ThirdParty/protobuf/bin/protoc \
 --grpc_out=/mnt/Shared/moveai/repos/grpclivelinkproject/Plugins/ProtobufLiveLink/Source/ProtobufLiveLink/Private \
 --proto_path=./ \
 --cpp_out=/mnt/Shared/moveai/repos/grpclivelinkproject/Plugins/ProtobufLiveLink/Source/ProtobufLiveLink/Private \
--plugin=protoc-gen-grpc=/mnt/Shared/moveai/repos/grpclivelinkproject/Plugins/UE_gRPC/Source/ThirdParty/grpc/bin/grpc_cpp_plugin \
 ./MocapExchange.proto