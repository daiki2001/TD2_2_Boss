#pragma once
#include "Particle/ParticleManager.h"
#include "Vector3.h"
#include <DirectXMath.h>
#include <vector>
#include <string>

class BaseParticle
{
public:
	BaseParticle();
	~BaseParticle();

public: // 仮想関数
	// 初期化処理
	virtual void Initialize();
	// 更新処理
	virtual void Update();

private: // 静的メンバ変数
	static std::vector<std::string> texFilepath; //テクスチャの名前群

public: // メンバ関数
	// 描画処理
	void Draw() const;
	// 終了処理
	void Finalize();
public:
	// ParticleManagerの生成処理
	void CreateManager(std::string texFilepath);

public: // メンバ変数
	int lifeTime; //生存時間

	Vector3 pos;   //座標
	Vector3 speed; //速度
	Vector3 accel; //加速度
	Vector3 offset;

	float startScale; //開始時スケール
	float endScale;   //終了時スケール

	DirectX::XMFLOAT4 startColor; //開始時の色
	DirectX::XMFLOAT4 endColor;   //終了時の色
public:
	ParticleManager* manager;

};
