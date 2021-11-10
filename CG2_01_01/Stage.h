#pragma once
#include "Object3d.h"

class Stage
{
public:
	Stage();
	void Initialize();
	void Update();
	void Draw() const;
private:

	Object3d *object;
};

