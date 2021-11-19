#include "Collision.h"
#include <DirectXMath.h>

namespace Collision
{
static Vector3 distance{}; //��_�Ԃ̋���(Collision namespace������)

bool IsBallToBallCollision(const Vector3& pos1, const float& r1, const Vector3& pos2, const float& r2)
{
	// ���S�_�Ԃ̋���
	distance = pos1 - pos2;

	return distance.Length() < (r1 + r2);
}

bool IsBoxToBoxCollision(const Vector3& pos1, const Vector3& rotation1, const Vector3& scale1, const Vector3& pos2, const Vector3& rotation2, const Vector3& scale2)
{
	using namespace DirectX;

	// ��]�s��
	static XMMATRIX matRot1{}, matRot2{};
	// ���S����ʂ܂ł̒���
	static Vector3 length1{}, length2{};
	// �������x�N�g��
	static Vector3 direction1_XN{}, direction1_YN{}, direction1_ZN{}, direction2_XN{}, direction2_YN{}, direction2_ZN{};
	// �����x�N�g��
	static Vector3 direction1_X{}, direction1_Y{}, direction1_Z{}, direction2_X{}, direction2_Y{}, direction2_Z{};

	/*OBB�̓����蔻��̉�����*/

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

	/*OBB�̓����蔻��*/

	distance = pos1 - pos2;

	static float r1{}, r2{}; //�����e����
	static float l{}; //���ԓ_�Ԃ̋���

	// �������x�N�g���Fdirection1_XN
	r1 = direction1_X.Length();
	r2 = fabsf(direction1_XN.VDot(direction2_X)) + fabsf(direction1_XN.VDot(direction2_Y)) + fabsf(direction1_XN.VDot(direction2_Z));
	l = fabsf(direction1_XN.VDot(distance));
	if ((r1 + r2) < l)
	{
		return false;
	}

	// �������x�N�g���Fdirection1_YN
	r1 = direction1_Y.Length();
	r2 = fabsf(direction1_YN.VDot(direction2_X)) + fabsf(direction1_YN.VDot(direction2_Y)) + fabsf(direction1_YN.VDot(direction2_Z));
	l = fabsf(direction1_YN.VDot(distance));
	if ((r1 + r2) < l)
	{
		return false;
	}

	// �������x�N�g���Fdirection1_ZN
	r1 = direction1_Z.Length();
	r2 = fabsf(direction1_ZN.VDot(direction2_X)) + fabsf(direction1_ZN.VDot(direction2_Y)) + fabsf(direction1_ZN.VDot(direction2_Z));
	l = fabsf(direction1_ZN.VDot(distance));
	if ((r1 + r2) < l)
	{
		return false;
	}

	// �������x�N�g���Fdirection2_XN
	r1 = direction2_X.Length();
	r2 = fabsf(direction2_XN.VDot(direction1_X)) + fabsf(direction2_XN.VDot(direction1_Y)) + fabsf(direction2_XN.VDot(direction1_Z));
	l = fabsf(direction2_XN.VDot(distance));
	if ((r1 + r2) < l)
	{
		return false;
	}

	// �������x�N�g���Fdirection2_YN
	r1 = direction2_Y.Length();
	r2 = fabsf(direction2_YN.VDot(direction1_X)) + fabsf(direction2_YN.VDot(direction1_Y)) + fabsf(direction2_YN.VDot(direction1_Z));
	l = fabsf(direction2_YN.VDot(distance));
	if ((r1 + r2) < l)
	{
		return false;
	}

	// �������x�N�g���Fdirection2_ZN
	r1 = direction2_Z.Length();
	r2 = fabsf(direction2_ZN.VDot(direction1_X)) + fabsf(direction2_ZN.VDot(direction1_Y)) + fabsf(direction2_ZN.VDot(direction1_Z));
	l = fabsf(direction2_ZN.VDot(distance));
	if ((r1 + r2) < l)
	{
		return false;
	}

	static Vector3 crossDirection{}; //�O�ϕ�����

	// �������x�N�g���FCross(direction1_X, direction2_X)
	crossDirection = direction1_X.VCross(direction2_X);
	r1 = fabsf(crossDirection.VDot(direction1_Y)) + fabsf(crossDirection.VDot(direction1_Z));
	r2 = fabsf(crossDirection.VDot(direction2_Y)) + fabsf(crossDirection.VDot(direction2_Z));
	l = fabsf(crossDirection.VDot(distance));
	if ((r1 + r2) < l)
	{
		return false;
	}

	// �������x�N�g���FCross(direction1_X, direction2_Y)
	crossDirection = direction1_X.VCross(direction2_Y);
	r1 = fabsf(crossDirection.VDot(direction1_Y)) + fabsf(crossDirection.VDot(direction1_Z));
	r2 = fabsf(crossDirection.VDot(direction2_X)) + fabsf(crossDirection.VDot(direction2_Z));
	l = fabsf(crossDirection.VDot(distance));
	if ((r1 + r2) < l)
	{
		return false;
	}

	// �������x�N�g���FCross(direction1_X, direction2_Z)
	crossDirection = direction1_X.VCross(direction2_Z);
	r1 = fabsf(crossDirection.VDot(direction1_Y)) + fabsf(crossDirection.VDot(direction1_Z));
	r2 = fabsf(crossDirection.VDot(direction2_X)) + fabsf(crossDirection.VDot(direction2_Y));
	l = fabsf(crossDirection.VDot(distance));
	if ((r1 + r2) < l)
	{
		return false;
	}

	// �������x�N�g���FCross(direction1_Y, direction2_X)
	crossDirection = direction1_Y.VCross(direction2_X);
	r1 = fabsf(crossDirection.VDot(direction1_X)) + fabsf(crossDirection.VDot(direction1_Z));
	r2 = fabsf(crossDirection.VDot(direction2_Y)) + fabsf(crossDirection.VDot(direction2_Z));
	l = fabsf(crossDirection.VDot(distance));
	if ((r1 + r2) < l)
	{
		return false;
	}

	// �������x�N�g���FCross(direction1_Y, direction2_Y)
	crossDirection = direction1_Y.VCross(direction2_Y);
	r1 = fabsf(crossDirection.VDot(direction1_X)) + fabsf(crossDirection.VDot(direction1_Z));
	r2 = fabsf(crossDirection.VDot(direction2_X)) + fabsf(crossDirection.VDot(direction2_Z));
	l = fabsf(crossDirection.VDot(distance));
	if ((r1 + r2) < l)
	{
		return false;
	}

	// �������x�N�g���FCross(direction1_Y, direction2_Z)
	crossDirection = direction1_Y.VCross(direction2_Z);
	r1 = fabsf(crossDirection.VDot(direction1_X)) + fabsf(crossDirection.VDot(direction1_Z));
	r2 = fabsf(crossDirection.VDot(direction2_X)) + fabsf(crossDirection.VDot(direction2_Y));
	l = fabsf(crossDirection.VDot(distance));
	if ((r1 + r2) < l)
	{
		return false;
	}

	// �������x�N�g���FCross(direction1_Z, direction2_X)
	crossDirection = direction1_Z.VCross(direction2_X);
	r1 = fabsf(crossDirection.VDot(direction1_X)) + fabsf(crossDirection.VDot(direction1_Y));
	r2 = fabsf(crossDirection.VDot(direction2_Y)) + fabsf(crossDirection.VDot(direction2_Z));
	l = fabsf(crossDirection.VDot(distance));
	if ((r1 + r2) < l)
	{
		return false;
	}

	// �������x�N�g���FCross(direction1_Z, direction2_Y)
	crossDirection = direction1_Z.VCross(direction2_Y);
	r1 = fabsf(crossDirection.VDot(direction1_X)) + fabsf(crossDirection.VDot(direction1_Y));
	r2 = fabsf(crossDirection.VDot(direction2_X)) + fabsf(crossDirection.VDot(direction2_Z));
	l = fabsf(crossDirection.VDot(distance));
	if ((r1 + r2) < l)
	{
		return false;
	}

	// �������x�N�g���FCross(direction1_Z, direction2_Z)
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

bool sphereSwept(const Vector3& pos1, const Vector3& speed1, const float& r1, const Vector3& pos2, const Vector3& speed2, const float& r2,
	float &outSecond,	//�i�s�x�i�[�p�|�C���^�[
	Vector3 &CollisionPos,	//�Փˈʒu(�ړ_)
	Vector3 *CollisionPosA,	//��ڂ��Փ˂������̈ʒu
	Vector3 *CollisionPosB	//��ڂ��Փ˂������̈ʒu
)
{
	Vector3 Cstart = pos2 - pos1;				//�ړ��O��ab�̋���
	Vector3 Aend = pos1 + speed1;		//�ړ����a�̈ʒu
	Vector3 Bend = pos2 + speed2;		//�ړ����b�̈ʒu
	Vector3 Cend = Bend - Aend;					//�I������ab�̋���
	Vector3 D = Cend - Cstart;					//�I������ab�̋���
	float RadiusAB = r1 + r2;					//���a�̍��v
	float RadiusABpow = RadiusAB * RadiusAB;				//���a�̍��v�̓��
	float P = D.LengthSq();	//D�̒���

	//���s�ړ�
	if (P == 0) {
		if (Vector3(pos2 - pos1).LengthSq() > RadiusABpow) {
			return false;
		}
		outSecond = 0.0f;	//�i�s�x0����X�^�[�g
		if (CollisionPosA != 0) {
			*CollisionPosA = pos1;
		}
		if (CollisionPosB != 0) {
			*CollisionPosB = pos2;
		}
		//���S�ʒu�������Ȃ̂Œ��S�ŏՓ˂������Ƃɂ���
		if (pos2 == pos1) {
			CollisionPos = pos1;
			return true;
		}
		//B�̕����Ɍ�������A�̔��a���Փ˒n�_�Ƃ���
		CollisionPos = pos1 + Cstart * r1 / RadiusAB/* Vector2<float>{(a.r /RadiusAB)*Cstart.x, (a.r / RadiusAB) *Cstart.y}*/;
		return true;
	}
	//�Փˌ��m
	//�ŏ��̎��_�ŏՓ˂��Ă��邩���Ȃ���
	if (Vector3(pos2 - pos1).LengthSq() <= RadiusABpow) {
		outSecond = 0.0f;
		CollisionPos = pos1 + Cstart * r1 / RadiusAB;
		if (CollisionPosA != 0) {
			*CollisionPosA = pos1;
		}
		if (CollisionPosB != 0) {
			*CollisionPosB = pos2;
		}
		return true;
	}
	float Q = Cstart.VDot(D);
	float R = Cstart.LengthSq();

	float judge = Q * Q - P * (R - RadiusAB * RadiusAB);
	if (judge < 0) {
		return false;
	}

	//�Փˎ��Ԍv�Z
	float judge_rt = sqrtf(judge);
	float t_plus = (-Q + judge_rt) / P;
	float t_minus = (-Q - judge_rt) / P;
	if (t_minus > t_plus) {	//plus�̕�����������Γ���ւ�
		float tmp = t_minus;
		t_minus = t_plus;
		t_plus = tmp;
	}

	//���ԊO�̏Փ˂������ꍇ�𖳌��ɂ���
	if (t_minus < 0.0f || t_minus > 1.0f) {
		return false;
	}

	//�Փˈʒu�v�Z
	outSecond = t_minus;
	Vector3 actA = pos1 + speed1 *  t_minus;
	Vector3 actB = pos2 + speed2 *  t_minus;
	CollisionPos = actA + (actB - actA) * r1 / RadiusAB;

	if (CollisionPosA != 0) {
		*CollisionPosA = actA;
	}
	if (CollisionPosB != 0) {
		*CollisionPosB = actB;
	}

	return true;
}

bool IsPredictCollisionBall(const Vector3& pos1, const Vector3& speed1, const float& r1, const Vector3& pos2, const Vector3& speed2, const float& r2)
{
	// ���S�_�Ԃ̋���
	distance = pos1 - pos2;

	return distance.Length() < ((r1 + speed1.Length()) + (r2 + speed2.Length()));
}
} // namespace Collision
