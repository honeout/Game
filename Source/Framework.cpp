#include <memory>
#include <sstream>

#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "Framework.h"
#include "Camera.h"
#include "EffectManager.h"
#include "SceneTitle.h"
#include "SceneManager.h"

// ���������Ԋu�ݒ�
static const int syncInterval = 1;

// �L�[�{�[�h�t�b�N�v���V�[�W��
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* pKeyboard = (KBDLLHOOKSTRUCT*)lParam;
        // VK_CONVERT (���p/�S�p�L�[) �������ꂽ���`�F�b�N
        if (pKeyboard->vkCode == VK_CONVERT) {
            // �L�[�C�x���g�𖳎�
            return 1;
        }
    }
    // ���̃L�[�C�x���g�̓V�X�e���ɓn��
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

HHOOK hHook = NULL;

// �L�[�{�[�h�t�b�N�R�[���o�b�N�֐�
LRESULT CALLBACK KeyboardProc3(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;

        // ���p/�S�p�L�[�̉��z�L�[�R�[�h
        if (p->vkCode == VK_CONVERT || p->vkCode == VK_NONCONVERT) {
            // �L�[�C�x���g�𖳎�
            return 1;
        }
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

// �t�b�N��ݒ肷��
void SetHook() {
    if (!(hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc3, NULL, 0))) {
        MessageBox(NULL, L"�t�b�N�̐ݒ�Ɏ��s���܂����B", L"�G���[", MB_ICONERROR);
    }
}

// �t�b�N����������
void ReleaseHook() {
    if (hHook) {
        UnhookWindowsHookEx(hHook);
        hHook = NULL;
    }
}

// �ۑ���e
void DrawTaskGUI()
{
    ImGui::SetNextWindowPos(ImVec2(970, 10), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(300, 150), ImGuiCond_Once);

    if (ImGui::Begin("UNIT.01", nullptr, ImGuiWindowFlags_None))
    {
        bool check = true;
        ImGui::Checkbox(u8"�Ȃɂ��\������", &check);
        ImGui::Checkbox(u8"�}�E�X����ŃJ��������", &check);
    }
    ImGui::End();
}

// �R���X�g���N�^
Framework::Framework(HWND hWnd)
    : hWnd(hWnd)
    , input(hWnd)
    , graphics(hWnd)
{        
    // �t�b�N��ݒ�
    SetHook();
    // �t�b�N��ݒ�
    HHOOK hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
    // �V�[��������
    //scene	= new SceneGame();
    //scene->Initialize();

    //�G�t�F�N�g�}�l�[�W���[������
    EffectManager::Instance().Initialize();

    //�V�[��������
    SceneManager::Instance().ChangeScene(new SceneTitle);

}

// �f�X�g���N�^
Framework::~Framework()
{
    //if (scene != nullptr)
    //{
    //	scene->Finalize();
    //	delete scene;
    //	scene = nullptr;
    //}

    //�V�[���I����
    SceneManager::Instance().Clear();

    //�G�t�F�N�g�}�l�[�W���[�I����
    EffectManager::Instance().Finalize();    
    
    // �t�b�N������
    UnhookWindowsHookEx(hHook);

    // �t�b�N������
    ReleaseHook();
}

// �X�V����
void Framework::Update(float elapsedTime/*Elapsed seconds from last frame*/)
{
    // ���͍X�V����
    input.Update();

    //�V�[���X�V����
    SceneManager::Instance().Update(elapsedTime);

    // �J�����R���g���[���[�X�V����
    //cameraController.Update(elapsedTime);
    // �V�[���X�V����
    //scene->Update(elapsedTime);

}

// �`�揈��
void Framework::Render(float elapsedTime/*Elapsed seconds from last frame*/)
{
    // �ʃX���b�h���Ƀf�o�C�X�R���e�L�X�g���g���Ă����ꍇ��
    // �����A�N�Z�X���Ȃ��悤�ɔr�����䂷��
    std::lock_guard<std::mutex> lock(graphics.GetMutex());

    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    Camera& camera = Camera::Instance();

    // IMGUI�t���[���J�n����
    graphics.GetImGuiRenderer()->NewFrame();

    // �V�[���`�揈��
    //scene->Render();
    SceneManager::Instance().Render();

    // �ۑ���e�`��
    //DrawTaskGUI();

    // IMGUI�f���E�C���h�E�`��iIMGUI�@�\�e�X�g�p�j
    //ImGui::ShowDemoWindow();

    // IMGUI�`��
    graphics.GetImGuiRenderer()->Render(dc);

    // �o�b�N�o�b�t�@�ɕ`�悵�������ʂɕ\������B
    graphics.GetSwapChain()->Present(syncInterval, 0);
}

// �t���[�����[�g�v�Z
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

// �A�v���P�[�V�������[�v
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

// ���b�Z�[�W�n���h��
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
