﻿#include "ParticleManager.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include "../DirectX/Object3d/Object3d.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ID3D12Device* ParticleManager::device = nullptr;
UINT ParticleManager::descriptorHandleIncrementSize = 0;
ID3D12GraphicsCommandList* ParticleManager::cmdList = nullptr;
ComPtr<ID3D12RootSignature> ParticleManager::rootsignature;
ComPtr<ID3D12PipelineState> ParticleManager::pipelinestate;
ComPtr<ID3D12DescriptorHeap> ParticleManager::descHeap;
vector<ComPtr<ID3D12Resource>> ParticleManager::vertBuff;
vector<ComPtr<ID3D12Resource>> ParticleManager::texbuff;
vector<CD3DX12_CPU_DESCRIPTOR_HANDLE> ParticleManager::cpuDescHandleSRV;
vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> ParticleManager::gpuDescHandleSRV;
XMMATRIX ParticleManager::matView{};
XMMATRIX ParticleManager::matProjection{};
XMFLOAT3 ParticleManager::eye = { 0, 0, -5.0f };
XMFLOAT3 ParticleManager::target = { 0, 0, 0 };
XMFLOAT3 ParticleManager::up = { 0, 1, 0 };
XMMATRIX ParticleManager::matBillboard = XMMatrixIdentity();
XMMATRIX ParticleManager::matBillboardY = XMMatrixIdentity();
vector<D3D12_VERTEX_BUFFER_VIEW> ParticleManager::vbView{};

ParticleManager::ParticleManager()
{
	vertexIndex = -1;
	textureIndex = -1;
}

ParticleManager::~ParticleManager()
{
}

bool ParticleManager::StaticInitialize(ID3D12Device* device, int window_width, int window_height)
{
	if (ParticleManager::device != nullptr)
	{
		return true;
	}

	// nullptrチェック
	assert(device);

	ParticleManager::device = device;

	// デスクリプタヒープの初期化
	if (InitializeDescriptorHeap() == false)
	{
		assert(0);
		return false;
	}

	// カメラ初期化
	InitializeCamera(window_width, window_height);

	// パイプライン初期化
	if (InitializeGraphicsPipeline() == false)
	{
		assert(0);
		return false;
	}

	return true;
}

ParticleManager* ParticleManager::Create()
{
	// パーティクルオブジェクトのインスタンスを生成
	ParticleManager* particleMan = new ParticleManager();
	if (particleMan == nullptr) {
		return nullptr;
	}

	// 頂点データ生成
	particleMan->vertexIndex = CreateVertex();
	if (particleMan->vertexIndex == -1)
	{
		return nullptr;
	}

	HRESULT result;
	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&particleMan->constBuff));

	return particleMan;
}

void ParticleManager::SetEye(XMFLOAT3 eye)
{
	ParticleManager::eye = eye;

	UpdateViewMatrix();
}

void ParticleManager::SetTarget(XMFLOAT3 target)
{
	ParticleManager::target = target;

	UpdateViewMatrix();
}

void ParticleManager::CameraMoveVector(XMFLOAT3 move)
{
	XMFLOAT3 eye_moved = GetEye();
	XMFLOAT3 target_moved = GetTarget();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	target_moved.x += move.x;
	target_moved.y += move.y;
	target_moved.z += move.z;

	SetEye(eye_moved);
	SetTarget(target_moved);
}

void ParticleManager::CameraMoveEyeVector(XMFLOAT3 move)
{
	XMFLOAT3 eye_moved = GetEye();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	SetEye(eye_moved);
}

bool ParticleManager::InitializeDescriptorHeap()
{
	HRESULT result = S_FALSE;
	
	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = 1000; // シェーダーリソースビュー
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));//生成
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// デスクリプタサイズを取得
	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	return true;
}

void ParticleManager::InitializeCamera(int window_width, int window_height)
{
	// ビュー行列の計算
	UpdateViewMatrix();

	// 透視投影による射影行列の生成
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)window_width / window_height,
		0.1f, 1000.0f
	);
}

