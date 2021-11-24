#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <vector>
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <wrl.h>

#pragma comment(lib, "d3dcompiler.lib")

#include "Vector3.h"


class Sprite
{
private:	//�L�q�ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:		//�T�u���X
	//���_�f�[�^�\����
	struct VertexPosUv {
		XMFLOAT3 pos;	//x,y,z���W
		XMFLOAT2 uv;	//uv���W
	};

	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData {
		XMFLOAT4 color; //�F(RGBA)
		XMMATRIX mat;	//3D�ϊ��s��
	};


public:		//�ÓI�����o�֐�
	//�ÓI������
	static bool StaticInitialize(ID3D12Device *dev, int window_width, int window_height);	//�X�v���C�g�p�p�C�v���C������	
	//�e�N�X�`���ǂݍ���
	static bool LoadTexture(UINT texnumber, const wchar_t *filename, ID3D12Device *dev);
	//�`��O����
	static void PreDraw(ID3D12GraphicsCommandList *cmdList);
	//�`��㏈��
	static void PostDraw();
	//�X�v���C�g����
	static Sprite *Create(UINT texNumber, int window_width, int window_height, XMFLOAT2 anchorpoint = { 0.5f,0.5f },bool isFlipX = false, bool isFlipY = false);

public:		//�����o�֐�
	Sprite(UINT texNumber, XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY);
	//������
	bool Initialize();
	//��]
	void SetRotation(float rotation);
	//���W
	void SetPosition(XMFLOAT2 position);
	//�c���̃T�C�Y
	void SetSize(XMFLOAT2 size);
	//�A���J�[�|�C���g�ύX
	void SetAnchorPoint(XMFLOAT2 anchorpoint);
	//���E���]
	void SetIsFlipX(bool isFlipX);
	//�㉺���]
	void SetIsFlipY(bool isFlipY);
	//�e�N�X�`���͈͎w��
	void SetTextureRect(XMFLOAT2 texBase, XMFLOAT2 texSize);
	//�`��
	void Draw();

private:	//�ÓI�����o�ϐ�
	static const int SRVCount = 512;				//�e�N�X�`���̍ő吔
	static ID3D12Device *dev;					//�f�o�C�X
	static ID3D12GraphicsCommandList *cmdList;		//�R�}���h���X�g
	static ComPtr<ID3D12RootSignature> rootSignature;	//���[�g�V�O�l�`��
	static ComPtr<ID3D12PipelineState> pipelineState;	//�p�C�v���C���X�e�[�g
	static XMMATRIX matProjection;						//�ˉe�s��
	static ComPtr<ID3D12DescriptorHeap> descHeap;		//�f�X�N���v�^�q�[�v
	static ComPtr<ID3D12Resource> texBuff[SRVCount];	//�e�N�X�`���o�b�t�@


private:
	ComPtr<ID3D12Resource> vertBuff;		//���_�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;		//�萔�o�b�t�@
	D3D12_VERTEX_BUFFER_VIEW vbView{};		//���_�o�b�t�@�r���[
	UINT texNumber = 0;						//�e�N�X�`���i���o�[
	float rotation = 0.0f;					//z���̉�]�p
	XMFLOAT2 position;		//���W
	XMFLOAT2 size;			//�傫��
	XMFLOAT2 anchorpoint;	//�A���J�[�|�C���g
	XMMATRIX matWorld;						//���[���h�s��
	XMFLOAT4 color;			//�F
	bool isFlipX;					//���E���]
	bool isFlipY;					//�㉺���]
	XMFLOAT2 texLeftTop = { 0,0 };			//�e�N�X�`��������W
	XMFLOAT2 texSize = { 100,100 };		//�؂���͈�
	bool isInvisible = false;

private:
	void TransferVertices();
};