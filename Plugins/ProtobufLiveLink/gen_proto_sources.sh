/media/F/sample_codes/UE4/grpclivelinkproject/Plugins/UE_gRPC/Source/ThirdParty/protobuf/bin/protoc \
 --grpc_out=/media/F/sample_codes/UE4/grpclivelinkproject/Plugins/ProtobufLiveLink/Source/ProtobufLiveLink/Private \
 --proto_path=/media/F/sample_codes/gRPC/mocap_stream/src/common/ \
 --cpp_out=/media/F/sample_codes/UE4/grpclivelinkproject/Plugins/ProtobufLiveLink/Source/ProtobufLiveLink/Private \
--plugin=protoc-gen-grpc=/media/F/sample_codes/UE4/grpclivelinkproject/Plugins/UE_gRPC/Source/ThirdParty/grpc/bin/grpc_cpp_plugin \
 /media/F/sample_codes/gRPC/mocap_stream/src/common/MocapExchange.proto