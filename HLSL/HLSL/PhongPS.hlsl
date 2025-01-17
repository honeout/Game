#include "Phong.hlsli"

Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1); // 法線マップ
//Texture2D shadowMap : register(t2);		// シャドウマップ
Texture2D shadowMap[ShadowmapCount] : register(t2); // シャドウマップ（配列にしているのでShadowmapCount分レジスターを利用していると考えてください。ShadowmapCount = 4の場合はt2,t3,t4,t5まで使われています）


SamplerState diffuseMapSamplerState : register(s0);
SamplerState shadowMapSamplerState : register(s1);

float4 main(VS_OUT pin) : SV_TARGET
{
    float4 diffuseColor = diffuseMap.Sample(diffuseMapSamplerState, pin.texcoord) * pin.color;

	//float3 N = normalize(pin.normal);
	// 法線マップからxyz成分を取得して( -1 〜 +1 )の間にスケーリング
    float3 normal = normalMap.Sample(diffuseMapSamplerState, pin.texcoord).rgb;

	// 変換用の3X3行列を用意する
	// 注意点として、頂点シェーダー内で算出した単位ベクトル類はラスタライズの際に数値が頂点間で補間されます。
	// そのため、ピクセルシェーダーのタイミングでは単位ベクトルではないものが入ってくる可能性があるので、
	// 正規化を忘れないようにしてください。
    float3x3 CM = { normalize(pin.tangent), normalize(pin.binormal), normalize(pin.normal) };

	// 法線マップで取得した法線情報に変換行列を掛け合わせましょう。正規化は忘れずに
    float3 N = normalMap.Sample(diffuseMapSamplerState, pin.texcoord).rgb;
    N = normalize(mul(N * 2.0f - 1.0f, CM));

    float3 L = normalize(directionalLightData.direction.xyz);
    float3 E = normalize(viewPosition.xyz - pin.world_position.xyz);

	// マテリアル定数
    float3 ka = float3(1, 1, 1);
    float3 kd = float3(1, 1, 1);
    float3 ks = float3(1, 1, 1);
    float shiness = 128;

	// 環境光の計算
    float3 ambient = ka * ambientLightColor;


	// 平行光源のライティング計算
#if 1
    float3 directionalDiffuse = CalcLambertDiffuse(N, L, directionalLightData.color.rgb, kd);
#else
	float3 directionalDiffuse = CalcHalfLambert(N, L, directionalLightData.color.rgb, kd);
#endif
    float3 directionalSpecular = CalcPhongSpecular(N, L, directionalLightData.color.rgb, E, shiness, ks);

	// 平行光源の影なので、平行怪訝に対して影を適応
	//float3 shadow = CalcShadowColor(shadowMap, shadowMapSamplerState, pin.shadowTexcoord, shadowColor, shadowBias);
	//float3 shadow = CalcShadowColorPCFFilter(shadowMap, shadowMapSamplerState, pin.shadowTexcoord, shadowColor, shadowBias);
    float3 shadow = 1;
    for (int k = 0; k < ShadowmapCount; ++k)
    {
        float3 shadowTexcoord = pin.shadowTexcoord[k];
		// シャドウマップのUV範囲内か、深度値が範囲内か判定する
        if (shadowTexcoord.z >= 0 && shadowTexcoord.z <= 1 &&
			shadowTexcoord.x >= 0 && shadowTexcoord.x <= 1 &&
			shadowTexcoord.y >= 0 && shadowTexcoord.y <= 1)
        {
			// シャドウマップから深度値取得
            float depth = shadowMap[k].Sample(shadowMapSamplerState, shadowTexcoord.xy).r;
			// 深度値を比較して影かどうかを判定する
            if (shadowTexcoord.z - depth > shadowBias[k])
                shadow = shadowColor;
            break;
        }
    }

    directionalDiffuse *= shadow;
    directionalSpecular *= shadow;

	// 点光源の処理
    float3 pointDiffuse = (float3) 0;
    float3 pointSpecular = (float3) 0;
    int i;
    for (i = 0; i < pointLightCount; ++i)
    {
		// ライトベクトルを算出
        float3 lightVector = pin.world_position.xyz - pointLightData[i].position.xyz;

		// ライトベクトルの長さを算出
        float lightLength = length(lightVector);

		// ライトの影響範囲外なら後の計算をしない。
        if (lightLength >= pointLightData[i].range)
            continue;

		// 距離減衰を算出する
        float attenuate = saturate(1.0f - lightLength / pointLightData[i].range);

        lightVector = lightVector / lightLength;
        pointDiffuse += CalcLambertDiffuse(N, lightVector,
			pointLightData[i].color.rgb, kd.rgb) * attenuate;
        pointSpecular += CalcPhongSpecular(N, lightVector,
			pointLightData[i].color.rgb, E, shiness, ks.rgb) * attenuate;
    }

	// スポットライトの処理
    float3 spotDiffuse = (float3) 0;
    float3 spotSpecular = (float3) 0;
    int j;
    for (j = 0; j < spotLightCount; ++j)
    {
		// ライトベクトルを算出
        float3 lightVector = pin.world_position.xyz - spotLightData[j].position.xyz;

		// ライトベクトルの長さを算出
        float lightLength = length(lightVector);

        if (lightLength >= spotLightData[j].range)
            continue;

		// 距離減衰を算出する
        float attenuate = saturate(1.0f - lightLength / spotLightData[j].range);

        lightVector = normalize(lightVector);

		// 角度減衰を算出してattenuateに乗算する
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

	// リムライティング
	//color.rgb += CalcRimLight(N, E, L, directionalLightData.color.rgb);
    return color;
}

