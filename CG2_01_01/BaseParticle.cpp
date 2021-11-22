#include "BaseParticle.h"
#include "../DirectX/DirectXCommon/DirectXCommon.h"

std::vector<std::string> BaseParticle::texFilepath;

BaseParticle::BaseParticle() :
	manager(nullptr)
{
	lifeTime = 0;
	pos = Vector3();
	speed = Vector3();
	accel = Vector3();
	startScale = 0.0f;
	endScale = 0.0f;
	startColor = DirectX::XMFLOAT4();
	endColor = DirectX::XMFLOAT4();
}

BaseParticle::~BaseParticle()
{
	Finalize();
}

size_t BaseParticle::GetParticleNum()
{
	size_t num = 0;
	auto& particles = manager->particles;

	for (auto i = particles.begin(); i != particles.end(); i++)
	{
		num++;
	}

	return num;
}

void BaseParticle::CreateManager(std::string texFilepath)
{
	if (manager != nullptr)
	{
		return;
	}

	static bool isInit = false;
	isInit = false;

	for (size_t i = 0; i < this->texFilepath.size(); i++)
	{
		if (texFilepath == this->texFilepath[i])
		{
			isInit = true;
			manager = manager->Create();
			manager->textureIndex = i;
			break;
		}
	}

	if (isInit == false)
	{
		wchar_t* wcs = new wchar_t[texFilepath.length() + 1];
		size_t ret;
		auto a = mbstowcs_s(&ret, wcs, texFilepath.length() + 1, texFilepath.c_str(), _TRUNCATE);
		manager = manager->Initialize(wcs);
		delete[] wcs;

		this->texFilepath.push_back(texFilepath);
	}
}

void BaseParticle::Draw() const
{
	manager->Draw(DirectXCommon::cmdList.Get());
}

void BaseParticle::Finalize()
{
	if (manager != nullptr)
	{
		delete manager;
		manager = nullptr;
	}
}
