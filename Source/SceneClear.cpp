#include "SceneClear.h"
#include "Input/Input.h"
#include<Windows.h>
#include "SceneManager.h"
#include "SceneGame.h"
#include "SceneLoading.h"
#include "Graphics/Graphics.h"
#include "SceneLoading.h"
#include "SceneTitle.h"

void SceneClear::Initialize()
{
    Clear_Bgm = Audio::Instance().LoadAudioSource("Data/Audio/Bgm/Scene/Clear.wav");
    Clear_Bgm->Play(true, 1.0f);

    bg = std::make_unique<Sprite>("Data/Texture/GAME_Clear01.jpg");
    texts[0] = std::make_unique<Sprite>("Data\\Font\\fonts\\font6.png");
}

void SceneClear::Finalize()
{
    Clear_Bgm->Stop();
}

void SceneClear::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();
    Graphics& graphics = Graphics::Instance();

    //なにかボタンを押したらローディングシーンへ切り替え
    const GamePadButton anyButton =
        GamePad::BTN_A
        | GamePad::BTN_B
        | GamePad::BTN_X
        | GamePad::BTN_Y;

    if ((mouse.GetButtonDown() & Mouse::BTN_LEFT || GetAsyncKeyState(VK_RETURN) & 0x8001 || gamePad.GetButtonDown() & anyButton))
    {
        SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
    }
}

void SceneClear::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f };
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    float screenWidth = static_cast<float>(graphics.GetScreenWidth());
    float screenHeight = static_cast<float>(graphics.GetScreenHeight());
    float textureWidth = static_cast<float>(bg->GetTextureWidth());
    float textureHeight = static_cast<float>(bg->GetTextureHeight());

    bg->Render(dc,
        0, 0, screenWidth, screenHeight,
        0, 0, textureWidth, textureHeight,
        0,
        1, 1, 1, 1);
}
