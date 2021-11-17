#include "ModelManager.h"

map<ModelManager::ModelName, Model *> ModelManager::model;

ModelManager *ModelManager::GetIns()
{
	static ModelManager instans;
	return &instans;
}

void ModelManager::Initialize()
{
	LoadModel(Player, "Player");
	LoadModel(TestEnemy, "Enemy");
	//LoadModel(BossCore, "Boss");
	LoadModel(Stage, "Stage");
	//LoadModel(TestDunut, "donut");
}

void ModelManager::LoadModel(const ModelName modelName, std::string fileName)
{
	model[modelName] = new Model();
	model[modelName]->InitializeDiscriptorHeap();
	model[modelName]->CreateFromObj(fileName);

}
