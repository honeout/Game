#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "SceneLoading.h"
#include "SceneManager.h"
#include <thread>

//������
void SceneLoading::Initialize()
{
    //�X�v���C�g������
    //sprite = new Sprite("Data/Texture/LoadingIcon.png");
    spritebuckGround = new Sprite("Data/Sprite/load.PNG");
    sprite = new Sprite("Data/Sprite/Loading.png");
    sprite_move = new Sprite("Data/Sprite/Loading_move.png");

    //�X���b�h�J�n
    std::thread thread(LoadingThread, this);

    //�X���b�h�̊Ǘ������
    thread.detach();
}

//�I����
void SceneLoading::Finalize()
{
    //�X�v���C�g�I����
    if (sprite != nullptr)
    {
        delete sprite;
        sprite = nullptr;
    }

    if (sprite_move != nullptr)
    {
        delete sprite_move;
        sprite_move = nullptr;
    }

    if (spritebuckGround != nullptr)
    {
        delete spritebuckGround;
        spritebuckGround = nullptr;
    }
}

//�X�V����
void SceneLoading::Update(float elapsedTime)
{
    constexpr float speed = 180;
    angle += speed * elapsedTime;

    //���̃V�[���̏���������������V�[����؂�ւ���
    if (nextScene->IsReady())
    {
        SceneManager::Instance().ChangeScene(nextScene);
        nextScene = nullptr;
    }
}

//�`�揈��
void SceneLoading::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    //��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
    FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f }; //RGBA(0.0~1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    //�QD�X�v���C�g�`��
    {
        //��ʉE���Ƀ��[�f�B���O�A�C�R����`��
        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        float textureWidth = static_cast<float>(sprite->GetTextureWidth());
        float textureHeight = static_cast<float>(sprite->GetTextureHeight());
        float positionX = screenWidth - textureWidth;
        float positionY = screenHeight - textureHeight;


        // �w�i
        spritebuckGround->Render(dc, 0, 0, screenWidth, screenHeight,
            0, 0,
            1902, 897,
            0,
            1, 1, 1, 1);

        // �񂳂Ȃ�����
        sprite->Render(dc, positionX, positionY, textureWidth, textureHeight,
            0, 0, textureWidth, textureHeight,
            0,
            1, 1, 1, 1);
        // ���ۂɉ񂷉~
        sprite_move->Render(dc, positionX, positionY, textureWidth, textureHeight,
            0, 0, textureWidth, textureHeight,
            angle,
            1, 1, 1, 1);

    }
}

//���[�f�B���O�X�v���b�h
void SceneLoading::LoadingThread(SceneLoading* scene)
{
    //COM�֘A�̏������ŃX���b�h���ɌĂԕK�v������
    // TODO�R�����g�A�E�g�߂��Ȃ��Ƃ����Ȃ�����
    CoInitialize(nullptr);

    //���̃V�[���̏��������s��
    scene->nextScene->Initialize();

    //�X���b�h���I���O��COM�֘A�̏I����
    CoUninitialize();

    //���̃V�[���̏��������ݒ�
    scene->nextScene->SetReady();
}