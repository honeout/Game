#pragma once

#include <memory>
#include <DirectXMath.h>

//#include "Graphics/ModelResource.h"
#include "Camera.h"
#include "CameraController.h"
#include "Graphics\Model.h"


// 剣の軌跡処理シーン
class SwordTrailScene 
{
public:



	//唯一のインスタンス取得
	static SwordTrailScene& Instance()
	{
		static SwordTrailScene instance;
		return instance;
	}

	SwordTrailScene() {}
	~SwordTrailScene() {}

	// 初期化
	void Initialize();

	// 更新処理
	void Update(float elapsedTime) ;

	// 描画処理
	//void Render(Camera& camera , ID3D11DeviceContext* dc) ;
	void Render(Camera& camera , ID3D11DeviceContext* dc/*, ID3D11RenderTargetView* render, ID3D11DepthStencilView* dsv*//*, ID3D11BlendState* bl*/) ;
	//void Render() ;

	// GUI描画処理
	void DrawGUI() ;

private:
	Model* model = nullptr;
	//Camera& camera;
	bool								spline = false;

	static const int MAX_POLYGON = 32;
	DirectX::XMFLOAT3					trailPositions[2][MAX_POLYGON];


	DirectX::XMFLOAT3 previouspos = {};
	int                                 vposition = -1;

	float                               t = 0.5f;

	Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blendStates[1];
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates[1];
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizerStates[1];
	

	DirectX::XMFLOAT3 wpos;
	DirectX::XMFLOAT3 wposL;
};
