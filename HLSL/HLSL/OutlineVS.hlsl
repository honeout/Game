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
// 法線方向に拡大    return diffuseMap.Sample(diffuseMapSamplerState, pin.texcoord);

    float scaleFactor = outlineSize; // 拡大のスケールファクター（適切な値に置き換えてください）
    float3 scaledNormal = n * scaleFactor; // 法線をスケーリング
    vout.position = float4(p + scaledNormal, 1.0f); // 法線方向に拡大した位置を新しい位置として設定

    vout.position = mul(vout.position, viewProjection);
    return vout;
}
