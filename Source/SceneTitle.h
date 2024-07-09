#pragma once


#include "Graphics/Sprite.h"
#include "Graphics/Texture.h"
#include "Graphics/RenderTarget.h"
#include "Graphics/DepthStencil.h"

#include "Scene.h"
#include "Audio/Audio.h"

//�^�C�g���V�[��
class SceneTitle : public Scene
{
public:
    SceneTitle() {}
    ~SceneTitle()override {}

    //������
    void Initialize()override;

    //�I����
    void Finalize()override;

    //�X�V����
    void Update(float elapsedTime) override;

    //�`�揈��
    void Render() override;

private:
    //CameraController* cameraController = nullptr;
    std::unique_ptr<Sprite> bg;    
    std::unique_ptr<Sprite> start;
    std::unique_ptr<Sprite> quit;

    // Toon�[�x�����_�����O�p�`��^�[�Q�b�g
    std::unique_ptr<DepthStencil> depthStencil;

    // �g�D�[�����
    //ToonData			toonData;

    bool isTryagain = false;
    bool isMenu = false;
    std::unique_ptr <Sprite> texts[8];

    DirectX::XMFLOAT2 pos;
    float screentime = 0.0f;

    std::unique_ptr<AudioSource> Title_Bgm;

};