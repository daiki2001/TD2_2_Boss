#pragma once
#include "Vector3.h"

namespace Collision
{
// ���̓��m�̓����蔻��
bool IsBallToBallCollision(const Vector3& pos1, const float& r1, const Vector3& pos2, const float& r2);

// �����̓��m�̓����蔻��
bool IsBoxToBoxCollision(const Vector3& pos1, const Vector3& rotation1, const Vector3& scale1, const Vector3& pos2, const Vector3& rotation2, const Vector3& scale2);

// ���̃X�C�[�v
// �������Ă�����Փ˂������Ԃ�0~1�̊ԂŕԂ��A�������Ă��Ȃ�������-1��Ԃ�
bool sphereSwept(const Vector3& pos1, const Vector3& speed1, const float& r1, const Vector3& pos2, const Vector3& speed2, const float& r2,
	float &outSecond,	//�i�s�x�i�[�p�|�C���^�[
	Vector3 &CollisionPos,	//�Փˈʒu(�ړ_)
	Vector3 *CollisionPosA = 0,	//��ڂ��Փ˂������̈ʒu
	Vector3 *CollisionPosB = 0	//��ڂ��Փ˂������̈ʒu
);

// �Փ˗\��
// ���̏ꍇ�́AVector3(����, ����, ���s).Length() / 2������Ύg���܂�
bool IsPredictCollisionBall(const Vector3& pos1, const Vector3& speed1, const float& r1, const Vector3& pos2, const Vector3& speed2, const float& r2);
};
