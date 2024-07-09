#include "UI.h"
#include "EnemyManager.h"
#include "SceneManager.h"
#include "SceneGameOver.h"
#include "SceneClear.h"
#include "SceneLoading.h"
#include "Mathf.h"
#include "PlayerManager.h"

void UI::Initialize()
{
    //フォント設定
    texts[0] = std::make_unique<Sprite>("Data\\Font\\fonts\\font6.png");
    
    Kersor = std::make_unique<Sprite>("Data/Sprite/sights.png");

    Player_HpBar = std::make_unique<Sprite>("Data/Sprite/player_status.png");
    Enemy_HpBar = std::make_unique<Sprite>("Data/Sprite/enemy_status.png");
    Game_UI = std::make_unique<Sprite>("Data/Sprite/UI_base.png");
    // 武器変更時のUI
    WeponDual = std::make_unique<Sprite>("Data/Sprite/sights.png");
    WeponGreat = std::make_unique<Sprite>("Data/Sprite/sights.png");
    WeponGun = std::make_unique<Sprite>("Data/Sprite/sights.png");
    

    // 右クリック用
    WeponRightClickDual = std::make_unique<Sprite>("Data/Sprite/unknown.png");
    WeponRightClickGreat = std::make_unique<Sprite>("Data/Sprite/guard.png");
    WeponRightClickGun = std::make_unique<Sprite>("Data/Sprite/reload.png");

    ClickMode = std::make_unique<Sprite>("Data/Sprite/shadow1.png");
    ClickMouseMode = std::make_unique<Sprite>("Data/Sprite/left_mouse.png");

    // 60で１秒　
    CountDownTime = 60.0f * 180;
   

   

    timeUp = false;
}

void UI::Update(float elapsedTime)
{
    float speed = 180;
    angle += speed * elapsedTime;
    if (angle >= 1800)
    {
        angle = 0;
    }
    alpha = 1;
    CountDownTime--;

    if (CountDownTime <= 0)
    {
        timeUp = true;
       
    }
}

void UI::UpdateMovie(float elapsedTime)
{
    alpha = 0;
    alphablinking = 0;


}

