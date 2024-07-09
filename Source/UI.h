#pragma once
#include "Graphics/Sprite.h"
#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include <memory>
using namespace DirectX;

enum class WeponState
{
    Dual,
    Great,
    Gun
};

class UI
{
public:
    static UI& Instance()
    {
        static UI instance;
        return instance;
    }

    UI() {}

    ~UI() {}

    void Initialize();

    void Update(float elapsedTime);
    void UpdateMovie(float elapsedTime);

    void RenderUI(ID3D11DeviceContext* dc/*, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection*/);

    void Clear();

    void DrawDebugGUI();

    void Lit(
        float max,            // �ő�l
        float min,            // �ŏ��l
        float increase,     // �����Ă����l
        float randMax = 0,  // �V�����ő�l
        float randMin = 0   // �V�����ŏ��l

    );

    bool TimeUp() { return timeUp; }
    float GetTime() { return CountDownTime; }


private:

    std::unique_ptr<Sprite> Timer;
    std::unique_ptr<Sprite> Enemy_Hp;
    std::unique_ptr<Sprite> Player_Hp;
    std::unique_ptr<Sprite> Enemy_HpBar;
    std::unique_ptr<Sprite> Player_HpBar;
    std::unique_ptr<Sprite> Kersor;
    std::unique_ptr<Sprite> Game_UI;
    std::unique_ptr<Sprite> WeponDual;
    std::unique_ptr<Sprite> WeponGreat;
    std::unique_ptr<Sprite> WeponGun;

    std::unique_ptr<Sprite> WeponRightClickDual;
    std::unique_ptr<Sprite> WeponRightClickGreat;
    std::unique_ptr<Sprite> WeponRightClickGun;
    // �����Ă���
    std::unique_ptr<Sprite> ClickMode;
    std::unique_ptr<Sprite> ClickMouseMode;


    float CountDownTime;     //180�b
    bool isClear = false;

    //�e�L�X�g�p�X�v���C�g
    std::unique_ptr <Sprite> texts[8];
    float angle = 0.0f;

    DirectX::XMFLOAT2 Uipos = { 24,30 };
    DirectX::XMFLOAT2 UiposHP = { 50,50 };

    float alpha = 1;
    float alphablinking = 1;
    float alphaspace = 1;

    bool switching = true;     // �؂�ւ�

    bool timeUp = false;
};