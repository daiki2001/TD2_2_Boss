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
	float3 s = float3(sin(input[0].angle.x), sin(input[0].angle.y), sin(input[0].angle.z));
	float3 c = float3(cos(input[0].angle.x), cos(input[0].angle.y), cos(input[0].angle.z));

	matrix rotAxis[3] = {
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f,  c.x,  s.x, 0.0f,
			0.0f, -s.x,  c.x, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		},
		{
			 c.y, 0.0f, -s.y, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			 s.y, 0.0f,  c.y, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		},
		{
			 c.z,  s.z, 0.0f, 0.0f,
			-s.z,  c.z, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		},
	};

	matrix rot = rotAxis[2];
	rot = mul(rotAxis[0], rot);
	rot = mul(rotAxis[1], rot);

	// 4点分まわす
	for (uint i = 0; i < vnum; i++)
	{
		float4 offset;
		offset = offset_array[i] * input[0].scale;
		offset = mul(rot, offset);
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