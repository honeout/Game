#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>

// スプライト
class Sprite
{
public:

    Sprite();

    Sprite(const char* filename);
    ~Sprite() {}

    struct Vertex
    {
        DirectX::XMFLOAT3	position;
        DirectX::XMFLOAT4	color;
        DirectX::XMFLOAT2	texcoord;
    };

    // バッファ更新
    void Update(
        float dx, float dy,
        float dw, float dh,
        float sx, float sy,
        float sw, float sh,
        float angle,
        float r, float g, float b, float a) const;

    // 描画実行
    void Render(ID3D11DeviceContext* dc,
        float dx, float dy,
        float dw, float dh,
        float sx, float sy,
        float sw, float sh,
        float angle,
        float r, float g, float b, float a) const;

    //円描画
    void RenderCircle(ID3D11DeviceContext* context,
        const DirectX::XMFLOAT2& center, float radius,
        const DirectX::XMFLOAT2& scale, float angle,
        const DirectX::XMFLOAT4& color, int n
    ) const;

    // シェーダーリソースビューの設定
    void SetShaderResourceView(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& srv, int texWidth, int texHeight);

    // 頂点バッファの取得
    const Microsoft::WRL::ComPtr<ID3D11Buffer>& GetVertexBuffer() const { return vertexBuffer; }

    // シェーダーリソースビューの取得
    const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() const { return shaderResourceView; }

    // テクスチャ幅取得
    int GetTextureWidth() const { return textureWidth; }

    // テクスチャ高さ取得
    int GetTextureHeight() const { return textureHeight; }

    //テキスト表示
    void textout(ID3D11DeviceContext* immediate_context, std::string s, float x, float y, float w, float h, float r, float g, float b, float a);

    //タイマー描画
    void Timer(ID3D11DeviceContext* immediate_context, int nums, float x, float y, float w, float h, float r, float g, float b, float a);

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer>				vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>			inputLayout;

    Microsoft::WRL::ComPtr<ID3D11BlendState>			blendState;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState>		rasterizerState;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		depthStencilState;

    Microsoft::WRL::ComPtr<ID3D11SamplerState>			samplerState;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shaderResourceView;

    int textureWidth = 0;
    int textureHeight = 0;
};