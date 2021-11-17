#include "ParticleHeader.hlsli"

// 四角形の頂点数
static const uint vnum = 4;

// センターからのオフセット
static const float4 offset_array[vnum] =
{
	float4(-0.5f, -0.5f, 0.0f, 0.0f), //左下
	float4(-0.5f, +0.5f, 0.0f, 0.0f), //左上
	float4(+0.5f, -0.5f, 0.0f, 0.0f), //右下
	float4(+0.5f, +0.5f, 0.0f, 0.0f), //右上
};

static const float2 uv_array[vnum] =
{
	float2(0.0f, 1.0f), //左下
	float2(0.0f, 0.0f), //左上
	float2(1.0f, 1.0f), //右下
	float2(1.0f, 0.0f), //右上
};

// 点の入力から、四角形を出力
[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION, 
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;
	// 4点分まわす
	for (uint i = 0; i < vnum; i++)
	{
		float4 offset;
		//offset = mul(matBillboard, offset_array[i]);
		/*資料03-03*/
		offset = offset_array[i] * input[0].scale;
		offset = mul(matBillboard, offset);

		// ワールド座標ベースでずらす
		element.svpos = input[0].pos + offset;
		// ビュー、射影変換
		element.svpos = mul(mat, element.svpos);
		element.uv = uv_array[i];
		element.color = input[0].color;
		output.Append(element);
	}
}