#pragma once
#include "Vector3.h"

class Input final
{
private: // デフォルトコンストラクタ・デストラクタのprivate化
	Input() {}
	~Input() {}
public: // コンストラクタ関係の設定
	// コピーコンストラクタの削除
	Input(const Input&) = delete;
	// 代入演算子の削除
	const Input& operator=(const Input&) = delete;

public: // 静的メンバ関数
	// 初期化
	static void Init();
	// 更新
	static void Update();

	/*各入力判定*/
	// 終了
	static bool Exit();
	// 移動
	static Vector3 Move();
	// 攻撃準備
	static bool AttackPrepare();
	// 攻撃
	static bool Attack();
	// ロックオン
	static bool LockOn();
	// テスト用(PC専用)
	static bool Test();
};
