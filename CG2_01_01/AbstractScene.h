#pragma once

#include "IoChangedListener.h"

class IoChangedListener;

class AbstractScene
{
protected:
	IoChangedListener *implSceneChanged;
public:
	AbstractScene(IoChangedListener *impl);
	virtual ~AbstractScene() = default;
	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
	virtual void Update() = 0;
	virtual void Draw() const = 0;
};

