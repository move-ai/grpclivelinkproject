// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveaiToMoveaiRemapAsset.h"

FVector UMoveaiToMoveaiRemapAsset::ConvertRootPosition(FVector LLPosition) const
{
    return FVector(
        LLPosition.X,
        -LLPosition.Y,
        LLPosition.Z
    );

}

FQuat UMoveaiToMoveaiRemapAsset::ConvertRootRotation(FQuat LLRotation) const
{
    return FQuat(
        LLRotation.Z,
        -LLRotation.Y,
        LLRotation.X,
        -LLRotation.W
    );
}

FQuat UMoveaiToMoveaiRemapAsset::ConvertBoneRotation(FQuat LLRotation) const
{
    return FQuat(
        LLRotation.X,
        -LLRotation.Y,
        LLRotation.Z,
        -LLRotation.W
    );
}

FQuat UMoveaiToMoveaiRemapAsset::ConvertRightArmRotation(FQuat LLRotation) const
{
    return FQuat(
        LLRotation.Z,
        -LLRotation.Y,
        -LLRotation.X,
        -LLRotation.W
    );
}

FQuat UMoveaiToMoveaiRemapAsset::ConvertRightForeArmRotation(FQuat LLRotation) const
{
    return FQuat(
        -LLRotation.Z,
        LLRotation.Y,
        LLRotation.X,
        -LLRotation.W
    );
}

FQuat UMoveaiToMoveaiRemapAsset::ConvertLeftArmRotation(FQuat LLRotation) const
{
    return FQuat(
        -LLRotation.Z,
        -LLRotation.Y,
        LLRotation.X,
        -LLRotation.W
    );
}

FQuat UMoveaiToMoveaiRemapAsset::ConvertLeftForeArmRotation(FQuat LLRotation) const
{
    return FQuat(
        LLRotation.Z,
        LLRotation.Y,
        -LLRotation.X,
        -LLRotation.W
    );
}

FQuat UMoveaiToMoveaiRemapAsset::ConvertMidSpineRotation(FQuat LLRotation) const
{
    return FQuat(
        LLRotation.X,
        LLRotation.Y,
        LLRotation.Z,
        -LLRotation.W
    );
}

FQuat UMoveaiToMoveaiRemapAsset::ConvertNeckRotation(FQuat LLRotation) const
{
    return FQuat(
        LLRotation.X,
        -LLRotation.Z,
        LLRotation.Y,
        -LLRotation.W
    );
}

FQuat UMoveaiToMoveaiRemapAsset::ConvertRightToeRotation(FQuat LLRotation) const
{
    return FQuat(
        -LLRotation.X,
        -LLRotation.Y,
        LLRotation.Z,
        -LLRotation.W
    );
}

FQuat UMoveaiToMoveaiRemapAsset::ConvertLeftToeRotation(FQuat LLRotation) const
{
    return FQuat(
        -LLRotation.X,
        -LLRotation.Y,
        LLRotation.Z,
        -LLRotation.W
    );
}

FQuat UMoveaiToMoveaiRemapAsset::ConvertRightFootRotation(FQuat LLRotation) const
{
    return FQuat(
        LLRotation.X,
        -LLRotation.Y,
        LLRotation.Z,
        -LLRotation.W
    );
}

FQuat UMoveaiToMoveaiRemapAsset::ConvertLeftFootRotation(FQuat LLRotation) const
{
    return FQuat(
        LLRotation.X,
        -LLRotation.Y,
        -LLRotation.Z,
        -LLRotation.W
    );
}