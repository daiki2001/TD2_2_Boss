#include "UI.h"
#include "ImageManager.h"
#include "DirectXCommon.h"
#include "GameObjCommon.h"

UI::UI()
{
	const int w = 48;
	const int h = 64;
	const int l = 10;

	timer = Sprite::Create(ImageManager::Timer, {0.25,0.3 });
	timer->SetScale(0.4f);
	timer->SetPosition({ 880, 0 });
	for(int b = 0; b < 3;b++){
		for (int i = 0; i < 10; i++) {

			Num[i][b] = Sprite::Create(ImageManager::Num);
			int number_index_y = i / l;
			int number_index_x = i % l;
			Num[i][b]->SetTextureRect(
				{ static_cast<float>(number_index_x) * w, static_cast<float>(number_index_y) * h },
				{ static_cast<float>(w), static_cast<float>(h) });
			Num[i][b]->SetSize({ w,h });
			Num[i][b]->SetScale(1.1f);
			Num[i][b]->SetAnchorPoint({0,0});


			Num[i][b]->SetPosition({ 1100 + (float)b * 50 ,7 });
		}
	}
	testCounter = 0;

	for (int i = 0; i < sizeof(enemysPos)/sizeof(enemysPos[0]); i++) {
		enemysPos[i] = Sprite::Create(ImageManager::miniEnemy);
		enemysPos[i]->SetScale(0.02f);
	}

	miniMap = Sprite::Create(ImageManager::miniMap, { 0,0 });
	miniMap->SetScale(0.1f);
	miniMap->SetPosition({-10,-10});
	playerPos = Sprite::Create(ImageManager::miniPlayer);
	playerPos->SetScale(0.05f);
	BossPos = Sprite::Create(ImageManager::miniBoss);
	BossPos->SetScale(0.035f);
}
void UI::Initialize(Player *player)
{
	playerData = player;
}
void UI::Update()
{
	testCounter++;
	Time = (int)(testCounter / 60);

	Timer.clear();
	// ”Žš‚ðŒ…‚²‚Æ‚É•ª‰ð‚µ‚Ä‹tŒü‚«‚É•Û‘¶
	for (int tmp = Time; tmp > 0;) {
		Timer.push_back(tmp % 10);
		tmp /= 10;
	}
	while (Timer.size() < 3) {
		Timer.push_back(0);
	}

	// •À‚×‚È‚¨‚·
	std::reverse(Timer.begin(), Timer.end());

	for (int i = 0; i < GameObjCommon::enemys.size(); i++) {
		if (GameObjCommon::enemys[i]->Tag == "Boss") {
			BossPos->SetPosition({ (GameObjCommon::enemys[i]->pos.x + 3990.0f) * 0.0195f + 13,-(GameObjCommon::enemys[i]->pos.z - 790.0f) * 0.035f + 13 });
		}
		else{
			enemysPos[i]->SetPosition({ (GameObjCommon::enemys[i]->pos.x + 3990.0f) * 0.0195f + 13,-(GameObjCommon::enemys[i]->pos.z - 790.0f) * 0.035f + 13 });
		}
	}
	playerPos->SetRotation(playerData->GetAngle() + 90);
	playerPos->SetPosition({ (playerData->pos.x + 3990.0f)*0.0195f + 13,-(playerData->pos.z - 790.0f)*0.035f + 13 });
}
void UI::Draw() const
{
	Sprite::PreDraw(DirectXCommon::cmdList.Get());
	timer->Draw();
	for (int i = 0; i < 3; i++) {
		Num[Timer[i]][i]->Draw();
	}

	miniMap->Draw();
	for (int i = 0; i < GameObjCommon::enemys.size(); i++) {
		if (GameObjCommon::enemys[i]->Tag == "Boss") {
			BossPos->Draw();
		}
		else{
			enemysPos[i]->Draw();
		}
	}


	playerPos->Draw();
	Sprite::PostDraw();
}