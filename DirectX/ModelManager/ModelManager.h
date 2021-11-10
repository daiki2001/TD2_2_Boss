#pragma once
#include <map>
#include <string>
#include "Model.h"
class ModelManager final
{
private:
	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(const ModelManager &r) = default;
	ModelManager &operator= (const ModelManager &r) = default;

public:
	enum ModelName {
		Player,
		Triangle,
		Stage,
		TestDunut,
	};

public:
	static ModelManager *GetIns();		//取得用

	void Initialize();
	void LoadModel(const ModelName modelName, std::string fileName);
	Model *GetModel(ModelName modelName) { return model[modelName]; }

private:
	static map<ModelName, Model *> model;	//モデル格納マップ
};

