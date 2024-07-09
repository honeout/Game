#include "SceneManager.h"
#include "SceneGame.h"
#include "SceneLoading.h"
#include "SceneGameOver.h"
#include "SceneTitle.h"
#include "Input/Input.h"
#include "Graphics/Graphics.h"
#include "Player.h"


void SceneGameOver::Initialize()
{

    texts[0] = std::make_unique<Sprite>("Data\\Font\\fonts\\font6.png");
    texts[1] = std::make_unique<Sprite>("Data\\Font\\fonts\\font6.png");
    texts[2] = std::make_unique<Sprite>("Data\\Font\\fonts\\font6.png");

    bg = std::make_unique<Sprite>("Data/Texture/GAMEOVER03.jpg");

    GameOver_Bgm = Audio::Instance().LoadAudioSource("Data/Audio/Bgm/Scene/GameOver.wav");
    GameOver_Bgm->Play(true, 1.0f);

    pos = { 1280 * 0.25f, 720 * 0.7f };
    screentime = 0.0f;
}

void SceneGameOver::Finalize()
{
    GameOver_Bgm->Stop();
}

void SceneGameOver::Update(float elapsedTime)
{
    screentime++;

    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();
    Graphics& graphics = Graphics::Instance();

    //なにかボタンを押したらローディングシーンへ切り替え
    const GamePadButton anyButton =
        GamePad::BTN_A
        | GamePad::BTN_B
        | GamePad::BTN_X
        | GamePad::BTN_Y;


    float screenWidth = static_cast<float>(graphics.GetScreenWidth());
    float screenHeight = static_cast<float>(graphics.GetScreenHeight());

    isTryagain = false;
    isMenu = false;

    if (GetAsyncKeyState('A') & 0x8001)
    {
        pos = { screenWidth * 0.25f, screenHeight * 0.7f };
    }
    if (GetAsyncKeyState('D') & 0x8001)
    {
        pos = { screenWidth * 0.75f, screenHeight * 0.7f };
    }

    if (pos.x == screenWidth * 0.25f)
    {
        isTryagain = true;
        if ((mouse.GetButtonDown() & Mouse::BTN_LEFT || GetAsyncKeyState(VK_RETURN) & 0x8001 || gamePad.GetButtonDown() & anyButton) && screentime >= 120.0f)
        {
            SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
        }
    }

    if (pos.x == screenWidth * 0.75f)
    {
        isMenu = true;
        if ((mouse.GetButtonDown() & Mouse::BTN_LEFT || GetAsyncKeyState(VK_RETURN) & 0x8001 || gamePad.GetButtonDown() & anyButton) && screentime >= 120.0f)
        {
            SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
        }
    }
}

void SceneGameOver::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    //画面クリア画＆レンダーターゲット設定
    FLOAT color[] = { 0.0f,0.0f,0.0f,1.0f };
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    float screenWidth = static_cast<float>(graphics.GetScreenWidth());
    float screenHeight = static_cast<float>(graphics.GetScreenHeight());

    bg->Render(dc, 0, 0, screenWidth, screenHeight, 0, 0, 1920, 1080, 0, 1, 1, 1, 1);

    //texts[0]->textout(dc, "You Died", 120, 340, 48, 48, 1, 1, 1, 1);

    {
        if (isTryagain)
        {
            texts[1]->textout(dc, "END", 1200, 700, 64, 64, 1, 1, 1, 0.6f);
        }
        else
        {
            texts[1]->textout(dc, "END", 1200, 700, 64, 64, 1, 1, 1, 1);
        }

        if (isMenu)
        {
            texts[1]->textout(dc, "CONTINUE", 250, 700.0f, 64, 64, 1, 1, 1, 0.6f);
        }
        else
        {
            texts[1]->textout(dc, "CONTINUE", 250, 700.0f, 64, 64, 1, 1, 1, 1);
        }
    }
}
