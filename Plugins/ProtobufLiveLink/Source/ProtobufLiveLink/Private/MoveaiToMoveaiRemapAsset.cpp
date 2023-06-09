// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveaiToMoveaiRemapAsset.h"

// switch(state)
// {
// case STARTUP:
//     //Do stuff
//     break;
// case EDIT:
//     //Do stuff
//     break;
// case ZONECREATION:
//     //Do stuff
//     break;
// case SHUTDOWN:
//     //Do stuff
//     break;
// case NOCHANGE:
//     //Do stuff
//     break;
// }
FVector UMoveaiToMoveaiRemapAsset::ConvertPosition(FVector LLPosition, ERetargetAxis X, ERetargetAxis Y, ERetargetAxis Z)
{
    switch (X)
    {
        case ERetargetAxis::X_pos:
            // UE_LOG(LogTemp, Warning, TEXT("\t X: X"));
            NewRetargetLocation.X = LLPosition.X * root_scale;
            break;
        case ERetargetAxis::Y_pos:
            // UE_LOG(LogTemp, Warning, TEXT("\t X: Y"));
            NewRetargetLocation.X = LLPosition.Y * root_scale;
            break;
        case ERetargetAxis::Z_pos:
            // UE_LOG(LogTemp, Warning, TEXT("\t X: Z"));
            NewRetargetLocation.X = LLPosition.Z * root_scale;
            break;
        case ERetargetAxis::X_neg:
            // UE_LOG(LogTemp, Warning, TEXT("\t X: -X"));
            NewRetargetLocation.X = -LLPosition.X * root_scale;
            break;
        case ERetargetAxis::Y_neg:
            // UE_LOG(LogTemp, Warning, TEXT("\t X: -Y"));
            NewRetargetLocation.X = -LLPosition.Y * root_scale;
            break;
        case ERetargetAxis::Z_neg:
            // UE_LOG(LogTemp, Warning, TEXT("\t X: -Z"));
            NewRetargetLocation.X = -LLPosition.Z * root_scale;
            break;
    }

    switch (Y)
    {
        case ERetargetAxis::X_pos:
            NewRetargetLocation.Y = LLPosition.X * root_scale;
            break;
        case ERetargetAxis::Y_pos:
            NewRetargetLocation.Y = LLPosition.Y * root_scale;
            break;
        case ERetargetAxis::Z_pos:
            NewRetargetLocation.Y = LLPosition.Z * root_scale;
            break;
        case ERetargetAxis::X_neg:
            NewRetargetLocation.Y = -LLPosition.X * root_scale;
            break;
        case ERetargetAxis::Y_neg:
            NewRetargetLocation.Y = -LLPosition.Y * root_scale;
            break;
        case ERetargetAxis::Z_neg:
            NewRetargetLocation.Y = -LLPosition.Z * root_scale;
            break;
    }

    switch (Z)
    {
        case ERetargetAxis::X_pos:
            NewRetargetLocation.Z = LLPosition.X * root_scale;
            break;
        case ERetargetAxis::Y_pos:
            NewRetargetLocation.Z = LLPosition.Y * root_scale;
            break;
        case ERetargetAxis::Z_pos:
            NewRetargetLocation.Z = LLPosition.Z * root_scale;
            break;
        case ERetargetAxis::X_neg:
            NewRetargetLocation.Z = -LLPosition.X * root_scale;
            break;
        case ERetargetAxis::Y_neg:
            NewRetargetLocation.Z = -LLPosition.Y * root_scale;
            break;
        case ERetargetAxis::Z_neg:
            NewRetargetLocation.Z = -LLPosition.Z * root_scale;
            break;
    }

    // UE_LOG(LogTemp, Warning, TEXT("NewRetargetLocation = %f, %f, %f"),
    // NewRetargetLocation.X, NewRetargetLocation.Y, NewRetargetLocation.Z);
    
    return NewRetargetLocation;
}

FQuat UMoveaiToMoveaiRemapAsset::ConvertRotation(FQuat LLRotation, ERetargetAxis X, ERetargetAxis Y, ERetargetAxis Z)
{   
    switch (X)
    {
        case ERetargetAxis::X_pos:
            // UE_LOG(LogTemp, Warning, TEXT("\t X: X"));
            NewRetargetRotation.X = LLRotation.X;
            break;
        case ERetargetAxis::Y_pos:
            // UE_LOG(LogTemp, Warning, TEXT("\t X: Y"));
            NewRetargetRotation.X = LLRotation.Y;
            break;
        case ERetargetAxis::Z_pos:
            // UE_LOG(LogTemp, Warning, TEXT("\t X: Z"));
            NewRetargetRotation.X = LLRotation.Z;
            break;
        case ERetargetAxis::X_neg:
            // UE_LOG(LogTemp, Warning, TEXT("\t X: -X"));
            NewRetargetRotation.X = -LLRotation.X;
            break;
        case ERetargetAxis::Y_neg:
            // UE_LOG(LogTemp, Warning, TEXT("\t X: -Y"));
            NewRetargetRotation.X = -LLRotation.Y;
            break;
        case ERetargetAxis::Z_neg:
            // UE_LOG(LogTemp, Warning, TEXT("\t X: -Z"));
            NewRetargetRotation.X = -LLRotation.Z;
            break;
    }

    switch (Y)
    {
        case ERetargetAxis::X_pos:
            NewRetargetRotation.Y = LLRotation.X;
            break;
        case ERetargetAxis::Y_pos:
            NewRetargetRotation.Y = LLRotation.Y;
            break;
        case ERetargetAxis::Z_pos:
            NewRetargetRotation.Y = LLRotation.Z;
            break;
        case ERetargetAxis::X_neg:
            NewRetargetRotation.Y = -LLRotation.X;
            break;
        case ERetargetAxis::Y_neg:
            NewRetargetRotation.Y = -LLRotation.Y;
            break;
        case ERetargetAxis::Z_neg:
            NewRetargetRotation.Y = -LLRotation.Z;
            break;
    }

    switch (Z)
    {
        case ERetargetAxis::X_pos:
            NewRetargetRotation.Z = LLRotation.X;
            break;
        case ERetargetAxis::Y_pos:
            NewRetargetRotation.Z = LLRotation.Y;
            break;
        case ERetargetAxis::Z_pos:
            NewRetargetRotation.Z = LLRotation.Z;
            break;
        case ERetargetAxis::X_neg:
            NewRetargetRotation.Z = -LLRotation.X;
            break;
        case ERetargetAxis::Y_neg:
            NewRetargetRotation.Z = -LLRotation.Y;
            break;
        case ERetargetAxis::Z_neg:
            NewRetargetRotation.Z = -LLRotation.Z;
            break;
    }

    NewRetargetRotation.W = -LLRotation.W;

    // UE_LOG(LogTemp, Warning, TEXT("bone %d . quater = %d,%d,%d,%d. loc = %d, %d, %d. scale = %d"),
    // NewRetargetLocation.);
    // UE_LOG(LogTemp, Warning, TEXT("\t Bone name = %s"), NewRetargetLocation.ToString());
    return NewRetargetRotation;
}

