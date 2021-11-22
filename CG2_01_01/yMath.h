#pragma once
#define _USE_MATH_DIFINES
#include <cmath>
#include "Vector3.h"
#include <DirectXMath.h>

namespace yMath {
	//0~360�x�͈̔͂Ɏ��߂�
	float ChangeDegree360(float deg) {
		deg = fmodf(deg, 360.0f);
		if (0.0f > deg) {
			deg += 360.0f;
		}
		
		return deg;
	}
	
	//���W�A�����x
	float RadToDeg(float rad) {
		return rad * (180.0f / XM_PI);
	}
	//�x�����W�A��
	float DegToRad(float deg) {
		return deg * (XM_PI / 180.0f);
	}

	Vector3 sprinePosition(const std::vector<Vector3> &points, size_t &startIndex, float time, bool isLoop)
	{
		//��Ԃ��ׂ��_�̐�
		if (!isLoop) {
			if (startIndex >= points.size() - 1) {
				startIndex = points.size() - 1;
				return points[points.size() - 1];
			}
		}

		//�I�_�ɓ��B������X�^�[�g�����Z�b�g
		if (startIndex == points.size()) {
			startIndex = 0;
		}
		//P0�`P3�̐���_���擾����
		Vector3 p0;
		Vector3 p1;
		Vector3 p2;
		Vector3 p3;
		if (startIndex == 0 && isLoop) {
			p0 = points[points.size() - 1];
		}
		else if (startIndex == 0 && !isLoop) {
			p0 = points[0];
		}
		else {
			p0 = points[startIndex - 1];
		}

		p1 = points[startIndex];
		//���̃C���f�b�N�X+1�Ƀf�[�^���Ȃ����0�ɖ߂�
		if (startIndex >= points.size() - 1) {
			p2 = points[0];
		}
		else {
			p2 = points[startIndex + 1];
		}
		//���̃C���f�b�N�X+1�Ƀf�[�^���Ȃ����0�ɖ߂�
		if (startIndex >= points.size() - 2) {
			p3 = points[startIndex - points.size() - 2];
		}
		else {
			p3 = points[startIndex + 2];
		}
		//Catmull-Rom �̎��ɂ����
		Vector3 a = (-p0 + p2) * time;
		Vector3 b = (2 * p0 - 5 * p1 + 4 * p2 - p3) * time * time;
		Vector3 c = (-p0 + 3 * p1 - 3 * p2 + p3) * time * time * time;
		Vector3 position = 0.5f * (2 * p1 + a + b + c);

		return position;
	}
}

