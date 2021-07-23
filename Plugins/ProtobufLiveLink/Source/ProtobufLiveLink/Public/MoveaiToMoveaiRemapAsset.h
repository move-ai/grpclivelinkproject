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
    virtual FVector ConvertRootPosition(FVector LLPosition) const override;

    virtual FQuat ConvertRootRotation(FQuat LLRotation) const override;

    virtual FQuat ConvertBoneRotation(FQuat LLRotation) const override;
};
