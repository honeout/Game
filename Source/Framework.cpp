#include <memory>
#include <sstream>

#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "Framework.h"
#include "Camera.h"
#include "EffectManager.h"
#include "SceneTitle.h"
#include "SceneManager.h"

// 垂直同期間隔設定
static const int syncInterval = 1;

// キーボードフックプロシージャ
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* pKeyboard = (KBDLLHOOKSTRUCT*)lParam;
        // VK_CONVERT (半角/全角キー) が押されたかチェック
        if (pKeyboard->vkCode == VK_CONVERT) {
            // キーイベントを無視
            return 1;
        }
    }
    // 他のキーイベントはシステムに渡す
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

HHOOK hHook = NULL;

// キーボードフックコールバック関数
LRESULT CALLBACK KeyboardProc3(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;

        // 半角/全角キーの仮想キーコード
        if (p->vkCode == VK_CONVERT || p->vkCode == VK_NONCONVERT) {
            // キーイベントを無視
            return 1;
        }
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

// フックを設定する
void SetHook() {
    if (!(hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc3, NULL, 0))) {
        MessageBox(NULL, L"フックの設定に失敗しました。", L"エラー", MB_ICONERROR);
    }
}

// フックを解除する
void ReleaseHook() {
    if (hHook) {
        UnhookWindowsHookEx(hHook);
        hHook = NULL;
    }
}

// 課題内容
void DrawTaskGUI()
{
    ImGui::SetNextWindowPos(ImVec2(970, 10), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(300, 150), ImGuiCond_Once);

    if (ImGui::Begin("UNIT.01", nullptr, ImGuiWindowFlags_None))
    {
        bool check = true;
        ImGui::Checkbox(u8"なにか表示する", &check);
        ImGui::Checkbox(u8"マウス操作でカメラ操作", &check);
    }
    ImGui::End();
}

// コンストラクタ
Framework::Framework(HWND hWnd)
    : hWnd(hWnd)
    , input(hWnd)
    , graphics(hWnd)
{        
    // フックを設定
    SetHook();
    // フックを設定
    HHOOK hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
    // シーン初期化
    //scene	= new SceneGame();
    //scene->Initialize();

    //エフェクトマネージャー初期化
    EffectManager::Instance().Initialize();

    //シーン初期化
    SceneManager::Instance().ChangeScene(new SceneTitle);

}

// デストラクタ
Framework::~Framework()
{
    //if (scene != nullptr)
    //{
    //	scene->Finalize();
    //	delete scene;
    //	scene = nullptr;
    //}

    //シーン終了化
    SceneManager::Instance().Clear();

    //エフェクトマネージャー終了化
    EffectManager::Instance().Finalize();    
    
    // フックを解除
    UnhookWindowsHookEx(hHook);

    // フックを解除
    ReleaseHook();
}

// 更新処理
void Framework::Update(float elapsedTime/*Elapsed seconds from last frame*/)
{
    // 入力更新処理
    input.Update();

    //シーン更新処理
    SceneManager::Instance().Update(elapsedTime);

    // カメラコントローラー更新処理
    //cameraController.Update(elapsedTime);
    // シーン更新処理
    //scene->Update(elapsedTime);

}

// 描画処理
void Framework::Render(float elapsedTime/*Elapsed seconds from last frame*/)
{
    // 別スレッド中にデバイスコンテキストが使われていた場合に
    // 同時アクセスしないように排他制御する
    std::lock_guard<std::mutex> lock(graphics.GetMutex());

    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    Camera& camera = Camera::Instance();

    // IMGUIフレーム開始処理
    graphics.GetImGuiRenderer()->NewFrame();

    // シーン描画処理
    //scene->Render();
    SceneManager::Instance().Render();

    // 課題内容描画
    //DrawTaskGUI();

    // IMGUIデモウインドウ描画（IMGUI機能テスト用）
    //ImGui::ShowDemoWindow();

    // IMGUI描画
    graphics.GetImGuiRenderer()->Render(dc);

    // バックバッファに描画した画を画面に表示する。
    graphics.GetSwapChain()->Present(syncInterval, 0);
}

// フレームレート計算
void Framework::CalculateFrameStats()
{
    // Code computes the average frames per second, and also the
    // average time it takes to render one frame.  These stats
    // are appended to the window caption bar.
    static int frames = 0;
    static float time_tlapsed = 0.0f;

    frames++;

    //// Compute averages over one second period.
    //if ((timer.TimeStamp() - time_tlapsed) >= 1.0f)
    //{
    //    float fps = static_cast<float>(frames); // fps = frameCnt / 1
    //    float mspf = 1000.0f / fps;
    //    std::ostringstream outs;
    //    outs.precision(6);
    //    outs << "FPS : " << fps << " / " << "Frame Time : " << mspf << " (ms)";
    //    SetWindowTextA(hWnd, outs.str().c_str());

    //    // Reset for next average.
    //    frames = 0;
    //    time_tlapsed += 1.0f;
    //}
    std::ostringstream outs;
    outs << "GUARDIAN OF THE JEWEL";
}

// アプリケーションループ
int Framework::Run()
{
    MSG msg = {};

    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            timer.Tick();
            CalculateFrameStats();

            float elapsedTime = syncInterval == 0
                ? timer.TimeInterval()
                : syncInterval / 60.0f
                ;
            Update(elapsedTime);
            Render(elapsedTime);
        }
    }
    return static_cast<int>(msg.wParam);
}

// メッセージハンドラ
LRESULT CALLBACK Framework::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (Graphics::Instance().GetImGuiRenderer()->HandleMessage(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc;
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_CREATE:
        break;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) PostMessage(hWnd, WM_CLOSE, 0, 0);
        break;
    case WM_ENTERSIZEMOVE:
        // WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
        timer.Stop();
        break;
    case WM_EXITSIZEMOVE:
        // WM_EXITSIZEMOVE is sent when the user releases the resize bars.
        // Here we reset everything based on the new window dimensions.
        timer.Start();
        break;
    case WM_MOUSEWHEEL:
        Input::Instance().GetMouse().SetWheel(GET_WHEEL_DELTA_WPARAM(wParam));
        break;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}
