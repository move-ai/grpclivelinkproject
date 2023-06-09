#pragma once

#include "CoreMinimal.h"

/**
 * Library of commonly used custom functions
 */
class QuatStaticLibrary
{
public:
    // float dot_qtqt(const float a[4], const float b[4])
    // {
    //     return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
    // }

    // void mul_qt_qtqt(float q[4], const float a[4], const float b[4])
    // {
    //     float t0, t1, t2;

    //     t0 = a[0] * b[0] - a[1] * b[1] - a[2] * b[2] - a[3] * b[3];
    //     t1 = a[0] * b[1] + a[1] * b[0] + a[2] * b[3] - a[3] * b[2];
    //     t2 = a[0] * b[2] + a[2] * b[0] + a[3] * b[1] - a[1] * b[3];
    //     q[3] = a[0] * b[3] + a[3] * b[0] + a[1] * b[2] - a[2] * b[1];
    //     q[0] = t0;
    //     q[1] = t1;
    //     q[2] = t2;
    // }

    // void mul_qt_fl(float q[4], const float f)
    // {
    //     q[0] *= f;
    //     q[1] *= f;
    //     q[2] *= f;
    //     q[3] *= f;
    // }

    // void conjugate_qt_qt(float q1[4], const float q2[4])
    // {
    //     q1[0] = q2[0];
    //     q1[1] = -q2[1];
    //     q1[2] = -q2[2];
    //     q1[3] = -q2[3];
    // }

    // // float q[4], const float q1[4], const float q2[4]
    // void rotation_between_quats_to_quat(float q[4], FQuat u_q1, FQuat u_q2)
    // {
    //     const float q1[4] = {u_q1.W, u_q1.X, u_q1.Y, u_q1.Z};
    //     // q1[0] = u_q1.W;
    //     // q1[1] = u_q1.X;
    //     // q1[2] = u_q1.Y;
    //     // q1[3] = u_q1.Z;

    //     const float q2[4] = {u_q2.W, u_q2.X, u_q2.Y, u_q2.Z};
    //     // q2[0] = u_q2.W;
    //     // q2[1] = u_q2.X;
    //     // q2[2] = u_q2.Y;
    //     // q2[3] = u_q2.Z;

    //     float tquat[4];

    //     conjugate_qt_qt(tquat, q1);

    //     mul_qt_fl(tquat, 1.0f / dot_qtqt(tquat, tquat));

    //     mul_qt_qtqt(q, tquat, q2);
    // }

	// Finds the difference between two quats
	static FQuat FindDeltaQuat(const FQuat& OldQuat, const FQuat& NewQuat)
	{
		return NewQuat * OldQuat.Inverse();
	}

	// Converts Quat from Local Rotation to World rotation (applies delta rotation in local space)
	static FQuat QuatLocalToWorld(const FQuat& LocalQuat, const FQuat& ComponentQuat)
	{
		return ComponentQuat * LocalQuat;
	}

	// Converts Quat from World Rotation to Local rotation
	static FQuat QuatWorldToLocal(const FQuat& WorldQuat, const FQuat& ComponentQuat)
	{
		return ComponentQuat.Inverse() * WorldQuat;
	}

	static FQuat ApplyDeltaQuatInWorldSpace(const FQuat& DeltaQuat, const FQuat& ComponentQuat)
	{
		return DeltaQuat * ComponentQuat;
	}

	static FQuat ApplyDeltaQuatInLocalSpace(const FQuat& DeltaQuat, const FQuat& ComponentQuat)
	{
		return QuatLocalToWorld(DeltaQuat, ComponentQuat);
	}
	
	static FRotator RotatorLocalToWorld(const FRotator& LocalRot, const FQuat& ComponentQuat)
	{
		return QuatStaticLibrary::QuatLocalToWorld(LocalRot.Quaternion(), ComponentQuat).Rotator();
	}
	
	static FRotator RotatorWorldToLocal(const FRotator& WorldRot, const FQuat& ComponentQuat)
	{
		return QuatStaticLibrary::QuatWorldToLocal(WorldRot.Quaternion(), ComponentQuat).Rotator();
	}
};