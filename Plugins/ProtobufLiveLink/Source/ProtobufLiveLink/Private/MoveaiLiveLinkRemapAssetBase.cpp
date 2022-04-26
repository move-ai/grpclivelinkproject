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
        // UE_LOG(LogTemp, Warning, TEXT("\t SrcBoneName = %s"), *SrcBoneName.ToString());
        FName* TargetBoneName = BoneNameMap.Find(SrcBoneName);

        // for (auto& Elem : BoneNameMap)
        // {   
        //     // UE_LOG(LogTemp, Warning, TEXT("(%s, \"%s\")\n"), *Elem.Key.ToString(), *Elem.Value.ToString());
        // }
        // UE_LOG(LogTemp, Warning, TEXT("\t TargetBoneName = %s"), BoneNameMap.Find(SrcBoneName));
        if (TargetBoneName == nullptr)
        {
            // UE_LOG(LogTemp, Warning, TEXT("\t SrcBoneName nullptr = %s"), *SrcBoneName.ToString());
            /* User will create a blueprint child class and implement this function using a switch statement. */
            FName NewName = GetRemappedBoneName(SrcBoneName);
            TransformedBoneNames.Add(NewName);
            BoneNameMap.Add(SrcBoneName, NewName);
        }
        else
        {
            // UE_LOG(LogTemp, Warning, TEXT("\t SrcBoneName non-nullptr = %s"), *SrcBoneName.ToString());
            TransformedBoneNames.Add(*TargetBoneName);
        }
    }

    

    // Iterate over remapped bone names, find the index of that bone on the skeleton, and apply the Live Link pose data.
    for (int32 i = 0; i < TransformedBoneNames.Num(); i++)
    {
        FName BoneName = TransformedBoneNames[i];
        // UE_LOG(LogTemp, Warning, TEXT("The integer value is: %d"), i);
        // UE_LOG(LogTemp, Warning, TEXT("\t SrcBoneName nullptr = %s"), *BoneName.ToString());
        FTransform BoneTransform = InFrameData->Transforms[i];
        const int32 MeshIndex = OutPose.GetBoneContainer().GetPoseBoneIndexForBoneName(BoneName);
        if (MeshIndex != INDEX_NONE)
        {
            FCompactPoseBoneIndex CPIndex = OutPose.GetBoneContainer().MakeCompactPoseIndex(
                FMeshPoseBoneIndex(MeshIndex));
            if (CPIndex != INDEX_NONE)
            {
                // UE_LOG(LogTemp, Warning, TEXT("\t Bone name = %s"), *BoneName.ToString());
                FQuat ConvertedLiveLinkRotation;
                // Retrieves the default reference pose for the skeleton. Live Link data contains relative transforms from the default pose.
                auto RefBoneTransform = OutPose.GetRefPose(CPIndex);

                const FName* RetargetSourceBoneName = BoneNameMap.FindKey(BoneName);

                // Only use position + rotation data for root. For all other bones, set rotation only.
                if (*RetargetSourceBoneName == GetTargetRootName())
                {
                    // OutPose[CPIndex].SetLocation(ConvertRootPosition(BoneTransform.GetTranslation()));
                    // FVector TempVector = BoneTransform.GetTranslation()                    
                    OutPose[CPIndex].SetLocation(ConvertPosition(BoneTransform.GetTranslation(), hips_loc_x, hips_loc_y, hips_loc_z));
                    // ConvertedLiveLinkRotation = ConvertRootRotation(BoneTransform.GetRotation());
                    ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), hips_rot_x, hips_rot_y, hips_rot_z);
                }
                else
                {   
                    // Spine
                    if (*RetargetSourceBoneName == GetMidSpineName())
                    {
                        ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), midSpine_rot_x, midSpine_rot_y, midSpine_rot_z);
                    }
                    else if (*RetargetSourceBoneName == GetNeckName())
                    {
                        ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), neck_rot_x, neck_rot_y, neck_rot_z);
                    }
                    
                    // Right arm
                    else if (*RetargetSourceBoneName == GetTargetRightArmName())
                    {
                        ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), rightArm_rot_x, rightArm_rot_y, rightArm_rot_z);
                    }
                    else if (*RetargetSourceBoneName == GetTargetRightForeArmName())
                    {
                        ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), rightForearm_rot_x, rightForearm_rot_y, rightForearm_rot_z);
                    }
                    else if (*RetargetSourceBoneName == GetRightHandName())
                    {
                        ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), rightHand_rot_x, rightHand_rot_y, rightHand_rot_z);
                    }
                    
                    // Left arm
                    else if (*RetargetSourceBoneName == GetTargetLeftArmName())
                    {
                        ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), leftArm_rot_x, leftArm_rot_y, leftArm_rot_z);
                    }
                    else if (*RetargetSourceBoneName == GetTargetLeftForeArmName())
                    {
                        ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), leftForearm_rot_x, leftForearm_rot_y, leftForearm_rot_z);
                    }
                    else if (*RetargetSourceBoneName == GetLeftHandName())
                    {
                        ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), leftHand_rot_x, leftHand_rot_y, leftHand_rot_z);
                    }

                    // Right leg
                    else if (*RetargetSourceBoneName == GetRightUpLeg())
                    {
                        ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), rightUpLeg_rot_x, rightUpLeg_rot_y, rightUpLeg_rot_z);
                    }
                    else if (*RetargetSourceBoneName == GetRightLeg())
                    {
                        ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), rightLeg_rot_x, rightLeg_rot_y, rightLeg_rot_z);
                    }
                    else if (*RetargetSourceBoneName == GetRightFootName())
                    {
                        ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), rightFoot_rot_x, rightFoot_rot_y, rightFoot_rot_z);
                    }
                    else if (*RetargetSourceBoneName == GetRightToeName())
                    {
                        ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), rightToe_rot_x, rightToe_rot_y, rightToe_rot_z);
                    }

                    // Left leg
                    else if (*RetargetSourceBoneName == GetLeftUpLeg())
                    {
                        ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), leftUpLeg_rot_x, leftUpLeg_rot_y, leftUpLeg_rot_z);
                    }
                    else if (*RetargetSourceBoneName == GetLeftLeg())
                    {
                        ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), leftLeg_rot_x, leftLeg_rot_y, leftLeg_rot_z);
                    }
                    else if (*RetargetSourceBoneName == GetLeftFootName())
                    {
                        ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), leftFoot_rot_x, leftFoot_rot_y, leftFoot_rot_z);
                    }
                    else if (*RetargetSourceBoneName == GetLeftToeName())
                    {
                        ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), leftToe_rot_x, leftToe_rot_y, leftToe_rot_z);
                    }
                    
                    
                    // else
                    // {
                    //     ConvertedLiveLinkRotation = ConvertBoneRotation(BoneTransform.GetRotation());
                    // }
                }
                

                OutPose[CPIndex].SetRotation(RefBoneTransform.GetRotation() * ConvertedLiveLinkRotation);
            }
        }
    }
}

