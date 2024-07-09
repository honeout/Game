#pragma once

#include "Graphics/Model.h"
#include "Graphics/Sprite.h"
#include "Graphics/Texture.h"
#include "Graphics/RenderTarget.h"
#include "Graphics/DepthStencil.h"
#include "Light.h"
#include "PostprocessingRenderer.h"
#include "CameraController.h"
#include "Scene.h"
#include "Weapon.h"
#include "Audio/Audio.h"

// �Q�[���V�[��
class SceneGame : public Scene
{
public:
    SceneGame() {}
    ~SceneGame() override {}

    // ������
    void Initialize() override;

    // �I����
    void Finalize() override;

    // �X�V����
    void Update(float elapsedTime) override;

    // �`�揈��
    void Render() override;

    // �`�揈��
    void DrawDebugParameter(DirectX::XMFLOAT4X4& transform, const char* label);

    // �O���b�h�`��
    void DrawGrid(ID3D11DeviceContext* context, int subdivisions, float scale);

private:
    // 3D��Ԃ̕`��
    void Render3DScene();

    // �V���h�E�}�b�v�̕`��
    void RenderShadowmap();

    // �[�x�̕`��
    void RenderDepth();

private:
    //std::unique_ptr<Model>	jummo;
    //std::unique_ptr<Model>	uncle;
    //std::unique_ptr<Model>	stage;
    //DirectX::XMFLOAT4X4		jummo_transform = {1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1};
    //DirectX::XMFLOAT4X4		uncle_transform = {1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1};
    //DirectX::XMFLOAT4X4		stage_transform = {1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1};
    std::unique_ptr<Sprite>	sprite;
    std::unique_ptr<Sprite>	fadeout;
    std::unique_ptr<Texture> texture;
    CameraController* cameraController = nullptr;
    Sprite* gauge = nullptr;
    
    // UV �X�N���[���f�[�^
    UVScrollData			uvScrollData;

    // �}�X�N�摜
    std::unique_ptr<Texture>	maskTexture;
    //Texture*	maskTexture;
    float						dissolveThreshold;
    float						edgeThreshold;		// ����臒l
    DirectX::XMFLOAT4			edgeColor;			// ���̐F

    // �I�t�X�N���[�������_�����O�p�`��^�[�Q�b�g
    std::unique_ptr<RenderTarget> renderTarget;

    // �V���h�E�}�b�v�p���
    Light* mainDirectionalLight = nullptr;					// �V���h�E�}�b�v�𐶐����镽�s����
    DirectX::XMFLOAT3 DirectionLight = { 0,0,0 }; //�@�V���h�E�}�b�v�̖@�� 
    //std::unique_ptr<DepthStencil> shadowmapDepthStencil;	// �V���h�E�}�b�v�p�[�x�X�e���V���o�b�t�@
    //float shadowDrawRect = 500.0f;							// �V���h�E�}�b�v�ɕ`�悷��͈�
    //DirectX::XMFLOAT4X4 lightViewProjection;				// ���C�g�r���[�v���W�F�N�V�����s��
    //DirectX::XMFLOAT3 shadowColor = { 0.2f, 0.2f, 0.2f };	// �e�̐F
    //float shadowBias = 0.001f;								// �[�x��r�p�̃I�t�Z�b�g�l

    //	�V���h�E�}�b�v�p�[�x�X�e���V���o�b�t�@
    std::unique_ptr<DepthStencil>	shadowmapDepthStencil[ShadowmapCount];
    //	���C�g�r���[�v���W�F�N�V�����s��
    DirectX::XMFLOAT4X4				lightViewProjection[ShadowmapCount];
    //	�[�x��r�p�̃I�t�Z�b�g�l
    //TODO �o�C�A�X�̕ύX
    //�ׂ��������炨���炭�d���Ȃ�
    float							shadowBias[ShadowmapCount] = { 0.0001f, 0.002f, 0.004f, 0.01f };
    //	�e�̐F
    DirectX::XMFLOAT3				shadowColor = { 0.2f, 0.2f, 0.2f };

    // �|�X�g�v���Z�X
    std::unique_ptr<PostprocessingRenderer> postprocessingRenderer;
    // SkyBox�e�N�X�`���[
    std::unique_ptr<Texture> skyboxTexture;

    // Toon�[�x�����_�����O�p�`��^�[�Q�b�g
    std::unique_ptr<DepthStencil> depthStencil;

    // �g�D�[�����
    ToonData			toonData;

    // 2D�p��ʂɓn���ׂ̃J�����ڐ���
    DirectX::XMFLOAT4X4 view;
    DirectX::XMFLOAT4X4 projection;

    //�}�E�X��^�񒆂ɖ߂����߂̃^�C��
    float mousetime;

    std::unique_ptr<AudioSource> Game_Bgm;

    bool BgmStart = false;

    float alphafout = 0.0f;

    bool changeangle = false;

};