// FVector UMoveaiToMoveaiRemapAsset::ConvertRootPosition(FVector LLPosition) const
// {
//     return FVector(
//         LLPosition.X,
//         -LLPosition.Y,
//         LLPosition.Z
//     );

// }

FQuat UMoveaiToMoveaiRemapAsset::ConvertRootRotation(FQuat LLRotation) const
{
    return FQuat(
        LLRotation.Z,
        -LLRotation.Y,
        LLRotation.X,
        -LLRotation.W
    );
}

// FQuat UMoveaiToMoveaiRemapAsset::ConvertBoneRotation(FQuat LLRotation) const
// {
//     return FQuat(
//         LLRotation.X,
//         -LLRotation.Y,
//         LLRotation.Z,
//         -LLRotation.W
//     );
// }

// FQuat UMoveaiToMoveaiRemapAsset::ConvertRightArmRotation(FQuat LLRotation) const
// {
//     return FQuat(
//         LLRotation.Z,
//         -LLRotation.Y,
//         -LLRotation.X,
//         -LLRotation.W
//     );
// }

// FQuat UMoveaiToMoveaiRemapAsset::ConvertRightForeArmRotation(FQuat LLRotation) const
// {
//     return FQuat(
//         -LLRotation.Z,
//         LLRotation.Y,
//         LLRotation.X,
//         -LLRotation.W
//     );
// }

// FQuat UMoveaiToMoveaiRemapAsset::ConvertLeftArmRotation(FQuat LLRotation) const
// {
//     return FQuat(
//         -LLRotation.Z,
//         -LLRotation.Y,
//         LLRotation.X,
//         -LLRotation.W
//     );
// }

// FQuat UMoveaiToMoveaiRemapAsset::ConvertLeftForeArmRotation(FQuat LLRotation) const
// {
//     return FQuat(
//         LLRotation.Z,
//         LLRotation.Y,
//         -LLRotation.X,
//         -LLRotation.W
//     );
// }

// FQuat UMoveaiToMoveaiRemapAsset::ConvertMidSpineRotation(FQuat LLRotation) const
// {
//     return FQuat(
//         LLRotation.X,
//         LLRotation.Y,
//         LLRotation.Z,
//         -LLRotation.W
//     );
// }

// FQuat UMoveaiToMoveaiRemapAsset::ConvertNeckRotation(FQuat LLRotation) const
// {
//     return FQuat(
//         LLRotation.X, // 3rd
//         -LLRotation.Y, 
//         LLRotation.Z,
//         -LLRotation.W
//     );
// }

// FQuat UMoveaiToMoveaiRemapAsset::ConvertRightToeRotation(FQuat LLRotation) const
// {
//     return FQuat(
//         -LLRotation.X,
//         -LLRotation.Y,
//         LLRotation.Z,
//         -LLRotation.W
//     );
// }

// FQuat UMoveaiToMoveaiRemapAsset::ConvertLeftToeRotation(FQuat LLRotation) const
// {
//     return FQuat(
//         -LLRotation.X,
//         -LLRotation.Y,
//         LLRotation.Z,
//         -LLRotation.W
//     );
// }

// FQuat UMoveaiToMoveaiRemapAsset::ConvertRightFootRotation(FQuat LLRotation) const
// {
//     return FQuat(
//         LLRotation.X,
//         -LLRotation.Y,
//         LLRotation.Z,
//         -LLRotation.W
//     );
// }

// FQuat UMoveaiToMoveaiRemapAsset::ConvertLeftFootRotation(FQuat LLRotation) const
// {
//     return FQuat(
//         LLRotation.X,
//         -LLRotation.Y,
//         -LLRotation.Z,
//         -LLRotation.W
//     );
// }

// FQuat UMoveaiToMoveaiRemapAsset::ConvertRightHandRotation(FQuat LLRotation) const
// {
//     return FQuat(
//         LLRotation.Z,
//         -LLRotation.Y,
//         -LLRotation.X,
//         -LLRotation.W
//     );
// }

// FQuat UMoveaiToMoveaiRemapAsset::ConvertLeftHandRotation(FQuat LLRotation) const
// {
//     return FQuat(
//         LLRotation.Z,
//         LLRotation.Y,
//         LLRotation.X,
//         -LLRotation.W
//     );
// }