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

void ImageManager::LoadImagALL(ID3D12Device *dev)
{
	//3d—p
	//2d—p
	Object2d::SpriteCommonLoadTexture(BackGround, L"Resources/background.png", dev);

	//Object2d::SpriteCommonLoadTexture()
}
