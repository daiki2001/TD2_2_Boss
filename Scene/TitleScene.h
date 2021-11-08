#pragma once
#include "Object2d.h"
#include "Object3d.h"
#include "AbstractScene.h"
class TitleScene
	:public AbstractScene
{
public:
	TitleScene(IoChangedListener *impl);
	virtual ~TitleScene() = default;
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() const override;
private:
	Object2d::Sprite title;
	Object3d *obj1;
	Object3d *obj2;
};

