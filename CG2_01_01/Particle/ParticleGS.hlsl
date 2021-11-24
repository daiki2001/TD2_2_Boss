#include "ParticleHeader.hlsli"

// �l�p�`�̒��_��
static const uint vnum = 4;

// �Z���^�[����̃I�t�Z�b�g
static const float4 offset_array[vnum] =
{
	float4(-0.5f, -0.5f, 0.0f, 0.0f), //����
	float4(-0.5f, +0.5f, 0.0f, 0.0f), //����
	float4(+0.5f, -0.5f, 0.0f, 0.0f), //�E��
	float4(+0.5f, +0.5f, 0.0f, 0.0f), //�E��
};

static const float2 uv_array[vnum] =
{
	float2(0.0f, 1.0f), //����
	float2(0.0f, 0.0f), //����
	float2(1.0f, 1.0f), //�E��
	float2(1.0f, 0.0f), //�E��
};

// �_�̓��͂���A�l�p�`���o��
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

	// 4�_���܂킷
	for (uint i = 0; i < vnum; i++)
	{
		float4 offset;
		offset = offset_array[i] * input[0].scale;
		offset = mul(rot, offset);
		offset = mul(matBillboard, offset);

		// ���[���h���W�x�[�X�ł��炷
		element.svpos = input[0].pos + offset;
		// �r���[�A�ˉe�ϊ�
		element.svpos = mul(mat, element.svpos);
		element.uv = uv_array[i];
		element.color = input[0].color;
		output.Append(element);
	}
}