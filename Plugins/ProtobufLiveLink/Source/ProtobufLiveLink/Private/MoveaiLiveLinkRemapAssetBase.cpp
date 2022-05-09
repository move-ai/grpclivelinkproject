// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveaiLiveLinkRemapAssetBase.h"
#include "QuatStaticLibrary.h"

#include "Roles/LiveLinkAnimationTypes.h"
#include "BonePose.h"

// void get_ref_pose_bone_comp_space_transform(TArray<FTransform>& outResult, FCompactPose& OutPose)
// {
//     //  FReferenceSkeleton refSkeleton = inSkelComp->SkeletalMesh->RefSkeleton;

//     const int32 poseNum = OutPose.GetNumBones()

//     outResult.Reset(); 
//     outResult.AddUninitialized(poseNum); 

//     for (int32 i = 0; i < poseNum; i++)
//     {
//         outResult[i] = get_ref_pose_single_bone_comp_space_transform(OutPose, i); 

//     }

// }
float dot_qtqt(const float a[4], const float b[4])
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}

void mul_qt_qtqt(float q[4], const float a[4], const float b[4])
{
    float t0, t1, t2;

    t0 = a[0] * b[0] - a[1] * b[1] - a[2] * b[2] - a[3] * b[3];
    t1 = a[0] * b[1] + a[1] * b[0] + a[2] * b[3] - a[3] * b[2];
    t2 = a[0] * b[2] + a[2] * b[0] + a[3] * b[1] - a[1] * b[3];
    q[3] = a[0] * b[3] + a[3] * b[0] + a[1] * b[2] - a[2] * b[1];
    q[0] = t0;
    q[1] = t1;
    q[2] = t2;
}

void mul_qt_fl(float q[4], const float f)
{
    q[0] *= f;
    q[1] *= f;
    q[2] *= f;
    q[3] *= f;
}

void conjugate_qt_qt(float q1[4], const float q2[4])
{
    q1[0] = q2[0];
    q1[1] = -q2[1];
    q1[2] = -q2[2];
    q1[3] = -q2[3];
}

// float q[4], const float q1[4], const float q2[4]
FQuat rotation_between_quats_to_quat(FQuat u_q1, FQuat u_q2)
{
    
    const float q1[4] = {u_q1.W, u_q1.X, u_q1.Y, u_q1.Z};
    // q1[0] = u_q1.W;
    // q1[1] = u_q1.X;
    // q1[2] = u_q1.Y;
    // q1[3] = u_q1.Z;

    const float q2[4] = {u_q2.W, u_q2.X, u_q2.Y, u_q2.Z};
    // q2[0] = u_q2.W;
    // q2[1] = u_q2.X;
    // q2[2] = u_q2.Y;
    // q2[3] = u_q2.Z;

    float tquat[4];

    conjugate_qt_qt(tquat, q1);

    mul_qt_fl(tquat, 1.0f / dot_qtqt(tquat, tquat));

    float q[4];
    mul_qt_qtqt(q, tquat, q2);

    FQuat quatDif;
    quatDif.W = q[0];
    quatDif.X = q[1];
    quatDif.Y = q[2];
    quatDif.Z = q[3];

    return quatDif;
}

