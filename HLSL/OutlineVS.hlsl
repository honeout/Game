#include "Outline.hlsli"

VS_OUT main(
	float4 position : POSITION,
	float3 normal : NORMAL,
	float3 tangent : TANGENT,
	float2 texcoord : TEXCOORD,
	float4 color : COLOR,
	float4 boneWeights : WEIGHTS,
	uint4 boneIndices : BONES
)
{
    float3 p = { 0, 0, 0 };
    float3 n = { 0, 0, 0 };
    for (int i = 0; i < 4; i++)
    {
        p += (boneWeights[i] * mul(position, boneTransforms[boneIndices[i]])).xyz;
        n += (boneWeights[i] * mul(float4(normal.xyz, 0), boneTransforms[boneIndices[i]])).xyz;
    }
    n = normalize(n);

    VS_OUT vout;
// �@�������Ɋg��    return diffuseMap.Sample(diffuseMapSamplerState, pin.texcoord);

    float scaleFactor = outlineSize; // �g��̃X�P�[���t�@�N�^�[�i�K�؂Ȓl�ɒu�������Ă��������j
    float3 scaledNormal = n * scaleFactor; // �@�����X�P�[�����O
    vout.position = float4(p + scaledNormal, 1.0f); // �@�������Ɋg�債���ʒu��V�����ʒu�Ƃ��Đݒ�

    vout.position = mul(vout.position, viewProjection);
    return vout;
}