bool ParticleManager::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob; // ジオメトリシェーダオブジェクト
	ComPtr<ID3DBlob> psBlob; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	static const std::wstring fileDir = L"Particle/"; //シェーダファイルのディレクトリ
	static const std::wstring vsFilePath = fileDir + L"ParticleVS.hlsl"; //頂点シェーダのファイルパス
	static const std::wstring gsFilePath = fileDir + L"ParticleGS.hlsl"; //ジオメトリシェーダのファイルパス
	static const std::wstring psFilePath = fileDir + L"ParticlePS.hlsl"; //ピクセルシェーダのファイルパス

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		vsFilePath.c_str(), // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		gsFilePath.c_str(), // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "gs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&gsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		psFilePath.c_str(), // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 色
			"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // スケール
			"SCALE", 0, DXGI_FORMAT_R32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // スケール
			"ROTATE", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO; //デプスの書き込み禁止

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	// 半透明合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	//blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	//blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	// 加算合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_ONE;
	blenddesc.DestBlend = D3D12_BLEND_ONE;
	// 減算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;
	//blenddesc.DestBlend = D3D12_BLEND_ONE;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[2] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	if (FAILED(result)) {
		return false;
	}

	gpipeline.pRootSignature = rootsignature.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));

	if (FAILED(result)) {
		return false;
	}

	return true;
}

int ParticleManager::LoadTexture(WCHAR* filePath)
{
	static const int failure = -1;
	HRESULT result = S_FALSE;

	// WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	result = LoadFromWICFile(
		filePath, WIC_FLAGS_NONE,
		&metadata, scratchImg);
	if (FAILED(result)) {
		return failure;
	}

	const Image* img = scratchImg.GetImage(0, 0, 0); // 生データ抽出

	// リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);

	static int index = 0;
	index = (int)texbuff.size();
	texbuff.push_back({});
	cpuDescHandleSRV.push_back({});
	gpuDescHandleSRV.push_back({});

	// テクスチャ用バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&texbuff[index]));
	if (FAILED(result)) {
		return failure;
	}

	// テクスチャバッファにデータ転送
	result = texbuff[index]->WriteToSubresource(
		0,
		nullptr, // 全領域へコピー
		img->pixels,    // 元データアドレス
		(UINT)img->rowPitch,  // 1ラインサイズ
		(UINT)img->slicePitch // 1枚サイズ
	);
	if (FAILED(result)) {
		return failure;
	}

	// シェーダリソースビュー作成
	cpuDescHandleSRV[index] = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), index, descriptorHandleIncrementSize);
	gpuDescHandleSRV[index] = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), index, descriptorHandleIncrementSize);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	D3D12_RESOURCE_DESC resDesc = texbuff[index]->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(texbuff[index].Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		cpuDescHandleSRV[index]
	);

	return index;
}

int ParticleManager::CreateVertex()
{
	static const int failure = -1;
	HRESULT result = S_FALSE;
	static VertexPos vertices[vertexCount]; // 頂点データ配列

	static int index = 0;
	index = (int)vertBuff.size();
	vertBuff.push_back({});
	vbView.push_back({});

	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff[index]));
	if (FAILED(result)) {
		assert(0);
		return failure;
	}

	// 頂点バッファビューの作成
	vbView[index].BufferLocation = vertBuff[index]->GetGPUVirtualAddress();
	vbView[index].SizeInBytes = sizeof(vertices);
	vbView[index].StrideInBytes = sizeof(vertices[0]);

	return index;
}

void ParticleManager::UpdateViewMatrix()
{
	XMVECTOR eyePos = XMLoadFloat3(&Object3d::objectCommon.eye); //視点座標
	XMVECTOR targetPos = XMLoadFloat3(&Object3d::objectCommon.target); //注視点座標
	XMVECTOR upVector = XMLoadFloat3(&Object3d::objectCommon.up); //（仮の）上方向

	// カメラのZ軸（視線方向）
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPos, eyePos);

	// 0ベクトルだと向きが定まらないから除外
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));

	// ベクトルを正規化
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	// カメラのX軸
	XMVECTOR cameraAxisX;
	// X軸は上方向→Z軸の外積で求まる
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	// ベクトルを正規化
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	// カメラのY軸
	XMVECTOR cameraAxisY;
	// Y軸はZ軸→X軸の外積で求まる
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);
	// ベクトルを正規化
	cameraAxisY = XMVector3Normalize(cameraAxisY);

	// カメラ回転行列
	XMMATRIX matCameraRot;
	// カメラ座標系→ワールド座標系の変換行列
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);

	// 転置により逆行列（逆回転）を計算
	matView = XMMatrixTranspose(matCameraRot);

	// 視点座標に-1を掛けた座標
	XMVECTOR reverseEyePos = XMVectorNegate(eyePos);
	// カメラの位置からワールド原点へのベクトル（カメラ座標系）
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePos); //X成分
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePos); //Y成分
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePos); //Z成分
	// 一つのベクトルにまとめる
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

	// ビュー行列に平行移動成分を設定
	matView.r[3] = translation;

