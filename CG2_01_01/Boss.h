#pragma once
#include "BaseEnemy.h"
class Boss :
	public BaseEnemy
{
public:
	Boss(Player *player, Vector3 startPos, float hp, float N, float e , vector<GameObjCommon *> &enemys);
	void Initialize()override;
	void Update()override;
	void Reflection()override;
	void Draw() const override;

private:
	Vector3 playerVec;	//プレイヤーへの向きベクトル

	void Missile();
	void AttackSelect();				//攻撃方法選択
	bool ChackRange( float max, float min = 0.0f);	//範囲内チェック
	
	int stayTimer;
	Vector3 SpinFrame();				//待機モーション
	int standEaseTimer;		//攻撃前モーションイージング用
	int standMotionTimer;	//攻撃前モーションタイマー
	bool StandbyMotion();			//攻撃前待機モーション
	void Attack();			//攻撃時待機モーション

	//攻撃した後の待機時間、攻撃終了ごとに設定する。

	Object3d *frame;
	Vector3 frameRotate;
	//攻撃中かどうか
	bool isAttack;
	//テスト用アタックステート
	enum ATTACKTYPE {
		Stay,
		Tackle,
		Tackle3,
		Bomb,
		Long,
	};
	ATTACKTYPE attackType;		//今の攻撃
	ATTACKTYPE nextAttackType;	//次の攻撃
	ATTACKTYPE oldAtackType;	//直前の攻撃
	int BombCounter;			//ボム後にタックルするためのカウンタ
	int TackleCounter;			//三連タックル用カウンタ
	Vector3 startPos;

	vector<GameObjCommon *> *enemys;
};

