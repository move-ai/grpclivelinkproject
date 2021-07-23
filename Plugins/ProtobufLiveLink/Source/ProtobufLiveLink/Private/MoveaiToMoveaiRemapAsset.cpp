// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveaiToMoveaiRemapAsset.h"

FVector UMoveaiToMoveaiRemapAsset::ConvertRootPosition(FVector LLPosition) const
{
    // Unreal uses cm, so apply x100 conversion factor
    return FVector(
        LLPosition.X,
        LLPosition.Y,
        LLPosition.Z
    );

}

FQuat UMoveaiToMoveaiRemapAsset::ConvertRootRotation(FQuat LLRotation) const
{
    return FQuat(
        LLRotation.X,
        LLRotation.Y,
        LLRotation.Z,
        LLRotation.W
    );
}

FQuat UMoveaiToMoveaiRemapAsset::ConvertBoneRotation(FQuat LLRotation) const
{
    return FQuat(
        LLRotation.X,
        LLRotation.Y,
        LLRotation.Z,
        LLRotation.W
    );
}
