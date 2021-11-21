#include "LoadStage.h"
#include <stdio.h>
#include "Boss.h"
#include "TestEnemy.h"
#include "RushEnemy.h"

LoadStage::Enemy LoadStage::testEnemy;
LoadStage::Enemy LoadStage::rushEnemy;
LoadStage::Enemy LoadStage::boss;

void LoadStage::Init()
{
	testEnemy = { 10.0f,  10.0f, 2.0f };
	rushEnemy = { 10.0f, 10.0f, 0.5f };
	boss = { 50.0f, 100.0f, 0.2f };
}

bool LoadStage::LoadCSV(int* buffer, const size_t& maxCount, const char* FilePath, int LoadStopNumber)
{
	for (size_t i = 0; i < maxCount; i++)
	{
		buffer[i] = 0;
	}

	FILE* fileHandle;
	errno_t err;
	char string[256] = { 0 };
	int index = 0;

	err = fopen_s(&fileHandle, FilePath, "r");
	if (err != 0)
	{
		return false;
	}

	bool isMinus = false;   //マイナスかどうか
	bool isDecimal = false; //小数かどうか

	while (fgets(string, 256, fileHandle) != nullptr)
	{
		bool end = false;

		for (int i = 0; string[i] != '\0'; i++)
		{
			if (string[i] == ',' || string[i] == '\n')
			{
				if (buffer[index] == LoadStopNumber)
				{
					// ヒットしたら、読み込みを強制的に終了する。
					end = true;
					break;
				}

				// 次の数字へ
				index++;
				isMinus = false;
				isDecimal = false;
			}
			else if (string[i] == '-')
			{
				isMinus = true;
			}
			else if (string[i] == '.')
			{
				isDecimal = true;
			}
			else if (string[i] >= '0' && string[i] <= '9')
			{
				if (isDecimal == false)
				{
					buffer[index] *= 10;

					if (isMinus == true)
					{
						buffer[index] -= string[i] - '0';
					}
					else
					{
						buffer[index] += string[i] - '0';
					}
				}
			}
		}
		if (end)
		{
			break;
		}
	}
	fclose(fileHandle);

	return true;
}

bool LoadStage::LoadStageEnemy(const char* FilePath, std::vector<GameObjCommon*>& enemy, Player* player)
{
	FILE* fileHandle;
	errno_t err;
	char string[256] = { 0 };
	static float buf = 0.0f;
	size_t enemyCount = 0;
	static int index = 0;

	err = fopen_s(&fileHandle, FilePath, "r");
	if (err != 0)
	{
		return false;
	}

	static bool isMinus = false;    //マイナスかどうか
	static bool isDecimal = false;  //小数かどうか
	static size_t decimalCount = 0; //小数点以下の桁数

	while (fgets(string, 256, fileHandle) != nullptr)
	{
		buf = 0.0f;
		index = 0;
		isMinus = false;
		isDecimal = false;
		decimalCount = 0;

		for (int i = 0; string[i] != '\0'; i++)
		{
			if (string[i] == ',' || string[i] == '\n')
			{
				if (index == 0)
				{
					CreateEnemy(enemy, enemyCount, (int)buf, player);
				}
				else if (index == 1)
				{
					enemy[enemyCount]->pos.x = buf;
				}
				else if (index == 2)
				{
					enemy[enemyCount]->pos.z = buf;
					break;
				}

				// 次の数字へ
				buf = 0.0f;
				index++;
				isMinus = false;
				isDecimal = false;
				decimalCount = 0;
			}
			else if (string[i] == '-')
			{
				isMinus = true;
			}
			else if (string[i] == '.')
			{
				isDecimal = true;
			}
			else if (string[i] >= '0' && string[i] <= '9')
			{
				if (isDecimal == true)
				{
					static float num = 0.0f;
					num = (float)(string[i] - '0');

					decimalCount++;
					for (size_t i = 0; i < decimalCount; i++)
					{
						if (num == 0.0f)
						{
							break;
						}
						num /= 10.0f;
					}

					if (isMinus == true)
					{
						buf -= num;
					}
					else
					{
						buf += num;
					}
				}
				else
				{
					buf *= 10.0f;

					if (isMinus == true)
					{
						buf -= (float)(string[i] - '0');
					}
					else
					{
						buf += (float)(string[i] - '0');
					}
				}
			}
		}

		enemyCount++;
	}
	fclose(fileHandle);

	return true;
}

void LoadStage::CreateEnemy(std::vector<GameObjCommon*>& enemy, const size_t& enemyIndex, const int& enemyType, Player* player)
{
	if (enemyIndex < enemy.size())
	{
		delete enemy[enemyIndex];
		enemy[enemyIndex] = nullptr;

		switch (enemyType)
		{
		case TEST:
			enemy[enemyIndex] =
				new TestEnemy(player, Vector3(), testEnemy.hp, testEnemy.N, testEnemy.e);
			break;
		case RUSH:
			enemy[enemyIndex] =
				new RushEnemy(player, Vector3(), rushEnemy.hp, rushEnemy.N, rushEnemy.e);
			break;
		case BOSS:
			enemy[enemyIndex] =
				new Boss(player, Vector3(), boss.hp, boss.N, boss.e);
			break;
		default:
			break;
		}
	}
	else
	{
		switch (enemyType)
		{
		case TEST:
			enemy.push_back(new TestEnemy(player, Vector3(), testEnemy.hp, testEnemy.N, testEnemy.e));
			break;
		case RUSH:
			enemy.push_back(new RushEnemy(player, Vector3(), rushEnemy.hp, rushEnemy.N, rushEnemy.e));
			break;
		case BOSS:
			enemy.push_back(new Boss(player, Vector3(), boss.hp, boss.N, boss.e));
			break;
		default:
			break;
		}
	}
}
