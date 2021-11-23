#pragma once
#include "Sprite.h"
#include "Object3d.h"
#include "AbstractScene.h"

#include "Player.h"

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
	Object3d *obj1;
	Object3d *obj2;
	Player player;
};

