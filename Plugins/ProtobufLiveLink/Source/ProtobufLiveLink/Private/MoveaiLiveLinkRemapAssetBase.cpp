// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveaiLiveLinkRemapAssetBase.h"


#include "Roles/LiveLinkAnimationTypes.h"
#include "BonePose.h"


// Take Live Link data and apply it to skeleton bones in UE4.
void UMoveaiLiveLinkRemapAssetBase::BuildPoseFromAnimationData(float DeltaTime,
                                                                const FLiveLinkSkeletonStaticData* InSkeletonData,
                                                                const FLiveLinkAnimationFrameData* InFrameData,
                                                                FCompactPose& OutPose)
{
    const TArray<FName>& SourceBoneNames = InSkeletonData->BoneNames;

    TArray<FName, TMemStackAllocator<>> TransformedBoneNames;
    TransformedBoneNames.Reserve(SourceBoneNames.Num());

    // Find remapped bone names and cache them for fast subsequent retrieval.
    for (const FName& SrcBoneName : SourceBoneNames)
    {
        FName* TargetBoneName = BoneNameMap.Find(SrcBoneName);
        if (TargetBoneName == nullptr)
        {
            /* User will create a blueprint child class and implement this function using a switch statement. */
            FName NewName = GetRemappedBoneName(SrcBoneName);
            TransformedBoneNames.Add(NewName);
            BoneNameMap.Add(SrcBoneName, NewName);
        }
        else
        {
            TransformedBoneNames.Add(*TargetBoneName);
        }
    }

    // Iterate over remapped bone names, find the index of that bone on the skeleton, and apply the Live Link pose data.
    for (int32 i = 0; i < TransformedBoneNames.Num(); i++)
    {
        FName BoneName = TransformedBoneNames[i];
        FTransform BoneTransform = InFrameData->Transforms[i];
        const int32 MeshIndex = OutPose.GetBoneContainer().GetPoseBoneIndexForBoneName(BoneName);
        if (MeshIndex != INDEX_NONE)
        {
            FCompactPoseBoneIndex CPIndex = OutPose.GetBoneContainer().MakeCompactPoseIndex(
                FMeshPoseBoneIndex(MeshIndex));
            if (CPIndex != INDEX_NONE)
            {
                FQuat ConvertedLiveLinkRotation;
                // Retrieves the default reference pose for the skeleton. Live Link data contains relative transforms from the default pose.
                auto RefBoneTransform = OutPose.GetRefPose(CPIndex);

                // Only use position + rotation data for root. For all other bones, set rotation only.
                if (BoneName == GetTargetRootName())
                {
                    OutPose[CPIndex].SetLocation(ConvertRootPosition(BoneTransform.GetTranslation()));
                    ConvertedLiveLinkRotation = ConvertRootRotation(BoneTransform.GetRotation());
                }
                else
                {
                    if (BoneName == GetTargetRightForeArmName())
                    {
                        ConvertedLiveLinkRotation = ConvertRightForeArmRotation(BoneTransform.GetRotation());
                    }
                    else if (BoneName == GetTargetRightArmName())
                    {
                        ConvertedLiveLinkRotation = ConvertRightArmRotation(BoneTransform.GetRotation());
                    }
                    else if (BoneName == GetTargetLeftForeArmName())
                    {
                        ConvertedLiveLinkRotation = ConvertLeftForeArmRotation(BoneTransform.GetRotation());
                    }
                    else if (BoneName == GetTargetLeftArmName())
                    {
                        ConvertedLiveLinkRotation = ConvertLeftArmRotation(BoneTransform.GetRotation());
                    }
                    else if (BoneName == GetMidSpineName())
                    {
                        ConvertedLiveLinkRotation = ConvertMidSpineRotation(BoneTransform.GetRotation());
                    }
                    else if (BoneName == GetNeckName())
                    {
                        ConvertedLiveLinkRotation = ConvertNeckRotation(BoneTransform.GetRotation());
                    }
                    else if (BoneName == GetRightToeName())
                    {
                        ConvertedLiveLinkRotation = ConvertRightToeRotation(BoneTransform.GetRotation());
                    }
                    else if (BoneName == GetLeftToeName())
                    {
                        ConvertedLiveLinkRotation = ConvertLeftToeRotation(BoneTransform.GetRotation());
                    }
                    else if (BoneName == GetRightFootName())
                    {
                        ConvertedLiveLinkRotation = ConvertRightFootRotation(BoneTransform.GetRotation());
                    }
                    else if (BoneName == GetLeftFootName())
                    {
                        ConvertedLiveLinkRotation = ConvertLeftFootRotation(BoneTransform.GetRotation());
                    }
                    else if (BoneName == GetRightHandName())
                    {
                        ConvertedLiveLinkRotation = ConvertRightHandRotation(BoneTransform.GetRotation());
                    }
                    else if (BoneName == GetLeftHandName())
                    {
                        ConvertedLiveLinkRotation = ConvertLeftHandRotation(BoneTransform.GetRotation());
                    }
                    else
                    {
                        ConvertedLiveLinkRotation = ConvertBoneRotation(BoneTransform.GetRotation());
                    }
                }
                

                OutPose[CPIndex].SetRotation(RefBoneTransform.GetRotation() * ConvertedLiveLinkRotation);
            }
        }
    }
}

