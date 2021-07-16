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
    // FName subjectName;
    SubjectName_m = ModelStructure_m.name().c_str();
    UE_LOG(LogTemp, Warning, TEXT("Subject %s created"), *SubjectName_m.ToString())

    FLiveLinkStaticDataStruct staticDataStruct = FLiveLinkStaticDataStruct(FLiveLinkSkeletonStaticData::StaticStruct());
    FLiveLinkSkeletonStaticData& staticData = *staticDataStruct.Cast<FLiveLinkSkeletonStaticData>();

    //To be initialized from ModelStructure_m;
    int nbones = ModelStructure_m.joints_size();
    staticData.BoneNames.SetNumUninitialized(nbones);
    staticData.BoneParents.SetNumUninitialized(nbones);


    for (int bidx=0; bidx < nbones; ++bidx){
        const Mocap::JointMeta& jointMeta = ModelStructure_m.joints(bidx);
        //To be initialized from ModelStructure_m
        FName boneName = jointMeta.name().c_str();
        staticData.BoneNames[bidx] = boneName;

        //To be initialized from ModelStructure_m;
        int32 linkId = jointMeta.linkid();
        const Mocap::Link& link = ModelStructure_m.links(linkId);
        int32 boneParentIdx = (int) link.parentlinkid();
        staticData.BoneParents[bidx] = boneParentIdx;
    }

    Client_m->PushSubjectStaticData_AnyThread({SourceGuid_m, SubjectName_m}, ULiveLinkAnimationRole::StaticClass(), MoveTemp(staticDataStruct));

}
void LiveLinkSubjectStream::OnNewPose(const Mocap::Pose &pose)
{
    FLiveLinkFrameDataStruct frmDataStructure = FLiveLinkFrameDataStruct(FLiveLinkAnimationFrameData::StaticStruct());
    
    FLiveLinkAnimationFrameData& frmData = *frmDataStructure.Cast<FLiveLinkAnimationFrameData>();

    //To be initialized from Pose or structure

    NewPose = pose;
    int subjectId = NewPose.subjectid();
    UE_LOG(LogTemp, Warning, TEXT("subjectId: %d"), subjectId);

    int nbones = NewPose.joints_size();
    frmData.Transforms.SetNumUninitialized(nbones);
    UE_LOG(LogTemp, Warning, TEXT("Number of bones: %d"), nbones);

    for (int i = 0; i < nbones; ++i) {
        const Mocap::Joint& joint = NewPose.joints(i);
        UE_LOG(LogTemp, Warning, TEXT("Joint number: %d"), i)
        UE_LOG(LogTemp, Warning, TEXT("translation().x(): %f"), joint.transform().translation().x());
        UE_LOG(LogTemp, Warning, TEXT("translation().y(): %f"), joint.transform().translation().y());
        UE_LOG(LogTemp, Warning, TEXT("translation().z(): %f"), joint.transform().translation().z());
        FVector boneLoc;
        boneLoc.X = (float) joint.transform().translation().x();
        boneLoc.Y = (float) joint.transform().translation().y();
        boneLoc.Z = (float) joint.transform().translation().z();
        FQuat boneQuat = FQuat(0.0f, 0.0f, 0.0f, 1.0f);
        // FVector boneLoc = FVector(0.0f, 0.0f, 0.0f);
        FVector boneScale = FVector(1.0f, 1.0f, 1.0f);
        frmData.Transforms[i] = FTransform(boneQuat, boneLoc, boneScale);
    }

    // structure information can be accessed from ModelStructure_m

    // for (int bidx = 0; bidx < nbones; ++bidx)
    // {
    //     //To be initialized
    //     FVector bscale = FVector(1.0f, 1.0f, 1.0f);
    //     FVector bloc = FVector(0.0f, 0.0f, 0.0f);
    //     FQuat bquat = FQuat(0.0f, 0.0f, 0.0f, 1.0f);
    //     frmData.Transforms[bidx] = FTransform(bquat, bloc, bscale);
    // }
    
    UE_LOG(LogTemp, Warning, TEXT("Push frame data to %s"), *SubjectName_m.ToString())
    
    
    Client_m->PushSubjectFrameData_AnyThread({SourceGuid_m, SubjectName_m}, MoveTemp(frmDataStructure));
}

void LiveLinkSubjectStream::OnLost()
{
    //TODO

}
} // namespace Mocap

