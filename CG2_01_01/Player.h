#pragma once

#include "Object3d.h"
#include "Vector3.h"

class Player
{
public:
	Player();

	void Initialize();
	void Update();
	void Draw() const;

public:


private:
	Vector3 pos;
	Vector3 move;
	Object3d *object;

};