#pragma region 全方向ビルボード行列の計算
	// ビルボード行列
	matBillboard.r[0] = cameraAxisX;
	matBillboard.r[1] = cameraAxisY;
	matBillboard.r[2] = cameraAxisZ;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma endregion

#pragma region Y軸回りビルボード行列の計算
	// カメラX軸、Y軸、Z軸
	XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

	// X軸は共通
	ybillCameraAxisX = cameraAxisX;
	// Y軸はワールド座標系のY軸
	ybillCameraAxisY = XMVector3Normalize(upVector);
	// Z軸はX軸→Y軸の外積で求まる
	ybillCameraAxisZ = XMVector3Cross(ybillCameraAxisX, ybillCameraAxisY);

	matBillboardY.r[0] = ybillCameraAxisX;
	matBillboardY.r[1] = ybillCameraAxisY;
	matBillboardY.r[2] = ybillCameraAxisZ;
	matBillboardY.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma endregion
}

ParticleManager* ParticleManager::Initialize(WCHAR* filePath)
{
	// パーティクルオブジェクトのインスタンスを生成
	ParticleManager* particleMan = Create();
	if (particleMan == nullptr) {
		return nullptr;
	}

	// nullptrチェック
	assert(device);

	// テクスチャ読み込み
	particleMan->textureIndex = LoadTexture(filePath);
	if (particleMan->textureIndex == -1)
	{
		return nullptr;
	}

	return particleMan;
}

void ParticleManager::Update()
{
	HRESULT result;

	// ビュー行列の計算
	UpdateViewMatrix();

	// 寿命が尽きたパーティクルを全削除
	particles.remove_if([](Particle& x) { return x.frame >= x.num_frame; });

	// 全パーティクル更新
	for (auto it = particles.begin(); it != particles.end(); it++)
	{
		// 経過フレーム数をカウント
		it->frame++;
		// 進行率
		float f = (float)it->num_frame / it->frame;

		if (it->isBezier)
		{
			static Vector3 t1, t2;
			t1 = (it->control_point - it->start_pos) / f;
			t1 += it->start_pos;
			t2 = (it->end_pos - it->control_point) / f;
			t2 += it->control_point;
			it->pos = (t2 - t1) / f;
			it->pos += t1;
		}
		else
		{
			// 速度に加速度を加算
			it->velocity = it->velocity + it->accel;
			// 速度による移動
			it->pos = it->pos + it->velocity;
		}
		// 角度の線形補間
		it->angle = (it->e_angle - it->s_angle) / f;
		it->angle += it->s_angle;
		// スケールの線形補間
		it->scale = (it->e_scale - it->s_scale) / f;
		it->scale += it->s_scale;
		// 色の線形補間
		it->color = (it->e_color - it->s_color) / f;
		it->color = it->color + it->s_color;
	}

	// 頂点バッファへのデータ転送
	VertexPos* vertMap = nullptr;
	result = vertBuff[vertexIndex]->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		for (auto it = particles.begin(); it != particles.end(); it++)
		{
			// 座標
			vertMap->pos = it->pos;
			// スケール
			vertMap->scale = it->scale;
			// 色
			vertMap->color = it->color;
			// 回転角度
			vertMap->angle = it->angle;
			// 次の頂点へ
			vertMap++;
		}
		vertBuff[vertexIndex]->Unmap(0, nullptr);
	}

	// 定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = matView * matProjection;
	constMap->matBillboard = matBillboard;
	constBuff->Unmap(0, nullptr);
}

void ParticleManager::Draw(ID3D12GraphicsCommandList* cmdList)
{
	// nullptrチェック
	assert(cmdList);

	// パイプラインステートの設定
	cmdList->SetPipelineState(pipelinestate.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView[vertexIndex]);

	// デスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandleSRV[textureIndex]);
	// 描画コマンド
	cmdList->DrawInstanced((UINT)std::distance(particles.begin(), particles.end()), 1, 0, 0);
}

