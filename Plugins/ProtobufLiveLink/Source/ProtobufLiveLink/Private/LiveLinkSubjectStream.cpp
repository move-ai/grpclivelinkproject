#if PLATFORM_WINDOWS
#pragma warning(push)
#pragma warning (disable : 4005)
#pragma warning (disable : 4125)
#pragma warning (disable : 4582)
#pragma warning (disable : 4583)
#pragma warning (disable : 4647)
#pragma warning (disable : 4668)
#pragma warning (disable : 4800)
#pragma warning (disable : 4946)

static void MemoryBarrier() {}
#pragma intrinsic(_InterlockedCompareExchange64)
#define InterlockedCompareExchangeAcquire64 _InterlockedCompareExchange64
#define InterlockedCompareExchangeRelease64 _InterlockedCompareExchange64
#define InterlockedCompareExchangeNoFence64 _InterlockedCompareExchange64
#define InterlockedCompareExchange64 _InterlockedCompareExchange64
#endif
#include "LiveLinkSubjectStream.h"

#include <grpc/support/log.h>

#include <iostream>

#include "LogUtility.h"

namespace Mocap
{
    
LiveLinkSubjectStream::LiveLinkSubjectStream(ILiveLinkClient* client, FGuid sourceGuid)
    : Client_m(client),
      SourceGuid_m(sourceGuid)
{
    UE_LOG(LogTemp, Warning, TEXT("LiveLinkSubjectStream constructor. Guid = %s"), *SourceGuid_m.ToString());
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
//    SubjectName_m = ModelStructure_m.name().c_str();
    uint32 structureId = ModelStructure_m.structureid();
    FString SubjectNamePrefix = "Subject_";
    FString GeneratedSubjectName = SubjectNamePrefix + FString::FromInt(structureId);

    SubjectName_m = FName(*GeneratedSubjectName);
    UE_LOG(LogTemp, Warning, TEXT("Subject %s created"), *SubjectName_m.ToString());

    FLiveLinkStaticDataStruct staticDataStruct = FLiveLinkStaticDataStruct(FLiveLinkSkeletonStaticData::StaticStruct());
    FLiveLinkSkeletonStaticData& staticData = *staticDataStruct.Cast<FLiveLinkSkeletonStaticData>();

    //To be initialized from ModelStructure_m;
    int nbones = ModelStructure_m.joints_size();
    staticData.BoneNames.SetNumUninitialized(nbones);
    staticData.BoneParents.SetNumUninitialized(nbones);

    UE_LOG(LogTemp, Warning, TEXT("Structure_name = %s. Number of bones: %d. SourceGuid_m  = %s"), *SubjectName_m.ToString(), nbones, *SourceGuid_m.ToString());

    for (int bidx=0; bidx < nbones; ++bidx){
        const Mocap::JointMeta& jointMeta = ModelStructure_m.joints(bidx);
        //To be initialized from ModelStructure_m
        FName boneName = jointMeta.name().c_str();
        staticData.BoneNames[bidx] = boneName;

        //To be initialized from ModelStructure_m;
        uint32 linkId = jointMeta.linkid();
        const Mocap::Link& link = ModelStructure_m.links(linkId);
        int boneParentIdx = (int) link.parentlinkid(); // Not sure how to get parents in correct way
        staticData.BoneParents[bidx] = boneParentIdx;
        UE_LOG(LogTemp, Warning, TEXT("Bone index: %d. bone_parent = , bone_name = "), bidx, boneParentIdx, *boneName.ToString());
    }

    Client_m->PushSubjectStaticData_AnyThread({SourceGuid_m, SubjectName_m}, ULiveLinkAnimationRole::StaticClass(), MoveTemp(staticDataStruct));

}
void LiveLinkSubjectStream::OnNewPose(const Mocap::Pose &pose)
{
    NewPose = pose;

    uint64 timestamp = NewPose.timestamp();
    FString TimestampString = FString::Printf(TEXT("%llu"), timestamp);
    UE_LOG(LogTemp, Warning, TEXT("Timestamp, OnNewPose very beginning: %s"), *TimestampString);

    LogCurrentTimeWithMilliseconds("Machine Clock OnNewPose very beginning:");

    FLiveLinkFrameDataStruct frmDataStructure = FLiveLinkFrameDataStruct(FLiveLinkAnimationFrameData::StaticStruct());
    
    FLiveLinkAnimationFrameData& frmData = *frmDataStructure.Cast<FLiveLinkAnimationFrameData>();

    //To be initialized from Pose or structure

    
    uint32 subjectId = NewPose.subjectid();

    int nbones = NewPose.joints_size();
    frmData.Transforms.SetNumUninitialized(nbones);

    // UE_LOG(LogTemp, Warning, TEXT("subjectId: %d. number_bones = %d"), subjectId, nbones);

    for (int i = 0; i < nbones; ++i) {
        const Mocap::Joint& joint = NewPose.joints(i);

        // Location
        FVector boneLoc;
        boneLoc.X = (float) joint.transform().translation().x();
        boneLoc.Y = (float) joint.transform().translation().y();
        boneLoc.Z = (float) joint.transform().translation().z();

        FVector boneRotationEuler;
        FQuat boneQuat;

        // Rotation
        if (joint.transform().has_orientation()) {
            switch (joint.transform().orientation().rotationtype()) {
                case Mocap::Orientation::QUATERNION:
                    boneQuat.X = (float) joint.transform().orientation().rotationvalues(0);
                    boneQuat.Y = (float) joint.transform().orientation().rotationvalues(1);
                    boneQuat.Z = (float) joint.transform().orientation().rotationvalues(2);
                    boneQuat.W = (float) joint.transform().orientation().rotationvalues(3);
                    break;
                case Mocap::Orientation::MATRIX:
                    break; // TODO
                case Mocap::Orientation::EULER_XYZ:
                case Mocap::Orientation::EULER_XZY:
                case Mocap::Orientation::EULER_YXZ:
                case Mocap::Orientation::EULER_YZX:
                case Mocap::Orientation::EULER_ZXY:
                case Mocap::Orientation::EULER_ZYX:
                case Mocap::Orientation::EULER_XY:
                case Mocap::Orientation::EULER_YX:
                case Mocap::Orientation::EULER_XZ:
                case Mocap::Orientation::EULER_ZX:
                case Mocap::Orientation::EULER_YZ:
                case Mocap::Orientation::EULER_ZY:
                case Mocap::Orientation::EULER_X:
                case Mocap::Orientation::EULER_Y:
                case Mocap::Orientation::EULER_Z:
                    boneRotationEuler.X = FMath::RadiansToDegrees((float) joint.transform().orientation().rotationvalues(0));
                    boneRotationEuler.Y = FMath::RadiansToDegrees((float) joint.transform().orientation().rotationvalues(1));
                    boneRotationEuler.Z = FMath::RadiansToDegrees((float) joint.transform().orientation().rotationvalues(2));
                    // UE_LOG(LogTemp, Warning, TEXT("X Euler before conversion to quat: %f"), boneRotationEuler.X);

                    boneQuat = FQuat::MakeFromEuler(boneRotationEuler);
                    break;
                case Mocap::Orientation::NONE:
                    boneQuat.X = 0.0f;
                    boneQuat.Y = 0.0f;
                    boneQuat.Z = 0.0f;
                    boneQuat.W = 1.0f;
                    break;
            }
        }
        else {
            boneQuat.X = 0.0f;
            boneQuat.Y = 0.0f;
            boneQuat.Z = 0.0f;
            boneQuat.W = 1.0f;            
        }

        // UE_LOG(LogTemp, Warning, TEXT("Joint number: %d"), i)
        // UE_LOG(LogTemp, Warning, TEXT("boneLoc.X: %f"), boneLoc.X);
        // UE_LOG(LogTemp, Warning, TEXT("boneLoc.Y: %f"), boneLoc.Y);
        // UE_LOG(LogTemp, Warning, TEXT("boneLoc.Z: %f"), boneLoc.Z);

        // UE_LOG(LogTemp, Warning, TEXT("rotationvalues(0): %f"), joint.transform().orientation().rotationvalues(0));

        // UE_LOG(LogTemp, Warning, TEXT("FQuat.X: %f"), boneQuat.X);
        // UE_LOG(LogTemp, Warning, TEXT("FQuat.Y: %f"), boneQuat.Y);
        // UE_LOG(LogTemp, Warning, TEXT("FQuat.Z: %f"), boneQuat.Z);
        // UE_LOG(LogTemp, Warning, TEXT("FQuat.W: %f"), boneQuat.W);

        FVector boneScale;
        if (i == 0) {
            boneScale = FVector(100.0f, 100.0f, 100.0f);
        }
        else {
            boneScale = FVector(1.0f, 1.0f, 1.0f);
        }
        // UE_LOG(LogTemp, Warning, TEXT("bone %d . quater = %d,%d,%d,%d. loc = %d, %d, %d. scale = %d"),
        // boneQuat.X, boneQuat.Y,boneQuat.Z, boneQuat.W, boneLoc.X, boneLoc.Y, boneLoc.Z, boneScale.X);
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
    
    // UE_LOG(LogTemp, Warning, TEXT("Push frame data to %s, guid=%s"), *SubjectName_m.ToString(), *SourceGuid_m.ToString())
    
    LogCurrentTimeWithMilliseconds("Machine Clock OnNewPose before PushSubjectFrameData_AnyThread:");

    Client_m->PushSubjectFrameData_AnyThread({SourceGuid_m, SubjectName_m}, MoveTemp(frmDataStructure));

    LogCurrentTimeWithMilliseconds("Machine Clock OnNewPose after PushSubjectFrameData_AnyThread:");
}

void LiveLinkSubjectStream::OnLost()
{
    //TODO

}
} // namespace Mocap

#if PLATFORM_WINDOWS
#pragma warning( pop )
#endif