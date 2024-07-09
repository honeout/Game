#include "Light.hlsli"

struct VS_OUT
{
	float4 position : SV_POSITION;
	float3 world_position : POSITION;
	float3 normal : NORMAL;
	float2 texcoord : TEXCOORD;
	float4 color : COLOR;
	float4 posInProj	: TEXCOORD1;
};

cbuffer CbScene : register(b0)
{
	float4					viewPosition;
	row_major float4x4		viewProjection;
	float4					ambientLightColor;
	DirectionalLightData	directionalLightData;
	PointLightData			pointLightData[PointLightMax];	// �_�������
	SpotLightData			spotLightData[SpotLightMax];	// �X�|�b�g���C�g���
	int						pointLightCount;				// �_������
	int						spotLightCount;					// �X�|�b�g���C�g��
	float					outlineWidth;
	float					dummy;
	float4					outlineColor;
};

#define MAX_BONES 128
cbuffer CbMesh : register(b1)
{
	row_major float4x4	boneTransforms[MAX_BONES];
};

cbuffer CbSubset : register(b2)
{
	float4				materialColor;
};
