#pragma once
#define _USE_MATH_DIFINES
#include <cmath>
#include "Vector3.h"
#include <DirectXMath.h>

namespace yMath {
	//0~360度の範囲に収める
	float ChangeDegree360(float deg) {
		deg = fmodf(deg, 360.0f);
		if (0.0f > deg) {
			deg += 360.0f;
		}
		
		return deg;
	}
	
	//ラジアン→度
	float RadToDeg(float rad) {
		return rad * (180.0f / XM_PI);
	}
	//度→ラジアン
	float DegToRad(float deg) {
		return deg * (XM_PI / 180.0f);
	}
}

