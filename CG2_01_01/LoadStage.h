#pragma once
#include <vector>
#include "GameObjCommon.h"
#include "Player.h"

class LoadStage
{
public:
	LoadStage() {}
	~LoadStage() {}

public: //�T�u�N���X
	// �G�̎��
	enum EnemyType
	{
		TEST,
		RUSH,
		BOSS
	};
	// �G�̐����ɕK�v�ȏ��
	struct Enemy
	{
		float hp = 0.0f;
		float N = 0.0f;
		float e = 0.0f;
	};

public: // �ÓI�����o�֐�
	// ����������
	static void Init();

	/// <summary>
	/// CSV�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="buffer"> �ǂݍ���CSV�t�@�C���̕ۑ��� </param>
	/// <param name="maxCount"> �i�[���鐔�l�̍ő吔 </param>
	/// <param name="FilePath"> CSV�t�@�C���̃p�X </param>
	/// <param name="LoadStopNumber"> �����Ɏw�肳�ꂽ�l���q�b�g����Ɠǂݍ��݂��I������ </param>
	/// <returns> ���� </returns>
	static bool LoadCSV(int* buffer, const size_t& maxCount, const char* FilePath, int LoadStopNumber = -1);

	// �X�e�[�W��ɔz�u�̓ǂݍ���
	static bool LoadStageEnemy(const char* FilePath, std::vector<GameObjCommon*>& enemy, Player* player);
private:
	// �G�̐���
	static void CreateEnemy(std::vector<GameObjCommon*>& enemy, const size_t& enemyIndex, const int& enemyType, Player* player);

private: // �����o�ϐ�
	static Enemy testEnemy; //TestEnemy�̌̒l���
	static Enemy rushEnemy; //RushEnemy�̌̒l���
	static Enemy boss;      //Boss�̌̒l���

};
