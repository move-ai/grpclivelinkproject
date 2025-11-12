// Fill out your copyright notice in the Description page of Project Settings.

#include "MoveaiLiveLinkRemapAssetBase.h"
#include "QuatStaticLibrary.h"

#include "Roles/LiveLinkAnimationTypes.h"
#include "BonePose.h"

#include "LogUtility.h"

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

FQuat rotation_between_quats_to_quat(FQuat u_q1, FQuat u_q2)
{
    const float q1[4] = {u_q1.W, u_q1.X, u_q1.Y, u_q1.Z};
    const float q2[4] = {u_q2.W, u_q2.X, u_q2.Y, u_q2.Z};

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

FTransform GetBoneCompSpaceTransform(FCompactPose& OutPose, FCompactPoseBoneIndex boneIdx)
{
    FTransform resultBoneTransform = OutPose[boneIdx];

    FCompactPoseBoneIndex parentBoneIndex = OutPose.GetParentBoneIndex(boneIdx);

    while (boneIdx.GetInt() != 0)
    {
        parentBoneIndex = OutPose.GetParentBoneIndex(boneIdx);
        resultBoneTransform *= OutPose[parentBoneIndex];
        boneIdx = parentBoneIndex;
    }

    return resultBoneTransform;
}

FTransform GetRefBoneCompSpaceTransform(FCompactPose& OutPose, FCompactPoseBoneIndex boneIdx)
{
    FTransform resultBoneTransform = OutPose.GetRefPose(boneIdx);

    FCompactPoseBoneIndex parentBoneIndex = OutPose.GetParentBoneIndex(boneIdx);

    while (boneIdx.GetInt() != 0)
    {
        parentBoneIndex = OutPose.GetParentBoneIndex(boneIdx);
        resultBoneTransform *= OutPose.GetRefPose(parentBoneIndex);
        boneIdx = parentBoneIndex;
    }

    return resultBoneTransform;
}

// Take Live Link data and apply it to skeleton bones in UE4.
void UMoveaiLiveLinkRemapAssetBase::BuildPoseFromAnimationData(
    float DeltaTime,
    const FLiveLinkSkeletonStaticData* InSkeletonData,
    const FLiveLinkAnimationFrameData* InFrameData,
    FCompactPose& OutPose)
{
    const TArray<FName>& SourceBoneNames = InSkeletonData->GetBoneNames();
    const TArray<int32>& SourceParentBoneNames = InSkeletonData->GetBoneParents();

    TArray<FName, TMemStackAllocator<>> TransformedBoneNames;
    TransformedBoneNames.Reserve(SourceBoneNames.Num());
    const FBoneContainer& BoneContainerRef = OutPose.GetBoneContainer();

    int32 MeshBoneIndex;
    FCompactPoseBoneIndex CPBoneIndex(1);

    // Map source->target bones and (optionally) precompute offsets
    for (const FName& SrcBoneName : SourceBoneNames)
    {
        FName* TargetBoneName = BoneNameMap.Find(SrcBoneName);
        FName NewName;
        if (TargetBoneName == nullptr)
        {
            NewName = GetRemappedBoneName(SrcBoneName);
            TransformedBoneNames.Add(NewName);
            BoneNameMap.Add(SrcBoneName, NewName);

            // Optional: compute retarget offsets (not used now)
            MeshBoneIndex = OutPose.GetBoneContainer().GetPoseBoneIndexForBoneName(NewName);
            if (MeshBoneIndex != INDEX_NONE)
            {
                CPBoneIndex = OutPose.GetBoneContainer().MakeCompactPoseIndex(FMeshPoseBoneIndex(MeshBoneIndex));
                if (CPBoneIndex != INDEX_NONE)
                {
                    FTransform RefBoneCompSpaceTransform = GetBoneCompSpaceTransform(OutPose, CPBoneIndex);

                    FMatrix identityMatrix;
                    identityMatrix.SetIdentity();

                    FQuat quatOffset = rotation_between_quats_to_quat(RefBoneCompSpaceTransform.GetRotation(), identityMatrix.ToQuat());
                    RetargetOffsets.Add(NewName, quatOffset); // unused for now
                }
            }
        }
        else
        {
            NewName = *TargetBoneName;
            TransformedBoneNames.Add(*TargetBoneName);
        }
    }

    FName HipsNameInTargetSkeleton = NAME_None;
    int32 HipsMeshIndexInTargetSkeleton = INDEX_NONE;
    int32 RootBone = InSkeletonData->FindRootBone();
    if (RootBone != -1)
    {
        HipsNameInTargetSkeleton = *BoneNameMap.Find(SourceBoneNames[RootBone]);
        HipsMeshIndexInTargetSkeleton = BoneContainerRef.GetPoseBoneIndexForBoneName(HipsNameInTargetSkeleton);
    }

    if (!bHaveInit)
    {
        while (MeshBoneIndex != INDEX_NONE)
        {
            HipsParentsTransform *= BoneContainerRef.GetRefPoseArray()[MeshBoneIndex];
            MeshBoneIndex = BoneContainerRef.GetParentBoneIndex(MeshBoneIndex);
        }

        // Build world rotations/positions from ref pose (kept for completeness)
        int32 MeshBoneCount = BoneContainerRef.GetNumBones();
        WorldRotations.Init(FQuat::Identity, MeshBoneCount);
        WorldPositions.Init(FVector::ZeroVector, MeshBoneCount);

        const TArray<FTransform>& MeshBoneRefPose = BoneContainerRef.GetRefPoseArray();
        for (int32 Index = 0; Index < MeshBoneCount; Index++)
        {
            FQuat Rotation = MeshBoneRefPose[Index].GetRotation();
            FVector Position = MeshBoneRefPose[Index].GetLocation();

            int32 ParentIndex = BoneContainerRef.GetParentBoneIndex(Index);
            if ((ParentIndex != INDEX_NONE) && (ParentIndex < MeshBoneCount))
            {
                Rotation = WorldRotations[ParentIndex] * Rotation;
                Position = WorldRotations[ParentIndex] * Position + WorldPositions[ParentIndex];
            }
            else
            {
                FTransform T0 = FTransform(Rotation, Position);
                Rotation = T0.GetRotation();
                Position = T0.GetLocation();
            }

            WorldRotations[Index] = Rotation;
            WorldPositions[Index] = Position;
        }

        bHaveInit = true;
    }

    // Apply incoming Live Link transforms
    for (int32 i = 0; i < TransformedBoneNames.Num(); i++)
    {
        FName BoneName = TransformedBoneNames[i];
        FName LogicParentBoneName = NAME_None;

        int32 SourceParentIndex = SourceParentBoneNames[i];
        if (SourceParentIndex != INDEX_NONE)
        {
            LogicParentBoneName = TransformedBoneNames[SourceParentIndex];
        }

        FTransform BoneTransform = InFrameData->Transforms[i];

        MeshBoneIndex = BoneContainerRef.GetPoseBoneIndexForBoneName(BoneName);
        if (MeshBoneIndex == INDEX_NONE)
        {
            continue;
        }

        CPBoneIndex = BoneContainerRef.MakeCompactPoseIndex(FMeshPoseBoneIndex(MeshBoneIndex));
        if (CPBoneIndex == INDEX_NONE)
        {
            continue;
        }

        FQuat ConvertedLiveLinkRotation = FQuat::Identity;
        const FName* RetargetSourceBoneName = BoneNameMap.FindKey(BoneName);

        // Root: set position + rotation
        if (*RetargetSourceBoneName == GetTargetRootName())
        {
            OutPose[CPBoneIndex].SetLocation(ConvertPosition(BoneTransform.GetTranslation(), hips_loc_x, hips_loc_y, hips_loc_z));
            const FQuat Delta = ConvertRotation(BoneTransform.GetRotation(), hips_rot_x, hips_rot_y, hips_rot_z);
            const FQuat BindLocalRoot = OutPose.GetRefPose(CPBoneIndex).GetRotation();
            OutPose[CPBoneIndex].SetRotation((BindLocalRoot * Delta).GetNormalized());
            continue;
        }
        else
        {
            // Compute per-bone delta (already in LiveLink local-of-ref space)
            if (*RetargetSourceBoneName == GetMidSpineName())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), midSpine_rot_x, midSpine_rot_y, midSpine_rot_z);
            }
            else if (*RetargetSourceBoneName == Get2ndMidSpineName())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), midSpine_rot_x, midSpine_rot_y, midSpine_rot_z);
            }
            else if (*RetargetSourceBoneName == GetNeckName())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), neck_rot_x, neck_rot_y, neck_rot_z);
            }
            // Right arm
            else if (*RetargetSourceBoneName == GetTargetRightClavicleName())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), rightClavicle_rot_x, rightClavicle_rot_y, rightClavicle_rot_z);
            }
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
            else if (*RetargetSourceBoneName == GetTargetLeftClavicleName())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), leftClavicle_rot_x, leftClavicle_rot_y, leftClavicle_rot_z);
            }
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
            // Fingers right
            else if (*RetargetSourceBoneName == GetRightThumb1Name())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), rightThumb1_rot_x, rightThumb1_rot_y, rightThumb1_rot_z);
            }
            else if (*RetargetSourceBoneName == GetRightThumb2Name())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), rightThumb2_rot_x, rightThumb2_rot_y, rightThumb2_rot_z);
            }
            else if (*RetargetSourceBoneName == GetRightThumb3Name())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), rightThumb3_rot_x, rightThumb3_rot_y, rightThumb3_rot_z);
            }
            else if (*RetargetSourceBoneName == GetRightIndex1Name())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), rightIndex1_rot_x, rightIndex1_rot_y, rightIndex1_rot_z);
            }
            else if (*RetargetSourceBoneName == GetRightIndex2Name())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), rightIndex2_rot_x, rightIndex2_rot_y, rightIndex2_rot_z);
            }
            else if (*RetargetSourceBoneName == GetRightIndex3Name())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), rightIndex3_rot_x, rightIndex3_rot_y, rightIndex3_rot_z);
            }
            else if (*RetargetSourceBoneName == GetRightMiddle1Name())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), rightMiddle1_rot_x, rightMiddle1_rot_y, rightMiddle1_rot_z);
            }
            else if (*RetargetSourceBoneName == GetRightMiddle2Name())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), rightMiddle2_rot_x, rightMiddle2_rot_y, rightMiddle2_rot_z);
            }
            else if (*RetargetSourceBoneName == GetRightMiddle3Name())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), rightMiddle3_rot_x, rightMiddle3_rot_y, rightMiddle3_rot_z);
            }
            else if (*RetargetSourceBoneName == GetRightRing1Name())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), rightRing1_rot_x, rightRing1_rot_y, rightRing1_rot_z);
            }
            else if (*RetargetSourceBoneName == GetRightRing2Name())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), rightRing2_rot_x, rightRing2_rot_y, rightRing2_rot_z);
            }
            else if (*RetargetSourceBoneName == GetRightRing3Name())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), rightRing3_rot_x, rightRing3_rot_y, rightRing3_rot_z);
            }
            else if (*RetargetSourceBoneName == GetRightPinky1Name())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), rightPinky1_rot_x, rightPinky1_rot_y, rightPinky1_rot_z);
            }
            else if (*RetargetSourceBoneName == GetRightPinky2Name())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), rightPinky2_rot_x, rightPinky2_rot_y, rightPinky2_rot_z);
            }
            else if (*RetargetSourceBoneName == GetRightPinky3Name())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), rightPinky3_rot_x, rightPinky3_rot_y, rightPinky3_rot_z);
            }
            // Fingers left
            else if (*RetargetSourceBoneName == GetLeftThumb1Name())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), leftThumb1_rot_x, leftThumb1_rot_y, leftThumb1_rot_z);
            }
            else if (*RetargetSourceBoneName == GetLeftThumb2Name())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), leftThumb2_rot_x, leftThumb2_rot_y, leftThumb2_rot_z);
            }
            else if (*RetargetSourceBoneName == GetLeftThumb3Name())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), leftThumb3_rot_x, leftThumb3_rot_y, leftThumb3_rot_z);
            }
            else if (*RetargetSourceBoneName == GetLeftIndex1Name())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), leftIndex1_rot_x, leftIndex1_rot_y, leftIndex1_rot_z);
            }
            else if (*RetargetSourceBoneName == GetLeftIndex2Name())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), leftIndex2_rot_x, leftIndex2_rot_y, leftIndex2_rot_z);
            }
            else if (*RetargetSourceBoneName == GetLeftIndex3Name())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), leftIndex3_rot_x, leftIndex3_rot_y, leftIndex3_rot_z);
            }
            else if (*RetargetSourceBoneName == GetLeftMiddle1Name())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), leftMiddle1_rot_x, leftMiddle1_rot_y, leftMiddle1_rot_z);
            }
            else if (*RetargetSourceBoneName == GetLeftMiddle2Name())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), leftMiddle2_rot_x, leftMiddle2_rot_y, leftMiddle2_rot_z);
            }
            else if (*RetargetSourceBoneName == GetLeftMiddle3Name())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), leftMiddle3_rot_x, leftMiddle3_rot_y, leftMiddle3_rot_z);
            }
            else if (*RetargetSourceBoneName == GetLeftRing1Name())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), leftRing1_rot_x, leftRing1_rot_y, leftRing1_rot_z);
            }
            else if (*RetargetSourceBoneName == GetLeftRing2Name())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), leftRing2_rot_x, leftRing2_rot_y, leftRing2_rot_z);
            }
            else if (*RetargetSourceBoneName == GetLeftRing3Name())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), leftRing3_rot_x, leftRing3_rot_y, leftRing3_rot_z);
            }
            else if (*RetargetSourceBoneName == GetLeftPinky1Name())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), leftPinky1_rot_x, leftPinky1_rot_y, leftPinky1_rot_z);
            }
            else if (*RetargetSourceBoneName == GetLeftPinky2Name())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), leftPinky2_rot_x, leftPinky2_rot_y, leftPinky2_rot_z);
            }
            else if (*RetargetSourceBoneName == GetLeftPinky3Name())
            {
                ConvertedLiveLinkRotation = ConvertRotation(BoneTransform.GetRotation(), leftPinky3_rot_x, leftPinky3_rot_y, leftPinky3_rot_z);
            }
        }

        // Apply LOCAL rotation: bind * delta (ensures rotation about the bone's local axes)
        const FQuat BindLocal = OutPose.GetRefPose(CPBoneIndex).GetRotation();
        const FQuat FinalLocal = (BindLocal * ConvertedLiveLinkRotation).GetNormalized();
        OutPose[CPBoneIndex].SetRotation(FinalLocal);
    }
}
