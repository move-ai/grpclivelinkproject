// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoveaiLiveLinkRemapAssetBase.h"

#include "MoveaiToMoveaiRemapAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROTOBUFLIVELINK_API UMoveaiToMoveaiRemapAsset : public UMoveaiLiveLinkRemapAssetBase
{
    GENERATED_BODY()

protected:
    // UFUNCTION(BlueprintCallable)
    
    virtual FVector ConvertPosition(FVector LLPosition, ERetargetAxis X, ERetargetAxis Y, ERetargetAxis Z) override;
    virtual FQuat ConvertRotation(FQuat LLRotation, ERetargetAxis X, ERetargetAxis Y, ERetargetAxis Z) override;
    // virtual FQuat ConvertBoneRotation(FQuat LLRotation) const override;

    // virtual FVector ConvertRootPosition(FVector LLPosition) const override;
    virtual FQuat ConvertRootRotation(FQuat LLRotation) const override;
    // virtual FQuat ConvertBoneRotation(FQuat LLRotation) const override;
    // virtual FQuat ConvertRightArmRotation(FQuat LLRotation) const override;
    // virtual FQuat ConvertRightForeArmRotation(FQuat LLRotation) const override;
    // virtual FQuat ConvertLeftArmRotation(FQuat LLRotation) const override;
    // virtual FQuat ConvertLeftForeArmRotation(FQuat LLRotation) const override;
    // virtual FQuat ConvertMidSpineRotation(FQuat LLRotation) const override;
    // virtual FQuat ConvertNeckRotation(FQuat LLRotation) const override;
    // virtual FQuat ConvertRightToeRotation(FQuat LLRotation) const override;
    // virtual FQuat ConvertLeftToeRotation(FQuat LLRotation) const override;
    // virtual FQuat ConvertRightFootRotation(FQuat LLRotation) const override;
    // virtual FQuat ConvertLeftFootRotation(FQuat LLRotation) const override;
    // virtual FQuat ConvertRightHandRotation(FQuat LLRotation) const override;
    // virtual FQuat ConvertLeftHandRotation(FQuat LLRotation) const override;

};
