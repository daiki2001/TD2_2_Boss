#pragma once
#include "Sprite.h"
#include "Player.h"
#include "GameObjCommon.h"
class UI
{
public:
	UI();

	void Initialize(Player *player);
	void Update();
	void Draw() const;
private:
	void MiniMap();

	Player *playerData;
	GameObjCommon *boss;

	Sprite *miniMap;
	Sprite *playerPos;
	Sprite *bossPoss;
	Sprite *timer;

	Sprite *Num[10][3];
	int testCounter;
	int Time;
	vector<int> Timer;
};

