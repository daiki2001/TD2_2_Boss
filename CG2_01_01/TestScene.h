#pragma once
#include "AbstractScene.h"

#include "Player.h"
#include "Stage.h"

class TestScene
	:public AbstractScene
{
public:

	TestScene(IoChangedListener *impl);
	virtual ~TestScene() = default;

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() const override;

private:
	Player player;
	Stage stage;
};

