#include "ModelManager.h"

map<ModelManager::ModelName, Model *> ModelManager::model;

ModelManager *ModelManager::GetIns()
{
	static ModelManager instans;
	return &instans;
}

void ModelManager::Initialize()
{
	LoadModel(PlayerFrame, "playerFrame");
	LoadModel(PlayerCore, "playerCore");
	LoadModel(Reticle, "Reticle");

	LoadModel(TestEnemy, "Enemy");
	LoadModel(RushEnemy, "Enemy");
	
	LoadModel(BossCore, "bossCore");
	LoadModel(BossFrame, "bossFrame");
	LoadModel(BossBomb, "bomb");
	
	
	LoadModel(Stage, "Stage");
	LoadModel(StageWall, "stageWall");
	//LoadModel(TestDunut, "donut");
}

void ModelManager::LoadModel(const ModelName modelName, std::string fileName)
{
	model[modelName] = new Model();
	model[modelName]->InitializeDiscriptorHeap();
	model[modelName]->CreateFromObj(fileName);

}
