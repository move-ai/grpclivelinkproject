#pragma once
#include "HandlerInterface.h"
#include "MocapExchange.grpc.pb.h"
#include "ILiveLinkSource.h"

#include <grpcpp/grpcpp.h>

using namespace Mocap;

namespace Mocap {

class LiveLinkSubjectStream
{
public:
    LiveLinkSubjectStream();
    ~LiveLinkSubjectStream();
    void OnInitialized(const Mocap::Structure &subjectStructure);
    void OnNewPose(const Mocap::Pose &pose);
    void OnLost();
private:
};

}

