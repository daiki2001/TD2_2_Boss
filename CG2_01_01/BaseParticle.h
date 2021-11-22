#pragma once
#include "Particle/ParticleManager.h"
#include "Vector3.h"
#include <DirectXMath.h>
#include <vector>
#include <string>

class BaseParticle
{
public:
	BaseParticle();
	~BaseParticle();

public: // ���z�֐�
	// ����������
	virtual void Initialize() {}
	// �X�V����
	virtual void Update() {}
	virtual void Update(const bool& isCreate, const Vector3& offset = Vector3()) {}
	// �`�揈��
	virtual void Draw() const;
	// �I������
	virtual void Finalize();

private: // �ÓI�����o�ϐ�
	static std::vector<std::string> texFilepath; //�e�N�X�`���̖��O�Q

public:
	// ���o���̃p�[�e�B�N���̐�
	size_t GetParticleNum();
protected: // �����o�֐�
	// ParticleManager�̐�������
	void CreateManager(std::string texFilepath);

public: // �����o�ϐ�
	int lifeTime; //��������

	Vector3 pos;   //���W
	Vector3 speed; //���x
	Vector3 accel; //�����x

	float startScale; //�J�n���X�P�[��
	float endScale;   //�I�����X�P�[��

	DirectX::XMFLOAT4 startColor; //�J�n���̐F
	DirectX::XMFLOAT4 endColor;   //�I�����̐F
protected:
	ParticleManager* manager;

};
