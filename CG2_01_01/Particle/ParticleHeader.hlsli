cbuffer cbuff0 : register(b0)
{
	matrix mat; // ビュープロジェクション行列
	matrix matBillboard; // ビルボード行列
};

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 pos : SV_POSITION; // システム用頂点座標
	float4 color : COLOR; //色
	float scale : SCALE; //スケール
	float3 angle : ROTATE; //回転角度
};

// ジオメトリシェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct GSOutput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
	float4 color : COLOR; //色
	float2 uv  :TEXCOORD; // uv値
};
