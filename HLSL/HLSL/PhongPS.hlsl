#include "Phong.hlsli"

Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1); // �@���}�b�v
//Texture2D shadowMap : register(t2);		// �V���h�E�}�b�v
Texture2D shadowMap[ShadowmapCount] : register(t2); // �V���h�E�}�b�v�i�z��ɂ��Ă���̂�ShadowmapCount�����W�X�^�[�𗘗p���Ă���ƍl���Ă��������BShadowmapCount = 4�̏ꍇ��t2,t3,t4,t5�܂Ŏg���Ă��܂��j


SamplerState diffuseMapSamplerState : register(s0);
SamplerState shadowMapSamplerState : register(s1);

float4 main(VS_OUT pin) : SV_TARGET
{
    float4 diffuseColor = diffuseMap.Sample(diffuseMapSamplerState, pin.texcoord) * pin.color;

	//float3 N = normalize(pin.normal);
	// �@���}�b�v����xyz�������擾����( -1 �` +1 )�̊ԂɃX�P�[�����O
    float3 normal = normalMap.Sample(diffuseMapSamplerState, pin.texcoord).rgb;

	// �ϊ��p��3X3�s���p�ӂ���
	// ���ӓ_�Ƃ��āA���_�V�F�[�_�[���ŎZ�o�����P�ʃx�N�g���ނ̓��X�^���C�Y�̍ۂɐ��l�����_�Ԃŕ�Ԃ���܂��B
	// ���̂��߁A�s�N�Z���V�F�[�_�[�̃^�C�~���O�ł͒P�ʃx�N�g���ł͂Ȃ����̂������Ă���\��������̂ŁA
	// ���K����Y��Ȃ��悤�ɂ��Ă��������B
    float3x3 CM = { normalize(pin.tangent), normalize(pin.binormal), normalize(pin.normal) };

	// �@���}�b�v�Ŏ擾�����@�����ɕϊ��s����|�����킹�܂��傤�B���K���͖Y�ꂸ��
    float3 N = normalMap.Sample(diffuseMapSamplerState, pin.texcoord).rgb;
    N = normalize(mul(N * 2.0f - 1.0f, CM));

    float3 L = normalize(directionalLightData.direction.xyz);
    float3 E = normalize(viewPosition.xyz - pin.world_position.xyz);

	// �}�e���A���萔
    float3 ka = float3(1, 1, 1);
    float3 kd = float3(1, 1, 1);
    float3 ks = float3(1, 1, 1);
    float shiness = 128;

	// �����̌v�Z
    float3 ambient = ka * ambientLightColor;


	// ���s�����̃��C�e�B���O�v�Z
#if 1
    float3 directionalDiffuse = CalcLambertDiffuse(N, L, directionalLightData.color.rgb, kd);
#else
	float3 directionalDiffuse = CalcHalfLambert(N, L, directionalLightData.color.rgb, kd);
#endif
    float3 directionalSpecular = CalcPhongSpecular(N, L, directionalLightData.color.rgb, E, shiness, ks);

	// ���s�����̉e�Ȃ̂ŁA���s���b�ɑ΂��ĉe��K��
	//float3 shadow = CalcShadowColor(shadowMap, shadowMapSamplerState, pin.shadowTexcoord, shadowColor, shadowBias);
	//float3 shadow = CalcShadowColorPCFFilter(shadowMap, shadowMapSamplerState, pin.shadowTexcoord, shadowColor, shadowBias);
    float3 shadow = 1;
    for (int k = 0; k < ShadowmapCount; ++k)
    {
        float3 shadowTexcoord = pin.shadowTexcoord[k];
		// �V���h�E�}�b�v��UV�͈͓����A�[�x�l���͈͓������肷��
        if (shadowTexcoord.z >= 0 && shadowTexcoord.z <= 1 &&
			shadowTexcoord.x >= 0 && shadowTexcoord.x <= 1 &&
			shadowTexcoord.y >= 0 && shadowTexcoord.y <= 1)
        {
			// �V���h�E�}�b�v����[�x�l�擾
            float depth = shadowMap[k].Sample(shadowMapSamplerState, shadowTexcoord.xy).r;
			// �[�x�l���r���ĉe���ǂ����𔻒肷��
            if (shadowTexcoord.z - depth > shadowBias[k])
                shadow = shadowColor;
            break;
        }
    }

    directionalDiffuse *= shadow;
    directionalSpecular *= shadow;

	// �_�����̏���
    float3 pointDiffuse = (float3) 0;
    float3 pointSpecular = (float3) 0;
    int i;
    for (i = 0; i < pointLightCount; ++i)
    {
		// ���C�g�x�N�g�����Z�o
        float3 lightVector = pin.world_position.xyz - pointLightData[i].position.xyz;

		// ���C�g�x�N�g���̒������Z�o
        float lightLength = length(lightVector);

		// ���C�g�̉e���͈͊O�Ȃ��̌v�Z�����Ȃ��B
        if (lightLength >= pointLightData[i].range)
            continue;

		// �����������Z�o����
        float attenuate = saturate(1.0f - lightLength / pointLightData[i].range);

        lightVector = lightVector / lightLength;
        pointDiffuse += CalcLambertDiffuse(N, lightVector,
			pointLightData[i].color.rgb, kd.rgb) * attenuate;
        pointSpecular += CalcPhongSpecular(N, lightVector,
			pointLightData[i].color.rgb, E, shiness, ks.rgb) * attenuate;
    }

	// �X�|�b�g���C�g�̏���
    float3 spotDiffuse = (float3) 0;
    float3 spotSpecular = (float3) 0;
    int j;
    for (j = 0; j < spotLightCount; ++j)
    {
		// ���C�g�x�N�g�����Z�o
        float3 lightVector = pin.world_position.xyz - spotLightData[j].position.xyz;

		// ���C�g�x�N�g���̒������Z�o
        float lightLength = length(lightVector);

        if (lightLength >= spotLightData[j].range)
            continue;

		// �����������Z�o����
        float attenuate = saturate(1.0f - lightLength / spotLightData[j].range);

        lightVector = normalize(lightVector);

		// �p�x�������Z�o����attenuate�ɏ�Z����
        float3 spotDirection = normalize(spotLightData[j].direction.xyz);
        float angle = dot(spotDirection, lightVector);
        float area = spotLightData[j].innerCorn - spotLightData[j].outerCorn;
        attenuate *= saturate(1.0f - (spotLightData[j].innerCorn - angle) / area);

        spotDiffuse += CalcLambertDiffuse(N, lightVector,
			spotLightData[j].color.rgb, kd.rgb) * attenuate;
        spotSpecular += CalcPhongSpecular(N, lightVector,
			spotLightData[j].color.rgb, E, shiness, ks.rgb) * attenuate;
    }

    float4 color = float4(0, 0, 0, diffuseColor.a);
    color.rgb += diffuseColor.rgb * ambient;
    color.rgb += diffuseColor.rgb * (directionalDiffuse + pointDiffuse + spotDiffuse);
    color.rgb += directionalSpecular + pointSpecular + spotSpecular;

	// �������C�e�B���O
	//color.rgb += CalcRimLight(N, E, L, directionalLightData.color.rgb);
    return color;
}