FTransform GetRefBoneCompSpaceTransform(FCompactPose& OutPose, FCompactPoseBoneIndex boneIdx)
{
    FTransform resultBoneTransform = OutPose.GetRefPose(boneIdx);

    FCompactPoseBoneIndex parentBoneIndex = OutPose.GetParentBoneIndex(boneIdx);    
    // auto refBoneInfo = OutPose.GetRefBoneInfo();

    while (boneIdx.GetInt() != 0)
    {
        // UE_LOG(LogTemp, Warning, TEXT("boneIdx is: %d"), boneIdx.GetInt());
        // UE_LOG(LogTemp, Warning, TEXT("parentBoneIndex is: %d"), parentBoneIndex.GetInt());
        parentBoneIndex = OutPose.GetParentBoneIndex(boneIdx);
        resultBoneTransform *= OutPose.GetRefPose(parentBoneIndex); 
        boneIdx = parentBoneIndex;
    }

    return resultBoneTransform; 
}


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
        FName NewName;
        if (TargetBoneName == nullptr)
        {
            // UE_LOG(LogTemp, Warning, TEXT("\t SrcBoneName nullptr = %s"), *SrcBoneName.ToString());
            /* User will create a blueprint child class and implement this function using a switch statement. */
            NewName = GetRemappedBoneName(SrcBoneName);
            TransformedBoneNames.Add(NewName);
            BoneNameMap.Add(SrcBoneName, NewName);

            const int32 MeshIndex = OutPose.GetBoneContainer().GetPoseBoneIndexForBoneName(NewName);
            if (MeshIndex != INDEX_NONE)
            {
                FCompactPoseBoneIndex CPIndex = OutPose.GetBoneContainer().MakeCompactPoseIndex(FMeshPoseBoneIndex(MeshIndex));
                if (CPIndex != INDEX_NONE)
                {
                    FTransform RefBoneCompSpaceTransform = GetRefBoneCompSpaceTransform(OutPose, CPIndex);
                    
                    FMatrix identityMatrix;
                    identityMatrix.SetIdentity();

                    FQuat quatOffset;
                    quatOffset = rotation_between_quats_to_quat(RefBoneCompSpaceTransform.GetRotation(), identityMatrix.ToQuat());

                    RetargetOffsets.Add(NewName, quatOffset);
                }
            }

            UE_LOG(LogTemp, Warning, TEXT("Retarget offsets:\n"));
            for (auto &Elem : RetargetOffsets)
            {   
                UE_LOG(LogTemp, Warning, TEXT("(%s, \"%s\")\n"), *Elem.Key.ToString(), *Elem.Value.ToString());
            }
        }
        else
        {   
            // UE_LOG(LogTemp, Warning, TEXT("\t SrcBoneName non-nullptr = %s"), *SrcBoneName.ToString());
            NewName = *TargetBoneName;
            TransformedBoneNames.Add(*TargetBoneName);
        }



    }

    // get bones transforms in component space
    // if (retargetOffsetsExist == false)
    // {
    //     for (int32 i = 0; i < TransformedBoneNames.Num(); i++)
    //     {
    //         FName BoneName = TransformedBoneNames[i];
    //         // FTransform BoneTransform = InFrameData->Transforms[i];
    //         const int32 MeshIndex = OutPose.GetBoneContainer().GetPoseBoneIndexForBoneName(BoneName);
    //         if (MeshIndex != INDEX_NONE)
    //         {
    //             FCompactPoseBoneIndex CPIndex = OutPose.GetBoneContainer().MakeCompactPoseIndex(FMeshPoseBoneIndex(MeshIndex));
    //             if (CPIndex != INDEX_NONE)
    //             {
    //                 FTransform RefBoneCompSpaceTransform = GetRefBoneCompSpaceTransform(OutPose, CPIndex);
                    
    //                 FMatrix identityMatrix;
    //                 identityMatrix.SetIdentity();

    //                 FQuat quatOffset;
    //                 quatOffset = rotation_between_quats_to_quat(RefBoneCompSpaceTransform.GetRotation(), identityMatrix.ToQuat());

    //                 RetargetOffsets.Add(BoneName, quatOffset);
    //             }
    //         }
    //     }
    //     retargetOffsetsExist = true;
    //     UE_LOG(LogTemp, Warning, TEXT("Retarget offsets:\n"));
    //     for (auto &Elem : RetargetOffsets)
    //     {   
    //         UE_LOG(LogTemp, Warning, TEXT("(%s, \"%s\")\n"), *Elem.Key.ToString(), *Elem.Value.ToString());
    //     }
    // }

    

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


                const FName* RetargetSourceBoneName = BoneNameMap.FindKey(BoneName);

                // Only use position + rotation data for root. For all other bones, set rotation only.
                if (*RetargetSourceBoneName == GetTargetRootName())
                {
                    // OutPose[CPIndex].SetLocation(ConvertRootPosition(BoneTransform.GetTranslation()));
                    // FVector TempVector = BoneTransform.GetTranslation()                    
                    OutPose[CPIndex].SetLocation(ConvertPosition(BoneTransform.GetTranslation(), hips_loc_x, hips_loc_y, hips_loc_z));
                    // ConvertedLiveLinkRotation = ConvertRootRotation(BoneTransform.GetRotation());
                    ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), hips_rot_x, hips_rot_y, hips_rot_z);
                    // UE_LOG(LogTemp, Warning, TEXT("\t Bone name = %s"), *BoneName.ToString());
                    // UE_LOG(LogTemp, Warning, TEXT("Ref rot: %f, %f, %f"), RefBoneTransform.GetRotation().Euler().X, RefBoneTransform.GetRotation().Euler().Y, RefBoneTransform.GetRotation().Euler().Z);
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
                
                FQuat refBoneRotCompSpace = GetRefBoneCompSpaceTransform(OutPose, CPIndex).GetRotation();
                FQuat retargetOffset = *RetargetOffsets.Find(BoneName);
                // RefBoneRotCompSpace * ConvertedLiveLinkRotation * RetargetOffsets.Find(BoneName)
                // FVector appliedAnimRot = ConvertedLiveLinkRotation.RotateVector(refBoneRotCompSpace.Vector());

                // RefBoneTransform.ConcatenateRotation(ConvertedLiveLinkRotation);
                // RefBoneTransform.ConcatenateRotation(retargetOffset.Inverse());
                // FQuat finalRot = retargetOffset * ConvertedLiveLinkRotation * refBoneRotCompSpace;

                FQuat RefBoneRot = OutPose.GetRefPose(CPIndex).GetRotation();

                FQuat finalRot = retargetOffset * ConvertedLiveLinkRotation * refBoneRotCompSpace;
                // FQuat finalRot = retargetOffset * refBoneRotCompSpace;

                finalRot = RefBoneRot * finalRot;

                OutPose[CPIndex].SetRotation(finalRot);
                // OutPose[CPIndex].SetRotation(retargetOffset.RotateVector(appliedAnimRot).ToOrientationQuat());
                // OutPose[CPIndex].SetRotation(RefBoneTransform.GetRotation());

                if (*RetargetSourceBoneName == GetTargetRootName())
                {
                    // UE_LOG(LogTemp, Warning, TEXT("Pelvis comp space rot: %s"), *RefBoneTransform.ToString());
                    UE_LOG(LogTemp, Warning, TEXT("Pelvis comp space rot: %s"), *refBoneRotCompSpace.ToString());
                    UE_LOG(LogTemp, Warning, TEXT("Pelvis retarget offset: %s"), *retargetOffset.ToString());
                    UE_LOG(LogTemp, Warning, TEXT("Pelvis rotation: %s"), *ConvertedLiveLinkRotation.ToString());
                    UE_LOG(LogTemp, Warning, TEXT("Final rotation: %s"), *finalRot.ToString());
                }

                // FTransform RefBoneTransformDelta = RefBoneTransform;
                // // RefBoneTransformDelta.ConcatenateRotation(ConvertedLiveLinkRotation);
                // // RefBoneTransformDelta.ConcatenateRotation(RefBoneTransform.GetRotation().Inverse());

                // if (*RetargetSourceBoneName == GetTargetRootName())
                // {
                //     UE_LOG(LogTemp, Warning, TEXT("Delta rot: %f, %f, %f"), RefBoneTransformDelta.GetRotation().Euler().X, RefBoneTransformDelta.GetRotation().Euler().Y, RefBoneTransformDelta.GetRotation().Euler().Z);
                // }

                // // OutPose[CPIndex].SetRotation(RefBoneTransform.GetRotation().Inverse() * ConvertedLiveLinkRotation);
                // // Quaternion to add = Destination quaternion * (Source quaternion^-1)
                // FQuat IdentityQuat = RefBoneTransform.GetRotation() * RefBoneTransform.GetRotation().Inverse();
                // // IdentityQuat * ConvertedLiveLinkRotation;
                // OutPose[CPIndex].SetRotation(IdentityQuat * ConvertedLiveLinkRotation * RefBoneTransform.GetRotation());
                // // OutPose[CPIndex].SetRotation(RefBoneTransformDelta.GetRotation());
            }
        }
    }
}


// TArray<FTransform> UMoveaiLiveLinkRemapAssetBase::CalculateRigOffsets(USkeletalMeshComponent* SourceRig, USkeletalMeshComponent* TargetRig)
// {
//     GetMesh()
// }