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
	int counter;
	void AttackSelect();
	bool ChackRange( float max, float min = 0.0f);
	
	//攻撃した後の待機時間、攻撃終了ごとに設定する。
	static int stayTimer;

	Object3d *frame;
	Vector3 frameRotate;
	Vector3 spinFrame();
	//攻撃中かどうか
	bool isAttack;
	//テスト用アタックステート
	enum ATTACKSTATE {
		Stay,
		Short,
		Middle,
		Long,
	};
	ATTACKSTATE atackState;		//次にとる攻撃
	ATTACKSTATE oldAtackState;	//直前の攻撃
};

