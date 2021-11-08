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


class Object2d
{
private:	//�L�q�ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:		//�\����
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

	//�p�C�v���C���Z�b�g
	struct PipelineSet {
		ComPtr<ID3D12PipelineState> pipelinestate;
		ComPtr<ID3D12RootSignature> rootsignature;
	};

	//�X�v���C�g1�����̃f�[�^
	struct Sprite {
		ComPtr<ID3D12Resource> vertBuff;		//���_�o�b�t�@
		D3D12_VERTEX_BUFFER_VIEW vbView{};		//���_�o�b�t�@�r���[
		ComPtr<ID3D12Resource> constBuff;		//�萔�o�b�t�@
		float rotation = 0.0f;					//z���̉�]�p
		Vector3 position = Vector3(0,0,0);		//���W
		XMMATRIX matWorld;						//���[���h�s��
		XMFLOAT4 color = { 1,1,1,1 };			//�F
		XMFLOAT2 size = { 100,100 };			//�傫��
		UINT texNumber = 0;						//�e�N�X�`���i���o�[
		XMFLOAT2 anchorpoint = { 0.5f,0.5f };	//�A���J�[�|�C���g
		bool isFlipX = false;					//���E���]
		bool isFlipY = false;					//�㉺���]
		XMFLOAT2 texLeftTop = { 0,0 };			//�e�N�X�`��������W
		XMFLOAT2 texSize = { 100,100 };		//�؂���͈�
		bool isInvisible = false;
	};

	//�X�v���C�g���ʂ̃f�[�^
	static const int spriteSRVCount = 512;
	struct SpriteCommon {
		PipelineSet pipelineSet;	//�p�C�v���C���Z�b�g
		XMMATRIX matProjection{};	//�ˉe�s��
		ComPtr<ID3D12DescriptorHeap> descHeap;	//�e�N�X�`���p
		ComPtr<ID3D12Resource>	texBuff[spriteSRVCount];	//�e�N�X�`���p
	};


public:		//�ÓI�����o�֐�
	static PipelineSet SpriteCreateGraphicPipeline(ID3D12Device *dev);	//�X�v���C�g�p�p�C�v���C������	
	
	//�X�v���C�g���ʃO���t�B�b�N�R�}���h�̃Z�b�g
	static void SpriteCommonBeginDraw(ID3D12GraphicsCommandList *cmdList);
	//�X�v���C�g���ʃe�N�X�`���ǂݍ���
	static void SpriteCommonLoadTexture(UINT texnumber, const wchar_t *filename, ID3D12Device *dev);
	//�X�v���C�g���ʃf�[�^����
	static void SpriteCommonCreate(int window_width, int window_height, ID3D12Device *dev);
	//�X�v���C�g����
	static Sprite SpriteCreate(ID3D12Device *dev, int window_width, int window_height, UINT texNumber,XMFLOAT2 anchorpoint = { 0.5f,0.5f },bool isFlipX = false, bool isFlipY = false);
	//�X�v���C�g�P�̍X�V
	static void SpriteUpdate(Sprite &sprite);
	//�X�v���C�g�P�̒��_�o�b�t�@�]��
	static void SpriteTransferVertexBuffer(const Sprite &sprite);
	//�X�v���C�g�P�̕`��
	static void SpriteDraw(const Sprite &sprite, ID3D12GraphicsCommandList *cmdList, ID3D12Device *dev);	//�T�C�Y�ύX��ɌĂяo��
	//SpriteCommon�擾
	static SpriteCommon GetCommon() { return spriteCommon; }

private:	//�ÓI�����o�ϐ�
	static SpriteCommon spriteCommon;

};