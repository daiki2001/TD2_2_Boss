#include "ParticleHeader.hlsli"

//VSOutput main(float4 pos : POSITION)
/*資料03-03*/
VSOutput main(float4 pos : POSITION, float4 color : COLOR, float scale : SCALE, float3 angle : ROTATE)
{
	VSOutput output; // ピクセルシェーダーに渡す値
	//output.svpos = mul(mat, pos);
	//output.normal = normal;
	//output.uv = uv;
	output.pos = pos;
	/*資料03-03*/
	output.color = color;
	output.scale = scale;
	output.angle = angle;
	return output;
}