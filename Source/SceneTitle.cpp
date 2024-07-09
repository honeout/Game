#include "Graphics/Graphics.h"
#include "SceneGame.h"

#include "Input/Input.h"

#include "Scenemanager.h"

#include "SceneTitle.h"
#include "SceneLoading.h"
#include <Framework.h>

//初期化
void SceneTitle::Initialize()
{
    //スプライト初期化
    bg = std::make_unique<Sprite>("Data/Texture/TITLE.png");
    start = std::make_unique<Sprite>("Data/Sprite/play.png");
    quit = std::make_unique<Sprite>("Data/Sprite/quit.png");

    //テキスト初期化
    texts[0] = std::make_unique<Sprite>("Data\\Font\\fonts\\font6.png");
    texts[1] = std::make_unique<Sprite>("Data\\Font\\fonts\\font6.png");
    texts[2] = std::make_unique<Sprite>("Data\\Font\\fonts\\font6.png");

    screentime = 0.0f;

    //BGMの設定
    Title_Bgm = Audio::Instance().LoadAudioSource("Data/Audio/Bgm/Scene/Title.wav");
    Title_Bgm->Play(true, 1.0f);

    pos.x = 1920 * 0.25f;

    
}

//終了化
void SceneTitle::Finalize()
{


    Title_Bgm->Stop();


}

HHOOK keyboardHook;
HHOOK mouseHook;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        KBDLLHOOKSTRUCT* pKeyboard = (KBDLLHOOKSTRUCT*)lParam;
        if (pKeyboard->vkCode == VK_RETURN) {
            // Enterキーが押されたら、Escキーとして処理する
            INPUT input;
            input.type = INPUT_KEYBOARD;
            input.ki.wVk = VK_ESCAPE;
            input.ki.dwFlags = 0;
            input.ki.time = 0;
            input.ki.dwExtraInfo = 0;
            SendInput(1, &input, sizeof(INPUT));
            return 1; // イベントを処理したので、これ以上伝搬しない
        }
    }
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        if (wParam == WM_LBUTTONDOWN) {
            // 左クリックが行われたら、Escキーとして処理する
            INPUT input;
            input.type = INPUT_KEYBOARD;
            input.ki.wVk = VK_ESCAPE;
            input.ki.dwFlags = 0;
            input.ki.time = 0;
            input.ki.dwExtraInfo = 0;
            SendInput(1, &input, sizeof(INPUT));
            return 1; // イベントを処理したので、これ以上伝搬しない
        }
    }
    return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}



//更新処理
void SceneTitle::Update(float elapsedTime)
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
        if ((mouse.GetButtonDown() & Mouse::BTN_LEFT || GetAsyncKeyState(VK_RETURN) & 0x8001 || gamePad.GetButtonDown() & anyButton) && screentime >= 30.0f)
        {
           // SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
            SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
        }
    }

    if (pos.x == screenWidth * 0.75f)
    {
        isMenu = true;    
        if ((mouse.GetButtonDown() & Mouse::BTN_LEFT || GetAsyncKeyState(VK_RETURN) & 0x8001 || gamePad.GetButtonDown() & anyButton) && screentime >= 30.0f)
        {
            exit(0);
        }
    }

}

//描画処理
void SceneTitle::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    //画面クリア＆レンダーターゲット設定
    FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f }; //RGBA(0.0~1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    float screenWidth = static_cast<float>(graphics.GetScreenWidth());
    float screenHeight = static_cast<float>(graphics.GetScreenHeight());

    bg->Render(dc, 0, 0, 1920, 1080, 0, 0, 1920, 1080, 0, 1, 1, 1, 1);

    {
        if (isMenu)
        {
            start->Render(dc, 300, 600, 182, 128, 0, 0, 182, 128, 0, 1, 1, 1, 0.6f);
        }
        else
        {
            start->Render(dc, 300, 600, 182, 128, 0, 0, 182, 128, 0, 1, 1, 1, 1.0f);
        }

        if (isTryagain)
        {
            quit->Render(dc, 800, 600, 178, 128, 0, 0, 178, 128, 0, 1, 1, 1, 0.6f);
        }
        else
        {
            quit->Render(dc, 800, 600, 178, 128, 0, 0, 178, 128, 0, 1, 1, 1, 1);
        }
    }
}
