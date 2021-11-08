#pragma once
#include "Object2d.h"
#include "Object3d.h"

class ImageManager final
{
private:
	ImageManager();
	~ImageManager();

public:
	ImageManager(const ImageManager &obj) = delete;
	ImageManager &operator=(const ImageManager &obj) = delete;
	static ImageManager *GetIns();

	//‰æ‘œ“Ç‚İ‚İ
	void LoadImagALL(ID3D12Device *dev);


	enum Tex2D {
		BackGround

	};
};

