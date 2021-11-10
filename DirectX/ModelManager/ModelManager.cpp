#include "ModelManager.h"

map<ModelManager::ModelName, Model *> ModelManager::model;

ModelManager *ModelManager::GetIns()
{
	static ModelManager instans;
	return &instans;
}

void ModelManager::Initialize()
{
	LoadModel(Player, "player");
	LoadModel(Triangle, "triangle_mat");
	LoadModel(Stage, "Stage");
	LoadModel(TestDunut, "donut");
}

void ModelManager::LoadModel(const ModelName modelName, std::string fileName)
{
	model[modelName] = new Model();
	model[modelName]->InitializeDiscriptorHeap();
	model[modelName]->CreateFromObj(fileName);

}
