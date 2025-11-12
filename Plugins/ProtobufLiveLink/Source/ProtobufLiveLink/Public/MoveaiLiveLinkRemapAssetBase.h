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

UCLASS() //showCategories = ("Retarget from", "Retarget to"))
class PROTOBUFLIVELINK_API UMoveaiLiveLinkRemapAssetBase : public ULiveLinkRemapAsset
{
    GENERATED_BODY()

    virtual void BuildPoseFromAnimationData(float DeltaTime, const FLiveLinkSkeletonStaticData* InSkeletonData,
                                            const FLiveLinkAnimationFrameData* InFrameData,
                                            FCompactPose& OutPose) override;

    

public:    
    // UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
    // USkeletalMeshComponent* SMWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SMWeapon"));

    // UPROPERTY(EditAnywhere, SimpleDisplay, Category = "Retarget from")
	// USkeletalMeshComponent *SourceRig;
    // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Retarget to", meta=(UseComponentPicker,AllowedClasses="SkeletalMeshComponent",AllowAnyActor))
	// FComponentReference TargetRig;
    // TArray<FTransform> CalculateRigOffsets(USkeletalMeshComponent* SourceRig, USkeletalMeshComponent* TargetRig);
    // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hips")
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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right clavicle")
	ERetargetAxis rightClavicle_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right clavicle")
    ERetargetAxis rightClavicle_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right clavicle")
    ERetargetAxis rightClavicle_rot_z;
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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left clavicle")
	ERetargetAxis leftClavicle_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left clavicle")
    ERetargetAxis leftClavicle_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left clavicle")
    ERetargetAxis leftClavicle_rot_z;
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

    // Right fingers
    // Index
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right index finger 1")
    ERetargetAxis rightIndex1_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right index finger 1")
    ERetargetAxis rightIndex1_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right index finger 1")
    ERetargetAxis rightIndex1_rot_z;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right index finger 2")
    ERetargetAxis rightIndex2_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right index finger 2")
    ERetargetAxis rightIndex2_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right index finger 2")
    ERetargetAxis rightIndex2_rot_z;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right index finger 3")
    ERetargetAxis rightIndex3_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right index finger 3")
    ERetargetAxis rightIndex3_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right index finger 3")
    ERetargetAxis rightIndex3_rot_z;

    // Middle
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right middle finger 1")
    ERetargetAxis rightMiddle1_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right middle finger 1")
    ERetargetAxis rightMiddle1_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right middle finger 1")
    ERetargetAxis rightMiddle1_rot_z;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right middle finger 2")
    ERetargetAxis rightMiddle2_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right middle finger 2")
    ERetargetAxis rightMiddle2_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right middle finger 2")
    ERetargetAxis rightMiddle2_rot_z;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right middle finger 3")
    ERetargetAxis rightMiddle3_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right middle finger 3")
    ERetargetAxis rightMiddle3_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right middle finger 3")
    ERetargetAxis rightMiddle3_rot_z;

    // Ring
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right ring finger 1")
    ERetargetAxis rightRing1_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right ring finger 1")
    ERetargetAxis rightRing1_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right ring finger 1")
    ERetargetAxis rightRing1_rot_z;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right ring finger 2")
    ERetargetAxis rightRing2_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right ring finger 2")
    ERetargetAxis rightRing2_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right ring finger 2")
    ERetargetAxis rightRing2_rot_z;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right ring finger 3")
    ERetargetAxis rightRing3_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right ring finger 3")
    ERetargetAxis rightRing3_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right ring finger 3")
    ERetargetAxis rightRing3_rot_z;

    // Pinky
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right pinky finger 1")
    ERetargetAxis rightPinky1_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right pinky finger 1")
    ERetargetAxis rightPinky1_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right pinky finger 1")
    ERetargetAxis rightPinky1_rot_z;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right pinky finger 2")
    ERetargetAxis rightPinky2_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right pinky finger 2")
    ERetargetAxis rightPinky2_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right pinky finger 2")
    ERetargetAxis rightPinky2_rot_z;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right pinky finger 3")
    ERetargetAxis rightPinky3_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right pinky finger 3")
    ERetargetAxis rightPinky3_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right pinky finger 3")
    ERetargetAxis rightPinky3_rot_z;

