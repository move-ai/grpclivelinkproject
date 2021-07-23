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

    // This is the bone we will apply position translation to.
    // Radical 3.1 Live Link position data is mapped to the hips.
    // For other skeletons, the hip bone's name can differ and you can override this method.
    virtual FName GetTargetRootName() const { return "Hips"; }

    // Cached lookup results from GetRemappedBoneName
    TMap<FName, FName> BoneNameMap;
};
