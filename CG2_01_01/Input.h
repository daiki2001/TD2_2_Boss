#pragma once
#include "Vector3.h"

class Input final
{
private: // �f�t�H���g�R���X�g���N�^�E�f�X�g���N�^��private��
	Input() {}
	~Input() {}
public: // �R���X�g���N�^�֌W�̐ݒ�
	// �R�s�[�R���X�g���N�^�̍폜
	Input(const Input&) = delete;
	// ������Z�q�̍폜
	const Input& operator=(const Input&) = delete;

public: // �ÓI�����o�֐�
	// ������
	static void Init();
	// �X�V
	static void Update();

	/*�e���͔���*/
	// �I��
	static bool Exit();
	// �ړ�
	static Vector3 Move();
	// �U������
	static bool AttackPrepare();
	// �U��
	static bool Attack();
	// ���b�N�I��
	static bool LockOn();
	// �e�X�g�p(PC��p)
	static bool Test();
};