    // Thumb
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right thumb finger 1")
    ERetargetAxis rightThumb1_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right thumb finger 1")
    ERetargetAxis rightThumb1_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right thumb finger 1")
    ERetargetAxis rightThumb1_rot_z;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right thumb finger 2")
    ERetargetAxis rightThumb2_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right thumb finger 2")
    ERetargetAxis rightThumb2_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right thumb finger 2")
    ERetargetAxis rightThumb2_rot_z;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right thumb finger 3")
    ERetargetAxis rightThumb3_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right thumb finger 3")
    ERetargetAxis rightThumb3_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Right thumb finger 3")
    ERetargetAxis rightThumb3_rot_z;

    // Left fingers
    // Index
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left index finger 1")
    ERetargetAxis leftIndex1_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left index finger 1")
    ERetargetAxis leftIndex1_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left index finger 1")
    ERetargetAxis leftIndex1_rot_z;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left index finger 2")
    ERetargetAxis leftIndex2_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left index finger 2")
    ERetargetAxis leftIndex2_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left index finger 2")
    ERetargetAxis leftIndex2_rot_z;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left index finger 3")
    ERetargetAxis leftIndex3_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left index finger 3")
    ERetargetAxis leftIndex3_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left index finger 3")
    ERetargetAxis leftIndex3_rot_z;

    // Middle
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left middle finger 1")
    ERetargetAxis leftMiddle1_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left middle finger 1")
    ERetargetAxis leftMiddle1_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left middle finger 1")
    ERetargetAxis leftMiddle1_rot_z;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left middle finger 2")
    ERetargetAxis leftMiddle2_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left middle finger 2")
    ERetargetAxis leftMiddle2_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left middle finger 2")
    ERetargetAxis leftMiddle2_rot_z;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left middle finger 3")
    ERetargetAxis leftMiddle3_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left middle finger 3")
    ERetargetAxis leftMiddle3_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left middle finger 3")
    ERetargetAxis leftMiddle3_rot_z;

    // Ring
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left ring finger 1")
    ERetargetAxis leftRing1_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left ring finger 1")
    ERetargetAxis leftRing1_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left ring finger 1")
    ERetargetAxis leftRing1_rot_z;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left ring finger 2")
    ERetargetAxis leftRing2_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left ring finger 2")
    ERetargetAxis leftRing2_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left ring finger 2")
    ERetargetAxis leftRing2_rot_z;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left ring finger 3")
    ERetargetAxis leftRing3_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left ring finger 3")
    ERetargetAxis leftRing3_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left ring finger 3")
    ERetargetAxis leftRing3_rot_z;

    // Pinky
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left pinky finger 1")
    ERetargetAxis leftPinky1_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left pinky finger 1")
    ERetargetAxis leftPinky1_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left pinky finger 1")
    ERetargetAxis leftPinky1_rot_z;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left pinky finger 2")
    ERetargetAxis leftPinky2_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left pinky finger 2")
    ERetargetAxis leftPinky2_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left pinky finger 2")
    ERetargetAxis leftPinky2_rot_z;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left pinky finger 3")
    ERetargetAxis leftPinky3_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left pinky finger 3")
    ERetargetAxis leftPinky3_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left pinky finger 3")
    ERetargetAxis leftPinky3_rot_z;

    // Thumb
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left thumb finger 1")
    ERetargetAxis leftThumb1_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left thumb finger 1")
    ERetargetAxis leftThumb1_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left thumb finger 1")
    ERetargetAxis leftThumb1_rot_z;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left thumb finger 2")
    ERetargetAxis leftThumb2_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left thumb finger 2")
    ERetargetAxis leftThumb2_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left thumb finger 2")
    ERetargetAxis leftThumb2_rot_z;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left thumb finger 3")
    ERetargetAxis leftThumb3_rot_x;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left thumb finger 3")
    ERetargetAxis leftThumb3_rot_y;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Left thumb finger 3")
    ERetargetAxis leftThumb3_rot_z;


    FVector NewRetargetLocation;
    FQuat NewRetargetRotation;

    // UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="retargetOffsetsExist")
    // bool retargetOffsetsExist = false;

protected:
    // Override these in child classes
    virtual FVector ConvertPosition(FVector LLPosition, ERetargetAxis X, ERetargetAxis Y, ERetargetAxis Z) { return NewRetargetLocation; };
    virtual FQuat ConvertRotation(FQuat LLRotation, ERetargetAxis X, ERetargetAxis Y, ERetargetAxis Z) { return NewRetargetRotation; };
    // virtual FQuat ConvertBoneRotation(FQuat LLRotation) const { return LLRotation; };

