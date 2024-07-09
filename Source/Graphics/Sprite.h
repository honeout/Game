#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>

// �X�v���C�g
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

    // �o�b�t�@�X�V
    void Update(
        float dx, float dy,
        float dw, float dh,
        float sx, float sy,
        float sw, float sh,
        float angle,
        float r, float g, float b, float a) const;

    // �`����s
    void Render(ID3D11DeviceContext* dc,
        float dx, float dy,
        float dw, float dh,
        float sx, float sy,
        float sw, float sh,
        float angle,
        float r, float g, float b, float a) const;

    //�~�`��
    void RenderCircle(ID3D11DeviceContext* context,
        const DirectX::XMFLOAT2& center, float radius,
        const DirectX::XMFLOAT2& scale, float angle,
        const DirectX::XMFLOAT4& color, int n
    ) const;

    // �V�F�[�_�[���\�[�X�r���[�̐ݒ�
    void SetShaderResourceView(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& srv, int texWidth, int texHeight);

    // ���_�o�b�t�@�̎擾
    const Microsoft::WRL::ComPtr<ID3D11Buffer>& GetVertexBuffer() const { return vertexBuffer; }

    // �V�F�[�_�[���\�[�X�r���[�̎擾
    const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() const { return shaderResourceView; }

    // �e�N�X�`�����擾
    int GetTextureWidth() const { return textureWidth; }

    // �e�N�X�`�������擾
    int GetTextureHeight() const { return textureHeight; }

    //�e�L�X�g�\��
    void textout(ID3D11DeviceContext* immediate_context, std::string s, float x, float y, float w, float h, float r, float g, float b, float a);

    //�^�C�}�[�`��
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