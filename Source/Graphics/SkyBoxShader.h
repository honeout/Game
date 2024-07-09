#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "Graphics/Shader.h"

// スプライト
class SkyBoxShader : public SpriteShader
{
public:
	SkyBoxShader(ID3D11Device* device);
	~SkyBoxShader() override{}

	struct Vertex
	{
		DirectX::XMFLOAT3	position;
	};


	struct CbScene
	{
		DirectX::XMFLOAT4X4 inverseViewProjection;
		DirectX::XMFLOAT4 viewPosition;

	};

	void Begin(const RenderContext& rc) override;
	void Draw(const RenderContext& rc, const Sprite* sprite) override;
	void End(const RenderContext& rc) override;



private:

	Microsoft::WRL::ComPtr<ID3D11Buffer>			sceneConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			inputLayout;


	Microsoft::WRL::ComPtr<ID3D11BlendState>			blendState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>			samplerState;

};