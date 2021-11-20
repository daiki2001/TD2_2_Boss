#pragma once
#include <vector>
#include "GameObjCommon.h"
#include "Player.h"

class LoadStage
{
public:
	LoadStage() {}
	~LoadStage() {}

public: //サブクラス
	// 敵の種類
	enum EnemyType
	{
		TEST,
		RUSH,
		BOSS
	};
	// 敵の生成に必要な情報
	struct Enemy
	{
		float hp = 0.0f;
		float N = 0.0f;
		float e = 0.0f;
	};

public: // 静的メンバ関数
	// 初期化処理
	static void Init();

	/// <summary>
	/// CSVファイルの読み込み
	/// </summary>
	/// <param name="buffer"> 読み込んだCSVファイルの保存先 </param>
	/// <param name="maxCount"> 格納する数値の最大数 </param>
	/// <param name="FilePath"> CSVファイルのパス </param>
	/// <param name="LoadStopNumber"> ここに指定された値がヒットすると読み込みを終了する </param>
	/// <returns> 成否 </returns>
	static bool LoadCSV(int* buffer, const size_t& maxCount, const char* FilePath, int LoadStopNumber = -1);

	// ステージ上に配置の読み込み
	static bool LoadStageEnemy(const char* FilePath, std::vector<GameObjCommon*>& enemy, Player* player);
private:
	// 敵の生成
	static void CreateEnemy(std::vector<GameObjCommon*>& enemy, const size_t& enemyIndex, const int& enemyType, Player* player);

private: // メンバ変数
	static Enemy testEnemy; //TestEnemyの個体値情報
	static Enemy rushEnemy; //RushEnemyの個体値情報
	static Enemy boss;      //Bossの個体値情報

};