void UI::RenderUI(ID3D11DeviceContext* dc/*, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection*/)
{

    Graphics& graphics = Graphics::Instance();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    //画面クリア＆レンダーターゲット設定
    FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f }; //RGBA(0.0~1.0)
    float screenWidth = static_cast<float>(graphics.GetScreenWidth());
    float screenHeight = static_cast<float>(graphics.GetScreenHeight());

    //2Dスプライト描画
    {
        //変換行列
        PlayerManager& player = PlayerManager::Instance();

        //プレイヤーの体力描画
        // 1.27
        float gaugeWidth = player.GetPlayer(PlayerManager::Instance().GetWeapon())->GetMaxHealth() * PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetWeapon())->GetHealth() * 2.2f;
        float enemyGaugeWidth;
        //全ての敵の頭上にHPゲージを表示

        Game_UI->Render(dc, 0, 0, screenWidth, screenHeight, 0, 0, 1920, 1080, 0, 1, 1, 1, alpha);
        
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();

        for (int i = 0; i < enemyCount; ++i)
        {
            Enemy* enemy = enemyManager.GetEnemy(i);

            enemyGaugeWidth = enemy->GetMaxHealth() * enemy->GetHealth() * 1.93f;

            Enemy_HpBar->Render(dc, 479, 120, enemyGaugeWidth, 6, 0, 0, 300, 55, 0, 1, 0, 0, alpha);
        }

        Player_HpBar->Render(dc, 718, 1032,  gaugeWidth, 6, 0, 0, 300, 55, 0, 1, 1, 1, alpha);
        

        Kersor->Render(dc, screenWidth/2, screenHeight/2, 50, 50, 0, 0, 128, 128, angle, 1, 1, 1, alpha);

        if (CountDownTime >= 6000)
        {
            //タイム描画
            texts[0]->textout(dc, std::to_string(static_cast<int>(CountDownTime / 60)), 40, 198, 30, 30, 1, 1, 1, alpha);
        }
        if (CountDownTime < 6000)
        {
            texts[0]->textout(dc, std::to_string(static_cast<int>(CountDownTime / 60)), 60, 198, 30, 30, 1, 1, 1, alpha);
        }




        // 弾丸足りない
        if (player.GetPlayer(2)->GetAttackCount() <= 5)
        Lit( 1, 0, 0.15f,3,3);
        else
        {
            alphablinking = 1;
        }

        // ボタン表示
        switch (PlayerManager::Instance().GetWeapon())
        {
        case static_cast<int>(WeponState::Dual):
            //WeponDual->Render(dc, 754, 635, 50, 50, 0, 0, 128, 128, 0, 1, 1, 1, 1);
            // 選択中
            WeponDual->Render(dc, 1140, 962, 50, 50, 0, 0, 128, 128, angle, 1, 1, 1, alpha);
            // 右クリックの効果
            WeponRightClickDual->Render(dc, 1680, 909, 50, 50, 0, 0, 49, 49, 0, 1, 1, 1, alpha);

            break;
        case static_cast<int>(WeponState::Great):
            // 選択中
            WeponGreat->Render(dc, 1055, 962, 50, 50, 0, 0, 128, 128, angle, 1, 1, 1, alpha);
            // 右クリックの効果
            WeponRightClickGreat->Render(dc, 1680, 909, 50, 50, 0, 0, 49, 49, 0, 1, 1, 1, alpha);
            break;
        case static_cast<int>(WeponState::Gun):
            // 選択中
            WeponGun->Render(dc, 965, 962, 50, 50, 0, 0, 128, 128, angle, 1, 1, 1, alpha);
            // 右クリックの効果
            WeponRightClickGun->Render(dc, 1680, 909, 50, 50, 0, 0, 49, 49, 0, 1, 1, 1, alphablinking);
            
            break;
        }

        GamePad& gamePad = Input::Instance().GetGamePad();
        Mouse& mouse = Input::Instance().GetMouse();



        // space
        if ((gamePad.GetButtonDown() & GamePad::BTN_A) || (GetAsyncKeyState(VK_SPACE) & 0x8001))
        {
            ClickMode->Render(dc, 1556, 861, 73, 48, 0, 0, 49, 49, 0, 1, 1, 1, alpha);
        }

        // shift
        if ((::GetKeyState(VK_SHIFT) & 0x8000))
        {
            ClickMode->Render(dc, 1798, 967, 70, 48, 0, 0, 49, 49, 0, 1, 1, 1, alpha);
        }




        // leftclick
        if ( (mouse.GetButton() & Mouse::BTN_LEFT))
        {
            
           

            ClickMouseMode->Render(dc, 1555, 967, 36, 51, 0, 0, 31, 46, 0, 1, 1, 1, alpha);
        }
        //TODO 後で戻す
        // rightclick
        if ( (mouse.GetButton() & Mouse::BTN_RIGHT))
        {
            ClickMode->Render(dc, 1680, 909, 50, 50, 0, 0, 49, 49, 0, 1, 1, 1, alpha);
            ClickMouseMode->Render(dc, 1625, 967, -35, 51, 0, 0, 31, 46, 0, 1, 1, 1, alpha);
        }


    }
}

void UI::Clear()
{
    CountDownTime = 60.0f * 180;    //180秒
}

void UI::DrawDebugGUI()
{

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Ui", nullptr, ImGuiWindowFlags_None))
    {
        //位置
        ImGui::SliderFloat("UiX", &Uipos.x, 0, 700);
        ImGui::SliderFloat("UiY", &Uipos.y, 0, 300);

        ImGui::SliderFloat("UiXhp", &UiposHP.x, 700, 1920);
        ImGui::SliderFloat("UiYhp", &UiposHP.y, 300, 1100);
        ImGui::SliderFloat("alphablinking", &alphablinking, 0, 1);
        
    }
    ImGui::End();

}

void UI::Lit(float max, float min, float increase,  float randMax, float randMin)
{
    
    
    // 最大値までいったら逆転
    if (alphablinking >= randMax )
    {
        switching = false;
        randMin = Mathf::RandomRange(  0.0f, min);
        alphablinking = randMax;
        

    }
    // 最低値までいったら逆転
    else if (alphablinking <=-randMin )
    {
        switching = true;
        randMax = Mathf::RandomRange( 0.0f , max);
        alphablinking = -randMin;
    }


    // 揺れる
    if (switching)
    {
        alphablinking += increase;
    }
    if (!switching)
    {
        alphablinking -= increase;
    }


}
