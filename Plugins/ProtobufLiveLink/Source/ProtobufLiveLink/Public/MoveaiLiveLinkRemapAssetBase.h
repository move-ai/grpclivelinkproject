// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LiveLinkRemapAsset.h"
#include "MoveaiLiveLinkRemapAssetBase.generated.h"

/**
 * 
 */

UENUM()
enum class ERetargetAxis : uint8
{
    X_pos UMETA(DisplayName = "X"),
    Y_pos UMETA(DisplayName = "Y"),
    Z_pos UMETA(DisplayName = "Z"),
    X_neg UMETA(DisplayName = "-X"),
    Y_neg UMETA(DisplayName = "-Y"),
    Z_neg UMETA(DisplayName = "-Z"),
};

UCLASS()
class PROTOBUFLIVELINK_API UMoveaiLiveLinkRemapAssetBase : public ULiveLinkRemapAsset
{
    GENERATED_BODY()

    virtual void BuildPoseFromAnimationData(float DeltaTime, const FLiveLinkSkeletonStaticData* InSkeletonData,
                                            const FLiveLinkAnimationFrameData* InFrameData,
                                            FCompactPose& OutPose) override;

public:
    // Spine
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hips")
	ERetargetAxis hips_loc_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hips")
    ERetargetAxis hips_loc_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hips")
    ERetargetAxis hips_loc_z;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hips")
	ERetargetAxis hips_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hips")
    ERetargetAxis hips_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hips")
    ERetargetAxis hips_rot_z;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hips")
    int32 root_scale;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Middle spine")
	ERetargetAxis midSpine_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Middle spine")
    ERetargetAxis midSpine_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Middle spine")
    ERetargetAxis midSpine_rot_z;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Neck")
	ERetargetAxis neck_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Neck")
    ERetargetAxis neck_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Neck")
    ERetargetAxis neck_rot_z;

    // Right arm
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right arm")
	ERetargetAxis rightArm_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right arm")
    ERetargetAxis rightArm_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right arm")
    ERetargetAxis rightArm_rot_z;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right forearm")
	ERetargetAxis rightForearm_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right forearm")
    ERetargetAxis rightForearm_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right forearm")
    ERetargetAxis rightForearm_rot_z;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right hand")
	ERetargetAxis rightHand_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right hand")
    ERetargetAxis rightHand_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right hand")
    ERetargetAxis rightHand_rot_z;

    // Left arm
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left arm")
	ERetargetAxis leftArm_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left arm")
    ERetargetAxis leftArm_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left arm")
    ERetargetAxis leftArm_rot_z;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left forearm")
	ERetargetAxis leftForearm_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left forearm")
    ERetargetAxis leftForearm_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left forearm")
    ERetargetAxis leftForearm_rot_z;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left hand")
	ERetargetAxis leftHand_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left hand")
    ERetargetAxis leftHand_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left hand")
    ERetargetAxis leftHand_rot_z;

    // Right leg
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right upper leg")
	ERetargetAxis rightUpLeg_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right upper leg")
    ERetargetAxis rightUpLeg_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right upper leg")
    ERetargetAxis rightUpLeg_rot_z;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right leg")
	ERetargetAxis rightLeg_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right leg")
    ERetargetAxis rightLeg_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right leg")
    ERetargetAxis rightLeg_rot_z;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right foot")
	ERetargetAxis rightFoot_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right foot")
    ERetargetAxis rightFoot_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right foot")
    ERetargetAxis rightFoot_rot_z;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right toe")
	ERetargetAxis rightToe_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right toe")
    ERetargetAxis rightToe_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right toe")
    ERetargetAxis rightToe_rot_z;

    // Left leg
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left upper leg")
	ERetargetAxis leftUpLeg_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left upper leg")
    ERetargetAxis leftUpLeg_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left upper leg")
    ERetargetAxis leftUpLeg_rot_z;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left leg")
	ERetargetAxis leftLeg_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left leg")
    ERetargetAxis leftLeg_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left leg")
    ERetargetAxis leftLeg_rot_z;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left foot")
	ERetargetAxis leftFoot_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left foot")
    ERetargetAxis leftFoot_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left foot")
    ERetargetAxis leftFoot_rot_z;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left toe")
	ERetargetAxis leftToe_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left toe")
    ERetargetAxis leftToe_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left toe")
    ERetargetAxis leftToe_rot_z;

