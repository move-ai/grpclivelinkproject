#include "MyObject.h"
#include "greeter_client.cc"

FString UMyObject::SayHello()
{
    GreeterClient* c = new GreeterClient(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
    std::string user("world");
    std::string reply = c->SayHello(user);
    delete c;
    return reply.c_str();
}