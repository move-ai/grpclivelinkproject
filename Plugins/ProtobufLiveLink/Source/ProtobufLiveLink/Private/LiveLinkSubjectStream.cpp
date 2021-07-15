#include "LiveLinkSubjectStream.h"

#include <grpc/support/log.h>

#include <iostream>


namespace Mocap
{
    
LiveLinkSubjectStream::LiveLinkSubjectStream(ILiveLinkClient* client, FGuid sourceGuid)
    : Client_m(client),
      SourceGuid_m(sourceGuid)
{
}

LiveLinkSubjectStream::~LiveLinkSubjectStream()
{}

void LiveLinkSubjectStream::OnInitialized(const Mocap::Structure &subjectStructure)
{
    //save model structure for later use. 
    // TODO: might be it's worth to create more convient data for fast access later.
    ModelStructure_m = subjectStructure;

    // To be initialized;
    FName subjectName;
    SubjectName_m = ModelStructure_m.name().c_str();
    // UE_LOG(LogTemp, Warning, TEXT("Subject %s created"), *FString(ModelStructure_m.name().c_str()))

    FLiveLinkStaticDataStruct staticDataStruct = FLiveLinkStaticDataStruct(FLiveLinkSkeletonStaticData::StaticStruct());
    FLiveLinkSkeletonStaticData& staticData = *staticDataStruct.Cast<FLiveLinkSkeletonStaticData>();

    //To be initialized from ModelStructure_m;
    int nbones = 10;
    staticData.BoneNames.SetNumUninitialized(nbones);
    staticData.BoneParents.SetNumUninitialized(nbones);


    for (int bidx=0; bidx < nbones; ++bidx){
        //To be initialized from ModelStructure_m
        FName bname;
        staticData.BoneNames[bidx] = bname;

        //To be initialized from ModelStructure_m;
        int32 boneParentIdx = -1;
        staticData.BoneParents[bidx] = boneParentIdx;
    }

    Client_m->PushSubjectStaticData_AnyThread({SourceGuid_m, SubjectName_m}, ULiveLinkAnimationRole::StaticClass(), MoveTemp(staticDataStruct));

}
void LiveLinkSubjectStream::OnNewPose(const Mocap::Pose &pose)
{
    FLiveLinkFrameDataStruct frmDataStructure = FLiveLinkFrameDataStruct(FLiveLinkAnimationFrameData::StaticStruct());
    
    FLiveLinkAnimationFrameData& frmData = *frmDataStructure.Cast<FLiveLinkAnimationFrameData>();

    //To be initialized from Pose or structure
    int nbones = 10;
    frmData.Transforms.SetNumUninitialized(nbones);


    // structure information can be accessed from ModelStructure_m

    for (int bidx = 0; bidx < nbones; ++bidx)
    {
        //To be initialized
        FVector bscale;
        FVector bloc;
        FQuat bquat;
        frmData.Transforms[bidx] = FTransform(bquat, bloc, bscale);
    }
    
    // UE_LOG(LogTemp, Warning, TEXT("Push frame data to %s"), *FString(ModelStructure_m.name().c_str()))
    
    Client_m->PushSubjectFrameData_AnyThread({SourceGuid_m, SubjectName_m}, MoveTemp(frmDataStructure));
}

void LiveLinkSubjectStream::OnLost()
{
    //TODO

}
} // namespace Mocap

