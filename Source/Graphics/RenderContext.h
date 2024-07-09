#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

// UV スクロール情報
struct UVScrollData
{
	DirectX::XMFLOAT2 uvScrollValue;	// UV スクロール値
};

// マスクデータ
struct MaskData
{
	ID3D11ShaderResourceView*	maskTexture;
	float						dissolveThreshold;
	float						edgeThreshold;		// 縁の閾値
	DirectX::XMFLOAT4			edgeColor;			// 縁の色
};

// 平行光源情報
struct DirectionalLightData
{
	DirectX::XMFLOAT4 direction;	// 向き
	DirectX::XMFLOAT4 color;		// 色
};

// 点光源情報
struct PointLightData
{
	DirectX::XMFLOAT4	position;	// 座標
	DirectX::XMFLOAT4	color;		// 色
	float				range;		// 範囲
	DirectX::XMFLOAT3	dummy;
};

// 点光源の最大数
static constexpr int PointLightMax = 8;

// スポットライト
struct SpotLightData
{
	DirectX::XMFLOAT4	position;	// 座標
	DirectX::XMFLOAT4	direction;	// 向き
	DirectX::XMFLOAT4	color;		// 色
	float				range;		// 範囲
	float				innerCorn;	// インナー角度範囲
	float				outerCorn;	// アウター角度範囲
	float				dummy;
};

// スポットライトの最大数
static constexpr int SpotLightMax = 8;

// 色調補正情報
struct ColorGradingData
{
	float	hueShift = 0;	// 色相調整
	float	saturation = 1;	// 彩度調整
	float	brightness = 1;	// 明度調整
	float	dummy;
};

// ガウスフィルター計算情報
struct GaussianFilterData
{
	int		kernelSize	= 8;		// カーネルサイズ
	float	deviation	= 10.0f;	// 標準偏差
	DirectX::XMFLOAT2 textureSize;	// ぼかすテクスチャのサイズ
};

// ガウスフィルターの最大カーネルサイズ
static const int MaxKernelSize = 16;

// 高輝度抽出用情報
struct LuminanceExtractionData
{
	float	threshold = 0.5f;	// 閾値
	float	intensity = 1.0f;	// ブルームの強度
	DirectX::XMFLOAT2 dummy2;
};

// ポストエフェクトの最終パス用情報
struct FinalpassData
{
	// ブルームテクスチャ
	ID3D11ShaderResourceView* bloomTexture;
};


//	シャドウマップの枚数
static	const	int	ShadowmapCount = 4;

// シャドウマップ用情報
struct ShadowMapData 
{
	//ID3D11ShaderResourceView* shadowMap;				//	シャドウマップテクスチャ
	//DirectX::XMFLOAT4X4			lightViewProjection;	//	ライトビュープロジェクション行列
	//DirectX::XMFLOAT3			shadowColor;			//	影の色
	//float						shadowBias;				//	深度比較用のオフセット値

	//	シャドウマップ用深度ステンシルバッファ
	ID3D11ShaderResourceView* shadowMap[ShadowmapCount];
	//	ライトビュープロジェクション行列
	DirectX::XMFLOAT4X4		lightViewProjection[ShadowmapCount];
	//	影の色
	DirectX::XMFLOAT3			shadowColor;
	//	深度比較用のオフセット値
	float					shadowBias[ShadowmapCount];

};

struct ToonData
{
	ID3D11ShaderResourceView* edgeTexture;
	DirectX::XMFLOAT4 edgeColor = { 0,0,0,1 };		// 色
	float			  edgeWidth = 0.001f;

};

//struct PrimitiveRender
//{
//
//};




// レンダーコンテキスト
struct RenderContext
{
	ID3D11DeviceContext*	deviceContext;

	//	カメラ情報
	DirectX::XMFLOAT4		viewPosition;
	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;

	// スクロールデータ
	UVScrollData			uvScrollData;

	// マスクデータ
	MaskData				maskData;

	// ライト情報
	DirectX::XMFLOAT4		ambientLightColor;
	DirectionalLightData	directionalLightData;
	PointLightData			pointLightData[PointLightMax];	// 点光源情報
	SpotLightData			spotLightData[SpotLightMax];	// スポットライト情報
	int						pointLightCount = 0;			// 点光源数
	int						spotLightCount = 0;				// スポットライト数

	// 色調補正情報
	ColorGradingData		colorGradingData;

	// ガウスフィルター情報
	GaussianFilterData		gaussianFilterData;

	// 高輝度抽出用情報
	LuminanceExtractionData	luminanceExtractionData;

	// 最終パス情報
	FinalpassData			finalpassData;

	// シャドウマップ情報
	ShadowMapData			shadowMapData;

	// トゥーンシェーディング
	ToonData				toonData;


};
