#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>
#include <string>
#include <vector>

/// <summary>
/// パーティクルマネージャー
/// </summary>
class ParticleManager
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// std::を省略
	template <class T> using vector = std::vector<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	// 頂点データ構造体
	struct VertexPos
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT4 color; // 色 (RGBA)
		float scale; //スケール
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMMATRIX mat;	// ビュープロジェクション行列
		XMMATRIX matBillboard;	// ビルボード行列
	};

	//パーティクル一粒の情報
	struct Particle
	{
		XMFLOAT3 pos = {}; //座標
		XMFLOAT3 velocity = {}; //速度
		XMFLOAT3 accel = {}; //加速度
		int frame = 0; //現在フレーム
		int num_frame = 0; //終了フレーム

		float scale = 1.0f;   //スケール
		float s_scale = 1.0f; //スケールの初期値
		float e_scale = 0.0f; //スケールの最終値

		XMFLOAT4 color = {};   //色
		XMFLOAT4 s_color = {}; //色の初期値
		XMFLOAT4 e_color = {}; //色の最終値
	};

public:
	ParticleManager();
	~ParticleManager();

private: // 定数
	static const int vertexCount = 32768; // 頂点数

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	/// <returns>成否</returns>
	static bool StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	/// <summary>
	/// 視点座標の取得
	/// </summary>
	/// <returns>座標</returns>
	static const XMFLOAT3& GetEye() { return eye; }

	/// <summary>
	/// 視点座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	static void SetEye(XMFLOAT3 eye);

	/// <summary>
	/// 注視点座標の取得
	/// </summary>
	/// <returns>座標</returns>
	static const XMFLOAT3& GetTarget() { return target; }

	/// <summary>
	/// 注視点座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	static void SetTarget(XMFLOAT3 target);

	/// <summary>
	/// ベクトルによる移動
	/// </summary>
	/// <param name="move">移動量</param>
	static void CameraMoveVector(XMFLOAT3 move);

	/// <summary>
	/// ベクトルによる視点移動
	/// </summary>
	/// <param name="move">移動量</param>
	static void CameraMoveEyeVector(XMFLOAT3 move);

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// 頂点バッファ
	static vector<ComPtr<ID3D12Resource>> vertBuff;
	// テクスチャバッファ
	static vector<ComPtr<ID3D12Resource>> texbuff;
	// シェーダリソースビューのハンドル(CPU)
	static vector<CD3DX12_CPU_DESCRIPTOR_HANDLE> cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	static vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> gpuDescHandleSRV;
	// ビュー行列
	static XMMATRIX matView;
	// 射影行列
	static XMMATRIX matProjection;
	// 視点座標
	static XMFLOAT3 eye;
	// 注視点座標
	static XMFLOAT3 target;
	// 上方向ベクトル
	static XMFLOAT3 up;
	// ビルボード行列
	static XMMATRIX matBillboard;
	// Y軸回りビルボード行列
	static XMMATRIX matBillboardY;
	// 頂点バッファビュー
	static vector<D3D12_VERTEX_BUFFER_VIEW> vbView;

private:// 静的メンバ関数
	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	/// <returns></returns>
	static bool InitializeDescriptorHeap();

	/// <summary>
	/// カメラ初期化
	/// </summary>
	/// <param name="window_width">画面横幅</param>
	/// <param name="window_height">画面縦幅</param>
	static void InitializeCamera(int window_width, int window_height);

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static bool InitializeGraphicsPipeline();

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns>-1で失敗</returns>
	static int LoadTexture(WCHAR* filePath);

	/// <summary>
	/// 頂点データ作成
	/// </summary>
	/// <returns>-1で失敗</returns>
	static int CreateVertex();

	/// <summary>
	/// ビュー行列を更新
	/// </summary>
	static void UpdateViewMatrix();

public: // メンバ関数
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="filePath">画像のファイルパス</param>
	/// <returns>成否</returns>
	ParticleManager* Initialize(WCHAR* filePath);
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	ParticleManager* Create();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// パーティクルの追加
	/// </summary>
	/// <param name="life">生存時間</param>
	/// <param name="pos">初期座標</param>
	/// <param name="velocity">速度</param>
	/// <param name="accel">加速度</param>
	/// <param name="start_scale">開始時スケール</param>
	/// <param name="end_scale">終了時スケール</param>
	/// <param name="start_color">開始時の色</param>
	/// <param name="end_color">終了時の色</param>
	void Add(int life, XMFLOAT3 pos, XMFLOAT3 velocity, XMFLOAT3 accel, float start_scale, float end_scale, XMFLOAT4 start_color, XMFLOAT4 end_color);

private: // メンバ変数
	ComPtr<ID3D12Resource> constBuff; // 定数バッファ
	std::forward_list<Particle> particles; // パーティクル配列
	size_t vertexIndex; //頂点関連のインデックス
	size_t textureIndex; //テクスチャ関連のインデックス
};

// XMFLOAT3同士の加算処理
const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs);

// XMFLOAT4同士の加算処理
const DirectX::XMFLOAT4 operator+(const DirectX::XMFLOAT4& lhs, const DirectX::XMFLOAT4& rhs);
// XMFLOAT4同士の減算処理
const DirectX::XMFLOAT4 operator-(const DirectX::XMFLOAT4& lhs, const DirectX::XMFLOAT4& rhs);
// XMFLOAT4同士の除算処理
const DirectX::XMFLOAT4 operator/(const DirectX::XMFLOAT4& lhs, const float& rhs);
