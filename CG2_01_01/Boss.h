#pragma once
#include "BaseEnemy.h"
class Boss :
	public BaseEnemy
{
public:
	Boss(Player *player, Vector3 startPos, float hp, float N, float e);

	void Initialize()override;
	void Update()override;
	void Reflection()override;
	void Draw() const override;

private:
	Object3d *frame;
};

