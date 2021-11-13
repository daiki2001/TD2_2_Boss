#include "Collision.h"
#include <DirectXMath.h>

namespace Collision
{
static Vector3 distance{}; //二点間の距離

bool IsBallToBallCollision(const Vector3& pos1, const float& r1, const Vector3& pos2, const float& r2)
{
	distance = pos1 - pos2;

	return distance.Length() < (r1 + r2);
}

bool IsBoxToBoxCollision(const Vector3& pos1, const Vector3& rotation1, const Vector3& scale1, const Vector3& pos2, const Vector3& rotation2, const Vector3& scale2)
{
	using namespace DirectX;

	// 回転行列
	static XMMATRIX matRot1{}, matRot2{};
	// 中心から面までの長さ
	static Vector3 length1{}, length2{};
	// 分離軸ベクトル
	static Vector3 direction1_XN{}, direction1_YN{}, direction1_ZN{}, direction2_XN{}, direction2_YN{}, direction2_ZN{};
	// 方向ベクトル
	static Vector3 direction1_X{}, direction1_Y{}, direction1_Z{}, direction2_X{}, direction2_Y{}, direction2_Z{};

	matRot1 = XMMatrixIdentity();
	matRot1 *= XMMatrixRotationZ(XMConvertToRadians(rotation1.z));
	matRot1 *= XMMatrixRotationX(XMConvertToRadians(rotation1.x));
	matRot1 *= XMMatrixRotationY(XMConvertToRadians(rotation1.y));

	length1 = scale1 / 2.0f;
	direction1_XN = XMVector4Normalize(matRot1.r[0]);
	direction1_YN = XMVector4Normalize(matRot1.r[1]);
	direction1_ZN = XMVector4Normalize(matRot1.r[2]);
	direction1_X = direction1_XN * length1.x;
	direction1_Y = direction1_YN * length1.y;
	direction1_Z = direction1_ZN * length1.z;

	matRot2 = XMMatrixIdentity();
	matRot2 *= XMMatrixRotationZ(XMConvertToRadians(rotation2.z));
	matRot2 *= XMMatrixRotationX(XMConvertToRadians(rotation2.x));
	matRot2 *= XMMatrixRotationY(XMConvertToRadians(rotation2.y));

	length2 = scale2 / 2.0f;
	direction2_XN = XMVector4Normalize(matRot2.r[0]);
	direction2_YN = XMVector4Normalize(matRot2.r[1]);
	direction2_ZN = XMVector4Normalize(matRot2.r[2]);
	direction2_X = direction2_XN * length2.x;
	direction2_Y = direction2_YN * length2.y;
	direction2_Z = direction2_ZN * length2.z;

	distance = pos1 - pos2;

	static float r1{}, r2{}; //半投影線分
	static float l{}; //中間点間の距離

	// 分離軸ベクトル：direction1_XN
	r1 = direction1_X.Length();
	r2 = fabsf(direction1_XN.VDot(direction2_X)) + fabsf(direction1_XN.VDot(direction2_Y)) + fabsf(direction1_XN.VDot(direction2_Z));
	l = fabsf(direction1_XN.VDot(distance));
	if ((r1 + r2) < l)
	{
		return false;
	}

	// 分離軸ベクトル：direction1_YN
	r1 = direction1_Y.Length();
	r2 = fabsf(direction1_YN.VDot(direction2_X)) + fabsf(direction1_YN.VDot(direction2_Y)) + fabsf(direction1_YN.VDot(direction2_Z));
	l = fabsf(direction1_YN.VDot(distance));
	if ((r1 + r2) < l)
	{
		return false;
	}

	// 分離軸ベクトル：direction1_ZN
	r1 = direction1_Z.Length();
	r2 = fabsf(direction1_ZN.VDot(direction2_X)) + fabsf(direction1_ZN.VDot(direction2_Y)) + fabsf(direction1_ZN.VDot(direction2_Z));
	l = fabsf(direction1_ZN.VDot(distance));
	if ((r1 + r2) < l)
	{
		return false;
	}

	// 分離軸ベクトル：direction2_XN
	r1 = direction2_X.Length();
	r2 = fabsf(direction2_XN.VDot(direction1_X)) + fabsf(direction2_XN.VDot(direction1_Y)) + fabsf(direction2_XN.VDot(direction1_Z));
	l = fabsf(direction2_XN.VDot(distance));
	if ((r1 + r2) < l)
	{
		return false;
	}

	// 分離軸ベクトル：direction2_YN
	r1 = direction2_Y.Length();
	r2 = fabsf(direction2_YN.VDot(direction1_X)) + fabsf(direction2_YN.VDot(direction1_Y)) + fabsf(direction2_YN.VDot(direction1_Z));
	l = fabsf(direction2_YN.VDot(distance));
	if ((r1 + r2) < l)
	{
		return false;
	}

	// 分離軸ベクトル：direction2_ZN
	r1 = direction2_Z.Length();
	r2 = fabsf(direction2_ZN.VDot(direction1_X)) + fabsf(direction2_ZN.VDot(direction1_Y)) + fabsf(direction2_ZN.VDot(direction1_Z));
	l = fabsf(direction2_ZN.VDot(distance));
	if ((r1 + r2) < l)
	{
		return false;
	}

	static Vector3 crossDirection{}; //外積分離軸

	// 分離軸ベクトル：Cross(direction1_X, direction2_X)
	crossDirection = direction1_X.VCross(direction2_X);
	r1 = fabsf(crossDirection.VDot(direction1_Y)) + fabsf(crossDirection.VDot(direction1_Z));
	r2 = fabsf(crossDirection.VDot(direction2_Y)) + fabsf(crossDirection.VDot(direction2_Z));
	l = fabsf(crossDirection.VDot(distance));
	if ((r1 + r2) < l)
	{
		return false;
	}

	// 分離軸ベクトル：Cross(direction1_X, direction2_Y)
	crossDirection = direction1_X.VCross(direction2_Y);
	r1 = fabsf(crossDirection.VDot(direction1_Y)) + fabsf(crossDirection.VDot(direction1_Z));
	r2 = fabsf(crossDirection.VDot(direction2_X)) + fabsf(crossDirection.VDot(direction2_Z));
	l = fabsf(crossDirection.VDot(distance));
	if ((r1 + r2) < l)
	{
		return false;
	}

	// 分離軸ベクトル：Cross(direction1_X, direction2_Z)
	crossDirection = direction1_X.VCross(direction2_Z);
	r1 = fabsf(crossDirection.VDot(direction1_Y)) + fabsf(crossDirection.VDot(direction1_Z));
	r2 = fabsf(crossDirection.VDot(direction2_X)) + fabsf(crossDirection.VDot(direction2_Y));
	l = fabsf(crossDirection.VDot(distance));
	if ((r1 + r2) < l)
	{
		return false;
	}

	// 分離軸ベクトル：Cross(direction1_Y, direction2_X)
	crossDirection = direction1_Y.VCross(direction2_X);
	r1 = fabsf(crossDirection.VDot(direction1_X)) + fabsf(crossDirection.VDot(direction1_Z));
	r2 = fabsf(crossDirection.VDot(direction2_Y)) + fabsf(crossDirection.VDot(direction2_Z));
	l = fabsf(crossDirection.VDot(distance));
	if ((r1 + r2) < l)
	{
		return false;
	}

	// 分離軸ベクトル：Cross(direction1_Y, direction2_Y)
	crossDirection = direction1_Y.VCross(direction2_Y);
	r1 = fabsf(crossDirection.VDot(direction1_X)) + fabsf(crossDirection.VDot(direction1_Z));
	r2 = fabsf(crossDirection.VDot(direction2_X)) + fabsf(crossDirection.VDot(direction2_Z));
	l = fabsf(crossDirection.VDot(distance));
	if ((r1 + r2) < l)
	{
		return false;
	}

	// 分離軸ベクトル：Cross(direction1_Y, direction2_Z)
	crossDirection = direction1_Y.VCross(direction2_Z);
	r1 = fabsf(crossDirection.VDot(direction1_X)) + fabsf(crossDirection.VDot(direction1_Z));
	r2 = fabsf(crossDirection.VDot(direction2_X)) + fabsf(crossDirection.VDot(direction2_Y));
	l = fabsf(crossDirection.VDot(distance));
	if ((r1 + r2) < l)
	{
		return false;
	}

	// 分離軸ベクトル：Cross(direction1_Z, direction2_X)
	crossDirection = direction1_Z.VCross(direction2_X);
	r1 = fabsf(crossDirection.VDot(direction1_X)) + fabsf(crossDirection.VDot(direction1_Y));
	r2 = fabsf(crossDirection.VDot(direction2_Y)) + fabsf(crossDirection.VDot(direction2_Z));
	l = fabsf(crossDirection.VDot(distance));
	if ((r1 + r2) < l)
	{
		return false;
	}

	// 分離軸ベクトル：Cross(direction1_Z, direction2_Y)
	crossDirection = direction1_Z.VCross(direction2_Y);
	r1 = fabsf(crossDirection.VDot(direction1_X)) + fabsf(crossDirection.VDot(direction1_Y));
	r2 = fabsf(crossDirection.VDot(direction2_X)) + fabsf(crossDirection.VDot(direction2_Z));
	l = fabsf(crossDirection.VDot(distance));
	if ((r1 + r2) < l)
	{
		return false;
	}

	// 分離軸ベクトル：Cross(direction1_Z, direction2_Z)
	crossDirection = direction1_Z.VCross(direction2_Z);
	r1 = fabsf(crossDirection.VDot(direction1_X)) + fabsf(crossDirection.VDot(direction1_Y));
	r2 = fabsf(crossDirection.VDot(direction2_X)) + fabsf(crossDirection.VDot(direction2_Y));
	l = fabsf(crossDirection.VDot(distance));
	if ((r1 + r2) < l)
	{
		return false;
	}

	return true;
}

bool IsPredictCollisionBall(const Vector3& pos1, const Vector3& speed1, const float& r1, const Vector3& pos2, const Vector3& speed2, const float& r2)
{
	distance = pos1 - pos2;

	return distance.Length() < ((r1 + speed1.Length()) + (r2 + speed2.Length()));
}
} // namespace Collision