void ParticleManager::Add(int life, XMFLOAT3 pos, XMFLOAT3 velocity, XMFLOAT3 accel, XMFLOAT3 start_angle, XMFLOAT3 end_angle,
	float start_scale, float end_scale, XMFLOAT4 start_color, XMFLOAT4 end_color)
{
	if (std::distance(particles.begin(), particles.end()) >= vertexCount)
	{
		return;
	}

	// リストに要素を追加
	particles.emplace_front();
	// 追加した要素の参照
	Particle& p = particles.front();
	// 値のセット
	p.pos = pos;
	p.velocity = velocity;
	p.accel = accel;
	p.num_frame = life;
	p.angle = XMFLOAT3(XMConvertToRadians(start_angle.x), XMConvertToRadians(start_angle.y), XMConvertToRadians(start_angle.z));
	p.s_angle = XMFLOAT3(XMConvertToRadians(start_angle.x), XMConvertToRadians(start_angle.y), XMConvertToRadians(start_angle.z));
	p.e_angle = XMFLOAT3(XMConvertToRadians(end_angle.x), XMConvertToRadians(end_angle.y), XMConvertToRadians(end_angle.z));
	p.scale = start_scale;
	p.s_scale = start_scale;
	p.e_scale = end_scale;
	p.color = start_color;
	p.s_color = start_color;
	p.e_color = end_color;
	p.isBezier = false;
	p.start_pos = p.pos;
	p.control_point = p.pos;
	p.end_pos = p.pos;
}

void ParticleManager::AddBezier(int life, XMFLOAT3 start_pos, XMFLOAT3 control_point, XMFLOAT3 end_pos, XMFLOAT3 start_angle, XMFLOAT3 end_angle,
	float start_scale, float end_scale, XMFLOAT4 start_color, XMFLOAT4 end_color)
{
	if (std::distance(particles.begin(), particles.end()) >= vertexCount)
	{
		return;
	}

	// リストに要素を追加
	particles.emplace_front();
	// 追加した要素の参照
	Particle& p = particles.front();
	// 値のセット
	p.pos = start_pos;
	p.velocity = XMFLOAT3();
	p.accel = XMFLOAT3();
	p.num_frame = life;
	p.angle = XMFLOAT3(XMConvertToRadians(start_angle.x), XMConvertToRadians(start_angle.y), XMConvertToRadians(start_angle.z));
	p.s_angle = XMFLOAT3(XMConvertToRadians(start_angle.x), XMConvertToRadians(start_angle.y), XMConvertToRadians(start_angle.z));
	p.e_angle = XMFLOAT3(XMConvertToRadians(end_angle.x), XMConvertToRadians(end_angle.y), XMConvertToRadians(end_angle.z));
	p.scale = start_scale;
	p.s_scale = start_scale;
	p.e_scale = end_scale;
	p.color = start_color;
	p.s_color = start_color;
	p.e_color = end_color;
	p.isBezier = true;
	p.start_pos = start_pos;
	p.control_point = control_point;
	p.end_pos = end_pos;
}

const DirectX::XMFLOAT4 operator+(const DirectX::XMFLOAT4& lhs, const DirectX::XMFLOAT4& rhs)
{
	DirectX::XMFLOAT4 result{};
	result.x += lhs.x + rhs.x;
	result.y += lhs.y + rhs.y;
	result.z += lhs.z + rhs.z;
	result.w += lhs.w + rhs.w;
	return result;
}

const DirectX::XMFLOAT4 operator-(const DirectX::XMFLOAT4& lhs, const DirectX::XMFLOAT4& rhs)
{
	DirectX::XMFLOAT4 result{};
	result.x += lhs.x - rhs.x;
	result.y += lhs.y - rhs.y;
	result.z += lhs.z - rhs.z;
	result.w += lhs.w - rhs.w;
	return result;
}

const DirectX::XMFLOAT4 operator/(const DirectX::XMFLOAT4& lhs, const float& rhs)
{
	DirectX::XMFLOAT4 result{};
	result.x += lhs.x / rhs;
	result.y += lhs.y / rhs;
	result.z += lhs.z / rhs;
	result.w += lhs.w / rhs;
	return result;
}