    FVector NewRetargetLocation;
    FQuat NewRetargetRotation;

protected:
    // Override these in child classes
    virtual FVector ConvertPosition(FVector LLPosition, ERetargetAxis X, ERetargetAxis Y, ERetargetAxis Z) { return NewRetargetLocation; };
    virtual FQuat ConvertRotation(FQuat LLRotation, ERetargetAxis X, ERetargetAxis Y, ERetargetAxis Z) { return NewRetargetRotation; };
    // virtual FQuat ConvertBoneRotation(FQuat LLRotation) const { return LLRotation; };

    // virtual FVector ConvertRootPosition(FVector LLPosition) const { return LLPosition; };
    // virtual FQuat ConvertRootRotation(FQuat LLRotation) const { return LLRotation; };
    // virtual FQuat ConvertBoneRotation(FQuat LLRotation) const { return LLRotation; };
    // virtual FQuat ConvertRightArmRotation(FQuat LLRotation) const { return LLRotation; };
    // virtual FQuat ConvertRightForeArmRotation(FQuat LLRotation) const { return LLRotation; };
    // virtual FQuat ConvertLeftArmRotation(FQuat LLRotation) const { return LLRotation; };
    // virtual FQuat ConvertLeftForeArmRotation(FQuat LLRotation) const { return LLRotation; };
    // virtual FQuat ConvertMidSpineRotation(FQuat LLRotation) const { return LLRotation; };
    // virtual FQuat ConvertNeckRotation(FQuat LLRotation) const { return LLRotation; };
    // virtual FQuat ConvertRightToeRotation(FQuat LLRotation) const { return LLRotation; };
    // virtual FQuat ConvertLeftToeRotation(FQuat LLRotation) const { return LLRotation; };
    // virtual FQuat ConvertRightFootRotation(FQuat LLRotation) const { return LLRotation; };
    // virtual FQuat ConvertLeftFootRotation(FQuat LLRotation) const { return LLRotation; };
    // virtual FQuat ConvertRightHandRotation(FQuat LLRotation) const { return LLRotation; };
    // virtual FQuat ConvertLeftHandRotation(FQuat LLRotation) const { return LLRotation; };

    // This is the bone we will apply position translation to.
    // Radical 3.1 Live Link position data is mapped to the hips.
    // For other skeletons, the hip bone's name can differ and you can override this method.
    // Spine
    virtual FName GetTargetRootName() const { return "R"; }
    virtual FName GetMidSpineName() const { return "Spine"; }
    virtual FName GetNeckName() const { return "Neck"; }

    // Right arm
    virtual FName GetTargetRightArmName() const { return "R_Shoulder"; }
    virtual FName GetTargetRightForeArmName() const { return "R_Elbow"; }
    virtual FName GetRightHandName() const { return "R_Wrist"; }

    // Left arm
    virtual FName GetTargetLeftArmName() const { return "L_Shoulder"; }
    virtual FName GetTargetLeftForeArmName() const { return "L_Elbow"; }
    virtual FName GetLeftHandName() const { return "L_Wrist"; }

    // Right leg
    virtual FName GetRightUpLeg() const { return "R_Hip"; }
    virtual FName GetRightLeg() const { return "R_Knee"; }
    virtual FName GetRightFootName() const { return "R_Ankle"; }
    virtual FName GetRightToeName() const { return "R_Foot_Pinky"; }

    // Left leg
    virtual FName GetLeftUpLeg() const { return "L_Hip"; }
    virtual FName GetLeftLeg() const { return "L_Knee"; }
    virtual FName GetLeftFootName() const { return "L_Ankle"; }
    virtual FName GetLeftToeName() const { return "L_Foot_Pinky"; }
      

    // Cached lookup results from GetRemappedBoneName
    TMap<FName, FName> BoneNameMap;

};
