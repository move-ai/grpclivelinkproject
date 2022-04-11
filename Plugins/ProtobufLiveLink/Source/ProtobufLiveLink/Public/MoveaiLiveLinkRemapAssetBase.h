// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LiveLinkRemapAsset.h"
#include "MoveaiLiveLinkRemapAssetBase.generated.h"

/**
 * 
 */
UCLASS()
class PROTOBUFLIVELINK_API UMoveaiLiveLinkRemapAssetBase : public ULiveLinkRemapAsset
{
    GENERATED_BODY()

    virtual void BuildPoseFromAnimationData(float DeltaTime, const FLiveLinkSkeletonStaticData* InSkeletonData,
                                            const FLiveLinkAnimationFrameData* InFrameData,
                                            FCompactPose& OutPose) override;
protected:
    // Override these in child classes
    virtual FVector ConvertRootPosition(FVector LLPosition) const { return LLPosition; };
    virtual FQuat ConvertRootRotation(FQuat LLRotation) const { return LLRotation; };
    virtual FQuat ConvertBoneRotation(FQuat LLRotation) const { return LLRotation; };
    virtual FQuat ConvertRightArmRotation(FQuat LLRotation) const { return LLRotation; };
    virtual FQuat ConvertRightForeArmRotation(FQuat LLRotation) const { return LLRotation; };
    virtual FQuat ConvertLeftArmRotation(FQuat LLRotation) const { return LLRotation; };
    virtual FQuat ConvertLeftForeArmRotation(FQuat LLRotation) const { return LLRotation; };
    virtual FQuat ConvertMidSpineRotation(FQuat LLRotation) const { return LLRotation; };
    virtual FQuat ConvertNeckRotation(FQuat LLRotation) const { return LLRotation; };
    virtual FQuat ConvertRightToeRotation(FQuat LLRotation) const { return LLRotation; };
    virtual FQuat ConvertLeftToeRotation(FQuat LLRotation) const { return LLRotation; };
    virtual FQuat ConvertRightFootRotation(FQuat LLRotation) const { return LLRotation; };
    virtual FQuat ConvertLeftFootRotation(FQuat LLRotation) const { return LLRotation; };
    virtual FQuat ConvertRightHandRotation(FQuat LLRotation) const { return LLRotation; };
    virtual FQuat ConvertLeftHandRotation(FQuat LLRotation) const { return LLRotation; };

    // This is the bone we will apply position translation to.
    // Radical 3.1 Live Link position data is mapped to the hips.
    // For other skeletons, the hip bone's name can differ and you can override this method.
    virtual FName GetTargetRootName() const { return "Hips"; }
    virtual FName GetTargetRightArmName() const { return "RightArm"; }
    virtual FName GetTargetRightForeArmName() const { return "RightForeArm"; }
    virtual FName GetTargetLeftArmName() const { return "LeftArm"; }
    virtual FName GetTargetLeftForeArmName() const { return "LeftForeArm"; }
    virtual FName GetMidSpineName() const { return "Spine2"; }
    virtual FName GetNeckName() const { return "Neck"; }
    virtual FName GetRightToeName() const { return "RightToeBase"; }
    virtual FName GetLeftToeName() const { return "LeftToeBase"; }
    virtual FName GetRightFootName() const { return "RightFoot"; }
    virtual FName GetLeftFootName() const { return "LeftFoot"; }
    virtual FName GetRightHandName() const { return "RightHand"; }
    virtual FName GetLeftHandName() const { return "LeftHand"; }
    

    // Cached lookup results from GetRemappedBoneName
    TMap<FName, FName> BoneNameMap;
};
