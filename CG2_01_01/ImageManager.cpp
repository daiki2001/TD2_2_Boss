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
}
