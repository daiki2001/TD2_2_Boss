#include "ImageManager.h"

ImageManager::ImageManager()
{
}

ImageManager::~ImageManager()
{
}

ImageManager *ImageManager::GetIns()
{
	static ImageManager ins;
	return &ins;
}

void ImageManager::LoadImagALL(ID3D12Device *dev) {
	Sprite::LoadTexture(Test, L"Resources/test.png", dev);
	Sprite::LoadTexture(Num, L"Resources/numbersFont.png", dev);
	Sprite::LoadTexture(Logo, L"Resources/Images/titleLogo.png", dev);
	Sprite::LoadTexture(Timer, L"Resources/Images/timer.png", dev);
	
	
	Sprite::LoadTexture(miniMap, L"Resources/Images/stageMap.png", dev);
	Sprite::LoadTexture(miniBoss, L"Resources/Images/bossIcon1.png", dev);
	Sprite::LoadTexture(miniPlayer, L"Resources/Images/playerIcon1.png", dev);
}