    // virtual FVector ConvertRootPosition(FVector LLPosition) const { return LLPosition; };
    virtual FQuat ConvertRootRotation(FQuat LLRotation) const { return LLRotation; };
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
    virtual FName GetMidSpineName() const { return "Spine1"; }
    virtual FName Get2ndMidSpineName() const { return "Spine2"; }
    virtual FName GetNeckName() const { return "Neck"; }

    // Right arm
    virtual FName GetTargetRightClavicleName() const { return "R_Clavicle"; }
    virtual FName GetTargetRightArmName() const { return "R_Shoulder"; }
    virtual FName GetTargetRightForeArmName() const { return "R_Elbow"; }
    virtual FName GetRightHandName() const { return "R_Wrist"; }

    // Left arm
    virtual FName GetTargetLeftClavicleName() const { return "L_Clavicle"; }
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

    // Left fingers
    virtual FName GetLeftIndex1Name() const { return "L_Hand_Index1"; }
    virtual FName GetLeftIndex2Name() const { return "L_Hand_Index2"; }
    virtual FName GetLeftIndex3Name() const { return "L_Hand_Index3"; }

    virtual FName GetLeftMiddle1Name() const { return "L_Hand_Middle1"; }
    virtual FName GetLeftMiddle2Name() const { return "L_Hand_Middle2"; }
    virtual FName GetLeftMiddle3Name() const { return "L_Hand_Middle3"; }

    virtual FName GetLeftRing1Name() const { return "L_Hand_Ring1"; }
    virtual FName GetLeftRing2Name() const { return "L_Hand_Ring2"; }
    virtual FName GetLeftRing3Name() const { return "L_Hand_Ring3"; }

    virtual FName GetLeftPinky1Name() const { return "L_Hand_Pinky1"; }
    virtual FName GetLeftPinky2Name() const { return "L_Hand_Pinky2"; }
    virtual FName GetLeftPinky3Name() const { return "L_Hand_Pinky3"; }

    virtual FName GetLeftThumb1Name() const { return "L_Hand_Thumb1"; }
    virtual FName GetLeftThumb2Name() const { return "L_Hand_Thumb2"; }
    virtual FName GetLeftThumb3Name() const { return "L_Hand_Thumb3"; }

    // Right fingers
    virtual FName GetRightIndex1Name() const { return "R_Hand_Index1"; }
    virtual FName GetRightIndex2Name() const { return "R_Hand_Index2"; }
    virtual FName GetRightIndex3Name() const { return "R_Hand_Index3"; }

    virtual FName GetRightMiddle1Name() const { return "R_Hand_Middle1"; }
    virtual FName GetRightMiddle2Name() const { return "R_Hand_Middle2"; }
    virtual FName GetRightMiddle3Name() const { return "R_Hand_Middle3"; }

    virtual FName GetRightRing1Name() const { return "R_Hand_Ring1"; }
    virtual FName GetRightRing2Name() const { return "R_Hand_Ring2"; }
    virtual FName GetRightRing3Name() const { return "R_Hand_Ring3"; }

    virtual FName GetRightPinky1Name() const { return "R_Hand_Pinky1"; }
    virtual FName GetRightPinky2Name() const { return "R_Hand_Pinky2"; }
    virtual FName GetRightPinky3Name() const { return "R_Hand_Pinky3"; }

    virtual FName GetRightThumb1Name() const { return "R_Hand_Thumb1"; }
    virtual FName GetRightThumb2Name() const { return "R_Hand_Thumb2"; }
    virtual FName GetRightThumb3Name() const { return "R_Hand_Thumb3"; }

    // Cached lookup results from GetRemappedBoneName
    TMap<FName, FName> BoneNameMap;
    TMap<FName, FQuat> RetargetOffsets;
    UPROPERTY ()
		bool bHaveInit = false;

	// MeshBone 
	UPROPERTY ()
		TArray<FQuat> WorldRotations;

	// MeshBone
	UPROPERTY ()
		TArray<FVector> WorldPositions;
    UPROPERTY ()
		FTransform HipsParentsTransform = FTransform::Identity;
};
