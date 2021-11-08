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
private:	//記述省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:		//構造体
	//頂点データ構造体
	struct VertexPosUv {
		XMFLOAT3 pos;	//x,y,z座標
		XMFLOAT2 uv;	//uv座標
	};

	//定数バッファ用データ構造体
	struct ConstBufferData {
		XMFLOAT4 color; //色(RGBA)
		XMMATRIX mat;	//3D変換行列
	};

	//パイプラインセット
	struct PipelineSet {
		ComPtr<ID3D12PipelineState> pipelinestate;
		ComPtr<ID3D12RootSignature> rootsignature;
	};

	//スプライト1枚分のデータ
	struct Sprite {
		ComPtr<ID3D12Resource> vertBuff;		//頂点バッファ
		D3D12_VERTEX_BUFFER_VIEW vbView{};		//頂点バッファビュー
		ComPtr<ID3D12Resource> constBuff;		//定数バッファ
		float rotation = 0.0f;					//z軸の回転角
		Vector3 position = Vector3(0,0,0);		//座標
		XMMATRIX matWorld;						//ワールド行列
		XMFLOAT4 color = { 1,1,1,1 };			//色
		XMFLOAT2 size = { 100,100 };			//大きさ
		UINT texNumber = 0;						//テクスチャナンバー
		XMFLOAT2 anchorpoint = { 0.5f,0.5f };	//アンカーポイント
		bool isFlipX = false;					//左右反転
		bool isFlipY = false;					//上下反転
		XMFLOAT2 texLeftTop = { 0,0 };			//テクスチャ左上座標
		XMFLOAT2 texSize = { 100,100 };		//切り取り範囲
		bool isInvisible = false;
	};

	//スプライト共通のデータ
	static const int spriteSRVCount = 512;
	struct SpriteCommon {
		PipelineSet pipelineSet;	//パイプラインセット
		XMMATRIX matProjection{};	//射影行列
		ComPtr<ID3D12DescriptorHeap> descHeap;	//テクスチャ用
		ComPtr<ID3D12Resource>	texBuff[spriteSRVCount];	//テクスチャ用
	};


public:		//静的メンバ関数
	static PipelineSet SpriteCreateGraphicPipeline(ID3D12Device *dev);	//スプライト用パイプライン生成	
	
	//スプライト共通グラフィックコマンドのセット
	static void SpriteCommonBeginDraw(ID3D12GraphicsCommandList *cmdList);
	//スプライト共通テクスチャ読み込み
	static void SpriteCommonLoadTexture(UINT texnumber, const wchar_t *filename, ID3D12Device *dev);
	//スプライト共通データ生成
	static void SpriteCommonCreate(int window_width, int window_height, ID3D12Device *dev);
	//スプライト生成
	static Sprite SpriteCreate(ID3D12Device *dev, int window_width, int window_height, UINT texNumber,XMFLOAT2 anchorpoint = { 0.5f,0.5f },bool isFlipX = false, bool isFlipY = false);
	//スプライト単体更新
	static void SpriteUpdate(Sprite &sprite);
	//スプライト単体頂点バッファ転送
	static void SpriteTransferVertexBuffer(const Sprite &sprite);
	//スプライト単体描画
	static void SpriteDraw(const Sprite &sprite, ID3D12GraphicsCommandList *cmdList, ID3D12Device *dev);	//サイズ変更後に呼び出す
	//SpriteCommon取得
	static SpriteCommon GetCommon() { return spriteCommon; }

private:	//静的メンバ変数
	static SpriteCommon spriteCommon;

};