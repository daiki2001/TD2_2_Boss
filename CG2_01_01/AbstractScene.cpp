#include "AbstractScene.h"

AbstractScene::AbstractScene(IoChangedListener *impl)
	:implSceneChanged(impl)
{
}
