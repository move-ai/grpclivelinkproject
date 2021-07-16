#pragma once
#include "HandlerInterface.h"
#include "MocapExchange.grpc.pb.h"
#include "ILiveLinkSource.h"
#include "ILiveLinkClient.h"
#include "LiveLinkTypes.h"
#include "Roles/LiveLinkAnimationRole.h"
#include "Roles/LiveLinkAnimationTypes.h"

#include <grpcpp/grpcpp.h>

using namespace Mocap;

namespace Mocap {

class LiveLinkSubjectStream
{
public:
    LiveLinkSubjectStream(ILiveLinkClient* client, FGuid sourceGuid);
    ~LiveLinkSubjectStream();
    void OnInitialized(const Mocap::Structure &subjectStructure);
    void OnNewPose(const Mocap::Pose &pose);
    void OnLost();
private:
    ILiveLinkClient* Client_m;
    FGuid SourceGuid_m;
    FName SubjectName_m;

    Mocap::Structure ModelStructure_m;
    Mocap::Pose NewPose;
};

}

