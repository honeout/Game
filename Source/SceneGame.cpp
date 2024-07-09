#include "Graphics/Graphics.h"
#include "SceneGame.h"
#include "Camera.h"
#include "LightManager.h"
#include "EnemyManager.h"
#include "PlayerManager.h"
#include "PlayerRobot.h"
#include "PlayerDual.h"
#include "PlayerGreat.h"
#include "PlayerGun.h"
#include "PlayerMaster.h"
#include "EnemyBOSS.h"
#include "EffectManager.h"
#include "Input/Input.h"
#include "StageManager.h"
#include "StageMain.h"
//#include "StageMove.h"
#include "SceneGameOver.h"
#include "SceneClear.h"
#include "Scenemanager.h"
//#include "StageWall.h"
#include "Weapon.h"
#include "UI.h"

// シャドウマップのサイズ
static const UINT SHADOWMAP_SIZE = 2048;

// 初期化
void SceneGame::Initialize()
{
    //BGM初期化
    Game_Bgm = Audio::Instance().LoadAudioSource("Data/Audio/Bgm/Scene/Game_01.wav");
    //Game_Bgm = Audio::Instance().LoadAudioSource("Data/Audio/Bgm/Scene/Game_02.wav");
    BgmStart = false;

    //ステージ初期化
    StageManager& stageManager = StageManager::Instance();
    StageMain* stageMain = new StageMain();
    stageManager.Register(stageMain);

    //プレイヤー初期化
    PlayerManager& playerManager = PlayerManager::Instance();
    //PlayerRobot* robot = new PlayerRobot();
    //playerManager.Register(robot);

    PlayerDual* dual = new PlayerDual();
    dual->SetPosition({0, 0, 33});

    dual->SetAngle({0,DirectX::XMConvertToRadians(165),0});
    playerManager.Register(dual);

    PlayerGreat* great = new PlayerGreat();
    playerManager.Register(great);

    PlayerGun* gun = new PlayerGun();
    playerManager.Register(gun);

    PlayerMaster* master = new PlayerMaster();
    playerManager.Register(master);


    playerManager.SetWeapon(0);


    // 記録カメラ
    //DirectX::XMFLOAT3(0, 75, 75),	//視点
    //カメラ初期設定
    Graphics& graphics = Graphics::Instance();
    Camera& camera = Camera::Instance();
    camera.SetLookAt(
        DirectX::XMFLOAT3(0, 75, 75),	//視点
        DirectX::XMFLOAT3(0, 0, 0),		//注視点
        DirectX::XMFLOAT3(0, 1, 0)		//上方向
    );
    camera.SetPerspectiveFov(
        DirectX::XMConvertToRadians(45),							//画角
        graphics.GetScreenWidth() / graphics.GetScreenHeight(),		//画面比率
        1.0f,														//クリップ距離
        10000.0f														//〃
    );

    //カメラコントローラー初期化
    cameraController = new CameraController();


    //エネミー初期化
        EnemyManager& enemyManager = EnemyManager::Instance();


    //#if 0
    //    EnemySlime* slime = new EnemySlime();
    //    slime->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 5));
    //    enemyManager.Register(slime);
    //#else
    //#endif

        //2人以上の敵
        //for (int i = 0; i < 10; ++i)
        //{
        //    EnemySlime* slime = new EnemySlime();
        //    slime->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 5));
        //    slime->SetTerritory(slime->GetPosition(), 10.0f);
        //    enemyManager.Register(slime);
        //}

        //１人の敵
        EnemyBOSS* Boss = new EnemyBOSS();
        Boss->SetPosition(DirectX::XMFLOAT3(2.0f, 5, 5));
        Boss->SetTerritory(Boss->GetPosition(), 10.0f);
        enemyManager.Register(Boss);

    //ゲージスプライト
    //gauge = new Sprite();

    // マスクテクスチャの読み込み
    maskTexture = std::make_unique<Texture>("Data/Texture/dissolve_animation2.png");
    //maskTexture = new Texture("Data/Texture/dissolve_animation2.png");

    dissolveThreshold = 0.0f;
    edgeThreshold = 0.2f;		// 縁の閾値
    edgeColor = { 1,0,0,1 };	// 縁の色

    // 平行光源を追加
    {
        mainDirectionalLight = new Light(LightType::Directional);
        mainDirectionalLight->SetPosition({ 0,-200,0 });
        mainDirectionalLight->SetDirection({ 1, -1, -1 });
        LightManager::Instance().Register(mainDirectionalLight);
    }

    // 点光源を追加
    {
        Light* light = new Light(LightType::Point);
        light->SetPosition(DirectX::XMFLOAT3(20, 10, 0));
        light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
        LightManager::Instance().Register(light);
    }

    // スポットライトの追加
    {
        Light* light = new Light(LightType::Spot);
        light->SetPosition(DirectX::XMFLOAT3(-30, 20, 0));
        light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
        light->SetDirection(DirectX::XMFLOAT3(+1, -1, 0));
        light->SetRange(40.0f);
        LightManager::Instance().Register(light);
    }


    // 新しい描画ターゲットの生成
    {
        Graphics& graphics = Graphics::Instance();
        renderTarget = std::make_unique<RenderTarget>(
            static_cast<UINT>(graphics.GetScreenWidth()),
            static_cast<UINT>(graphics.GetScreenHeight()),
            DXGI_FORMAT_R8G8B8A8_UNORM);
    }

    // シャドウマップ用に深度ステンシルの生成
    {
        //Graphics& graphics = Graphics::Instance();
        //shadowmapDepthStencil = std::make_unique<DepthStencil>(SHADOWMAP_SIZE, SHADOWMAP_SIZE);
        for (auto& it : shadowmapDepthStencil)
        {
            it = std::make_unique<DepthStencil>(SHADOWMAP_SIZE, SHADOWMAP_SIZE);
        }

    }

    // ポストプロセス描画クラス生成
    {
        postprocessingRenderer = std::make_unique<PostprocessingRenderer>();
        // シーンテクスチャを設定しておく
        ShaderResourceViewData srvData;
        srvData.srv = renderTarget->GetShaderResourceView();
        srvData.width = renderTarget->GetWidth();
        srvData.height = renderTarget->GetHeight();
        postprocessingRenderer->SetSceneData(srvData);
    }
    // スカイボックステクスチャの読み込み
    {
        Graphics& graphics = Graphics::Instance();
        //skyboxTexture = std::make_unique<Texture>("Data/Texture/1920px-Equirectangular-projection.jpg");
        //skyboxTexture = std::make_unique<Texture>("Data/SkyBox/vei9u5pws3i91.png");
        //skyboxTexture = std::make_unique<Texture>("Data/SkyBox/beautiful_sky.jpg");
        //skyboxTexture = std::make_unique<Texture>("Data/SkyBox/field20220706.jpg");
        //skyboxTexture = std::make_unique<Texture>("Data/SkyBox/Sky.jpg");
        //skyboxTexture = std::make_unique<Texture>("Data/SkyBox/sunflowers.jpg");
        //skyboxTexture = std::make_unique<Texture>("Data/SkyBox/torii20220706.jpg");
        //skyboxTexture = std::make_unique<Texture>("Data/SkyBox/town20220706.jpg");

        //sprite = std::make_unique<Sprite>();
        //sprite->SetShaderResourceView(skyboxTexture->GetShaderResourceView(), skyboxTexture->GetWidth(), skyboxTexture->GetHeight());
        //sprite->Update(0, 0, graphics.GetScreenWidth(), graphics.GetScreenHeight(),
        //    0, 0, static_cast<float>(skyboxTexture->GetWidth()), static_cast<float>(skyboxTexture->GetHeight()),
        //    0,
        //    1, 1, 1, 1);
    }
    // UI
    {
        UI::Instance().Initialize();
    }

    {
        Graphics& graphics = Graphics::Instance();
        depthStencil = std::make_unique<DepthStencil>(
            static_cast<UINT>(graphics.GetScreenWidth()),
            static_cast<UINT>(graphics.GetScreenHeight()));
    }
    {
        //// テクスチャを読み込む
        texture = std::make_unique<Texture>("Data/Texture/Title.png");

        // スプライト
        sprite = std::make_unique<Sprite>();
        sprite->SetShaderResourceView(texture->GetShaderResourceView(), texture->GetWidth(), texture->GetHeight());

        fadeout = std::make_unique<Sprite>("Data/Sprite/fadeout.png");

    }

    

    // カメラ変更前のupdate一度通す

    changeangle = false;
}

// 終了化
void SceneGame::Finalize()
{
    //エネミー終了化
    EnemyManager::Instance().Clear();

    //プレイヤー終了化
    PlayerManager::Instance().Clear();

    //LightManager::Instance().Clear();


    //カメラコントローラー終了化
    if (cameraController != nullptr)
    {
        delete cameraController;
        cameraController = nullptr;
    }

    UI::Instance().Clear();


    

    //ステージ終了化
    StageManager::Instance().Clear();

    Game_Bgm->Stop();
}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
    //TODO　強制的にフルスクリーンにする処理
    //Graphics& graphics = Graphics::Instance();
    //graphics.GetSwapChain()->SetFullscreenState(true, nullptr);

    //Bgm開始
    if (BgmStart == false)
    {
        Game_Bgm->Play(true, 0.6f);     //TODO Seのために音量調整をする
        BgmStart = true;
    }
    //ステージ更新処理
    StageManager::Instance().Update(elapsedTime);

    EnemyManager& enemyManager = EnemyManager::Instance();
    int enemyCount = enemyManager.GetEnemyCount();

    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        if (enemy->GetCameraChange())
        {
            //プレイヤー更新処理
            //PlayerManager::Instance().Update(elapsedTime);
            Player* player = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetWeapon());
            player->Update(elapsedTime);
            UI::Instance().Update(elapsedTime);
        }
    }

    if (!PlayerManager::Instance().GetPlayer(3)->GetChangeTimeMode() )
    //エネミー更新処理
    EnemyManager::Instance().Update(elapsedTime);

    //エフェクト更新処理
   EffectManager::Instance().Update(elapsedTime);

    //UI更新処理




    // カメラ
    if (!PlayerManager::Instance().GetPlayer(3)->GetChangeTimeMode())
    {
        DirectX::XMFLOAT3 target = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetWeapon())->GetPosition();
        target.y += 0.5f;										//プレイヤーの腰辺りにターゲットを設定
        cameraController->SetTarget(target);
        cameraController->Update(elapsedTime);
    }


    EnemyManager& enemyManagercamera = EnemyManager::Instance();
    int enemyCountcamera = enemyManagercamera.GetEnemyCount();

    for (int i = 0; i < enemyCountcamera; ++i)
    {
        Enemy* enemy = enemyManagercamera.GetEnemy(i);

        if (!enemy->GetCameraChange())
        {
            //カメラコントローラー更新処理
            DirectX::XMFLOAT3 target = enemy->GetPosition();
            target.y += 3.5f;										//プレイヤーの腰辺りにターゲットを設定
            cameraController->SetTarget(target);

            cameraController->UpdateDemo(elapsedTime);
            // アップ最大限
            if (enemy->GetRastApCamera())
            {
                // アップ減少
                cameraController->CameraDown(elapsedTime);


            }
            else
            {
                cameraController->camerafall(elapsedTime);

            }
            UI::Instance().UpdateMovie(elapsedTime);
            if (enemy->GetFadeOutCamera())
                alphafout += 00.1f;



        }
        else
        {
            alphafout -= 0.1f;
            cameraController->SetRange(7.0f);
            if (alphafout <= 0)
            {
                alphafout = 0;
                changeangle = true;
                enemy->SetStartBoss(true);
            }
        }


        if (changeangle)
        {
            cameraController->SetAngleY(DirectX::XMConvertToRadians(3));
            changeangle = false;
        }
        if (enemy->GetShackCheck())
            cameraController->CameraShake(0.8f, 0.8f, 0.8f, 0.8f, 0.01f);

        //// プレイヤーのカメラ情報を出す。
        //if (PlayerManager::Instance().GetPlayer(3)->GetChangeTimeMode())
        //{
        //    DirectX::XMFLOAT3 target = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetWeapon())->GetPosition();
        //    target.y += 0.5f;										//プレイヤーの腰辺りにターゲットを設定
        //    cameraController->SetTarget(target);
        //    cameraController->UpdateDemo(elapsedTime);
        //    //cameraController->SetCamera_Operate(1);

        //}
        ////else
        ////{
        ////    cameraController->SetCamera_Operate(0);
        ////}
        //if (PlayerManager::Instance().GetPlayer(3)->GetShackCheck())
        //    cameraController->CameraShake(0.8f, 0.8f, 0.8f, 0.8f, 0.01f);

        //
    }



    //if (PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetWeaponFlag())->GetShackStrongCheck())
    //    cameraController->CameraShake(1.0f, 1.0f, 1.0f, 1.0f, 0.01f);

    //// 2Dマスクシェーダ
    //{
    //    Graphics& graphics = Graphics::Instance();
    //    //ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    //    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    //    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    //    float screenWidth = static_cast<float>(graphics.GetScreenWidth());
    //    float screenHeight = static_cast<float>(graphics.GetScreenHeight());

    //    sprite->Update(0, 0,
    //        screenWidth, screenHeight,
    //        0.0f, 0.0f,
    //        static_cast<float>(texture->GetWidth()), static_cast<float>(texture->GetHeight()),
    //        0.0f,
    //        1.0f, 1.0f, 1.0f, 1.0f);


    //
    //}


    // enemy死亡

    if (EnemyManager::Instance().GetDie())
    {
        SceneManager::Instance().ChangeScene(new SceneClear);
    }

    //TODOplayer側で呼ぶ
    for (int i = 0; i < PlayerManager::Instance().GetPlayerCount(); ++i)
    {
        if (PlayerManager::Instance().GetPlayer(i)->GetHealth() <= 0)
        {
            SceneManager::Instance().ChangeScene(new SceneGameOver);
        }
        else if (/*PlayerManager::Instance().GetPlayer(i)->GetPosition().y <= -10.0f ||*/ UI::Instance().TimeUp())
        {
            SceneManager::Instance().ChangeScene(new SceneGameOver);
        }
        
    }

    //if (UI::Instance().GetCountDownTime() <= 0.0f)
    //{
    //    SceneManager::Instance().ChangeScene(new SceneGameOver);
    //}
}

// 描画処理
void SceneGame::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();


    // シャドウマップの描画
    //RenderShadowmap();

    // 深度値描画
    RenderDepth();

    // 3D 空間の描画を別のバッファに対して行う
    Render3DScene();

    // 書き込み先をバックバッファに変えてオフスクリーンレンダリングの結果を描画する
    {
        ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
        ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

        // 画面クリア＆レンダーターゲット設定
        FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
        dc->ClearRenderTargetView(rtv, color);
        dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        dc->OMSetRenderTargets(1, &rtv, dsv);

        // ビューポートの設定
        D3D11_VIEWPORT vp = {};
        vp.Width = graphics.GetScreenWidth();
        vp.Height = graphics.GetScreenHeight();
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        dc->RSSetViewports(1, &vp);

        // ポストプロセス処理を行う
        postprocessingRenderer->Render(dc);

    }

    // 2Dスプライト描画
    {
        UI::Instance().RenderUI(dc);

        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        fadeout->Render(
            dc,
            0, 0,
            screenWidth, screenHeight,
            0.0f, 0.0f,
            static_cast<float>(texture->GetWidth()), static_cast<float>(texture->GetHeight()),
            0,
            1.0f, 1.0f, 1.0f, alphafout);

        //// 描画処理
        //RenderContext rc;
        //rc.deviceContext = dc;

        //SpriteShader* shader = graphics.GetShader(SpriteShaderId::Mask);
        //shader->Begin(rc);

        //rc.maskData.maskTexture = maskTexture->GetShaderResourceView().Get();
        //rc.maskData.dissolveThreshold = dissolveThreshold;
        //rc.maskData.edgeThreshold = edgeThreshold;

        //shader->Draw(rc, sprite.get());

        //shader->End(rc);


    }

    //2Dデバッグ描画
    {

        PlayerManager::Instance().DrawDebugGUI();
    }

    // デバッグ情報の表示
    //TODO リリース時は全部コメントアウトさせる
    //{
    //    cameraController->DrawGUI();
    //    UI::Instance().DrawDebugGUI();
    //    //DrawDebugParameter(jummo_transform, "jummo");
    //    //DrawDebugParameter(uncle_transform, "uncle");
    //    //DrawDebugParameter(stage_transform, "stage");
    //    ImGui::Separator();
    //    if (ImGui::TreeNode("UVScroll"))
    //    {
    //        ImGui::SliderFloat2("Scroll Value", &uvScrollData.uvScrollValue.x, -10.0f, 10.0f);
    //        ImGui::TreePop();
    //    }
    //    ImGui::Separator();
    //    if (ImGui::TreeNode("Alpha"))
    //    {
    //        ImGui::SliderFloat("alpha", &alphafout, -10.0f, 10.0f);
    //        ImGui::TreePop();
    //    }

    //    ImGui::Separator();
    //    if (ImGui::TreeNode("Mask"))
    //    {
    //        ImGui::SliderFloat("Dissolve Threshold", &dissolveThreshold, 0.0f, 1.0f);
    //        ImGui::SliderFloat("Edge Threshold", &edgeThreshold, 0.0f, 1.0f);
    //        ImGui::ColorEdit4("Edge Color", &edgeColor.x);
    //        ImGui::TreePop();
    //    }
    //    ImGui::Separator();

    //    LightManager::Instance().DrawDebugGUI();
    //    ImGui::Separator();

    //    postprocessingRenderer->DrawDebugGUI();

    //    ImGui::Separator();
    //    if (ImGui::TreeNode("Shadowmap"))
    //    {
    //        ImGui::ColorEdit3("Color", &shadowColor.x);
    //        for (int i = 0; i < ShadowmapCount; ++i)
    //        {
    //            ImGui::SliderFloat((std::string("Bias") + std::to_string(i)).c_str(),
    //                &shadowBias[i], 0.0f, 0.1f, "%.5f", 1.0f);
    //        }
    //        ImGui::Text("texture");
    //        for (int i = 0; i < ShadowmapCount; ++i)
    //        {
    //            ImGui::Image(shadowmapDepthStencil[i]->GetShaderResourceView().Get(), { 128, 128 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
    //            ImGui::SameLine();
    //        }

    //        ImGui::TreePop();
    //    }
    //    ImGui::Separator();

    //    if (ImGui::TreeNode("OutLine(Toon)"))
    //    {
    //        ImGui::ColorEdit4("OutLine Color", &toonData.edgeColor.x);
    //        ImGui::SliderFloat("Line Width", &toonData.edgeWidth, 0.0f, 5.0f);
    //        ImGui::Image(depthStencil->GetShaderResourceView().Get(), { 256, 256 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });

    //        ImGui::TreePop();
    //    }

    //}
}

void SceneGame::DrawDebugParameter(DirectX::XMFLOAT4X4& transform, const char* label)
{
    ImGui::PushID(label);
    if (ImGui::TreeNode(label))
    {
        DirectX::XMVECTOR Scale, Rotation, Position;
        DirectX::XMMatrixDecompose(&Scale, &Rotation, &Position, DirectX::XMLoadFloat4x4(&transform));
        DirectX::XMFLOAT3 scale, rotation, position;
        DirectX::XMStoreFloat3(&scale, Scale);
        DirectX::XMStoreFloat3(&rotation, Rotation);
        DirectX::XMStoreFloat3(&position, Position);
        ImGui::SliderFloat3("scale", &scale.x, 0.0f, 10.0f);
        ImGui::SliderFloat3("rotation", &rotation.x, -DirectX::XM_PI, DirectX::XM_PI);
        ImGui::SliderFloat3("position", &position.x, -300.0f, 1000.0f);
        ImGui::SliderFloat("alphafout", &alphafout, -300.0f, 1000.0f);
        DirectX::XMMATRIX Transform;
        Transform = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z)
            * DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z)
            * DirectX::XMMatrixTranslation(position.x, position.y, position.z);
        DirectX::XMStoreFloat4x4(&transform, Transform);
        ImGui::TreePop();
    }
    ImGui::PopID();
}

// グリッド描画
void SceneGame::DrawGrid(ID3D11DeviceContext* context, int subdivisions, float scale)
{
    int numLines = (subdivisions + 1) * 2;
    int vertexCount = numLines * 2;

    float corner = 0.5f;
    float step = 1.0f / static_cast<float>(subdivisions);

    int index = 0;
    float s = -corner;

    const DirectX::XMFLOAT4 white = DirectX::XMFLOAT4(1, 1, 1, 1);

    LineRenderer* lineRenderer = Graphics::Instance().GetLineRenderer();
    // Create vertical lines
    float scaling = static_cast<float>(subdivisions) * scale;
    DirectX::XMMATRIX M = DirectX::XMMatrixScaling(scaling, scaling, scaling);
    DirectX::XMVECTOR V, P;
    DirectX::XMFLOAT3 position;
    for (int i = 0; i <= subdivisions; i++)
    {
        V = DirectX::XMVectorSet(s, 0, corner, 0);
        P = DirectX::XMVector3TransformCoord(V, M);
        DirectX::XMStoreFloat3(&position, P);
        lineRenderer->AddVertex(position, white);

        V = DirectX::XMVectorSet(s, 0, -corner, 0);
        P = DirectX::XMVector3TransformCoord(V, M);
        DirectX::XMStoreFloat3(&position, P);
        lineRenderer->AddVertex(position, white);

        s += step;
    }

    // Create horizontal lines
    s = -corner;
    for (int i = 0; i <= subdivisions; i++)
    {
        V = DirectX::XMVectorSet(corner, 0, s, 0);
        P = DirectX::XMVector3TransformCoord(V, M);
        DirectX::XMStoreFloat3(&position, P);
        lineRenderer->AddVertex(position, white);

        V = DirectX::XMVectorSet(-corner, 0, s, 0);
        P = DirectX::XMVector3TransformCoord(V, M);
        DirectX::XMStoreFloat3(&position, P);
        lineRenderer->AddVertex(position, white);

        s += step;
    }

    // X軸
    {
        const DirectX::XMFLOAT4 red = DirectX::XMFLOAT4(1, 0, 0, 1);
        V = DirectX::XMVectorSet(0, 0, 0, 0);
        P = DirectX::XMVector3TransformCoord(V, M);
        DirectX::XMStoreFloat3(&position, P);
        lineRenderer->AddVertex(position, red);

        V = DirectX::XMVectorSet(corner, 0, 0, 0);
        P = DirectX::XMVector3TransformCoord(V, M);
        DirectX::XMStoreFloat3(&position, P);
        lineRenderer->AddVertex(position, red);
    }

    // Y軸
    {
        const DirectX::XMFLOAT4 green = DirectX::XMFLOAT4(0, 1, 0, 1);
        V = DirectX::XMVectorSet(0, 0, 0, 0);
        P = DirectX::XMVector3TransformCoord(V, M);
        DirectX::XMStoreFloat3(&position, P);
        lineRenderer->AddVertex(position, green);

        V = DirectX::XMVectorSet(0, corner, 0, 0);
        P = DirectX::XMVector3TransformCoord(V, M);
        DirectX::XMStoreFloat3(&position, P);
        lineRenderer->AddVertex(position, green);
    }

    // Z軸
    {
        const DirectX::XMFLOAT4 blue = DirectX::XMFLOAT4(0, 0, 1, 1);
        V = DirectX::XMVectorSet(0, 0, 0, 0);
        P = DirectX::XMVector3TransformCoord(V, M);
        DirectX::XMStoreFloat3(&position, P);
        lineRenderer->AddVertex(position, blue);

        V = DirectX::XMVectorSet(0, 0, corner, 0);
        P = DirectX::XMVector3TransformCoord(V, M);
        DirectX::XMStoreFloat3(&position, P);
        lineRenderer->AddVertex(position, blue);
    }
}
// 深度値描画
void SceneGame::RenderDepth()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = renderTarget->GetRenderTargetView().Get();
    ID3D11DepthStencilView* dsv = depthStencil->GetDepthStencilView().Get();
    //ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    // 画面クリア＆レンダーターゲット設定
    FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    // ビューポートの設定
    D3D11_VIEWPORT	vp = {};
    vp.Width = graphics.GetScreenWidth();
    vp.Height = graphics.GetScreenHeight();
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    dc->RSSetViewports(1, &vp);

    // 描画処理
    RenderContext rc;
    rc.deviceContext = dc;

    // ライトの情報を詰め込む
    LightManager::Instance().PushRenderContext(rc);

    // シャドウマップの設定
    //rc.shadowMapData.shadowMap = shadowmapDepthStencil->GetShaderResourceView().Get();
    //rc.shadowMapData.lightViewProjection = lightViewProjection;
    //rc.shadowMapData.shadowColor = shadowColor;
    //rc.shadowMapData.shadowBias = shadowBias;
    for (int i = 0; i < ShadowmapCount; ++i)
    {
        rc.shadowMapData.shadowMap[i] = shadowmapDepthStencil[i]->GetShaderResourceView().Get();
        rc.shadowMapData.lightViewProjection[i] = lightViewProjection[i];
        rc.shadowMapData.shadowBias[i] = shadowBias[i];
    }
    rc.shadowMapData.shadowColor = shadowColor;


    // カメラパラメータ設定
    Camera& camera = Camera::Instance();
    rc.viewPosition.x = camera.GetEye().x;
    rc.viewPosition.y = camera.GetEye().y;
    rc.viewPosition.z = camera.GetEye().z;
    rc.viewPosition.w = 1;
    rc.view = camera.GetView();
    rc.projection = camera.GetProjection();

    // 2Dスプライトモデル
    {

    }

    // 3Dモデル描画
    {

        ModelShader* shader = graphics.GetShader(ModelShaderId::Default);
        shader->Begin(rc);

        //shader->Draw(rc, stage.get());
        //shader->Draw(rc, jummo.get());
        //shader->Draw(rc, uncle.get());

        //ステージ描画
        StageManager::Instance().Render(rc, shader);

        //プレイヤー描画
        PlayerManager::Instance().Render(rc, shader);



        //エネミー描画
        //EnemyManager::Instance().Render(rc, shader);

        shader->End(rc);
    }



}

// 3D空間の描画
void SceneGame::Render3DScene()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = renderTarget->GetRenderTargetView().Get();
    //ID3D11DepthStencilView* dsv = depthStencil->GetDepthStencilView().Get();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    // 画面クリア＆レンダーターゲット設定
    FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    // ビューポートの設定
    D3D11_VIEWPORT	vp = {};
    vp.Width = graphics.GetScreenWidth();
    vp.Height = graphics.GetScreenHeight();
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    dc->RSSetViewports(1, &vp);

    // 描画処理
    RenderContext rc;
    rc.deviceContext = dc;

    // ライトの情報を詰め込む
    LightManager::Instance().PushRenderContext(rc);

    // シャドウマップの設定
    //rc.shadowMapData.shadowMap = shadowmapDepthStencil->GetShaderResourceView().Get();
    //rc.shadowMapData.lightViewProjection = lightViewProjection;
    //rc.shadowMapData.shadowColor = shadowColor;
    //rc.shadowMapData.shadowBias = shadowBias;

    for (int i = 0; i < ShadowmapCount; ++i)
    {
        rc.shadowMapData.shadowMap[i] = shadowmapDepthStencil[i]->GetShaderResourceView().Get();
        rc.shadowMapData.lightViewProjection[i] = lightViewProjection[i];
        rc.shadowMapData.shadowBias[i] = shadowBias[i];
    }
    rc.shadowMapData.shadowColor = shadowColor;


    // カメラパラメータ設定
    Camera& camera = Camera::Instance();
    rc.viewPosition.x = camera.GetEye().x;
    rc.viewPosition.y = camera.GetEye().y;
    rc.viewPosition.z = camera.GetEye().z;
    rc.viewPosition.w = 1;
    rc.view = camera.GetView();
    rc.projection = camera.GetProjection();

    // スカイボックスの描画
    {
        //SpriteShader* shader = graphics.GetShader(SpriteShaderId::SkyBox);
        //shader->Begin(rc);

        //shader->Draw(rc, sprite.get());

        //shader->End(rc);


        //SpriteShader* shader = graphics.GetShader(SpriteShaderId::Mask);
        //shader->Begin(rc);

        //rc.maskData.maskTexture = maskTexture->GetShaderResourceView().Get();
        //rc.maskData.dissolveThreshold = dissolveThreshold;
        //rc.maskData.edgeThreshold = edgeThreshold;

        //shader->Draw(rc, sprite.get());

        //shader->End(rc);

    }

    // 3Dモデル描画（フォンシェーダー）
    {

        //ModelShader* shader = graphics.GetShader(ModelShaderId::Phong);
        //shader->Begin(rc);

        //shader->Draw(rc, stage.get());
        //shader->Draw(rc, jummo.get());
        //shader->Draw(rc, uncle.get());

        //ステージ描画
        //StageManager::Instance().Render(rc, shader);

        ////プレイヤー描画
        //PlayerManager::Instance().Render(rc, shader);

        ////エネミー描画
        //EnemyManager::Instance().Render(rc, shader);

        //shader->End(rc);
    }

    //トゥーンシェーダー
    {
        rc.toonData.edgeTexture = depthStencil->GetShaderResourceView().Get();
        rc.toonData.edgeWidth = toonData.edgeWidth;
        rc.toonData.edgeColor = toonData.edgeColor;
        ModelShader* shader = graphics.GetShader(ModelShaderId::Toon);
        shader->Begin(rc);

        //shader->Draw(rc, stage.get());
        //shader->Draw(rc, jummo.get());
        //shader->Draw(rc, uncle.get());

        //ステージ描画
        StageManager::Instance().Render(rc, shader);

        // プレイヤー描画
        //PlayerManager::Instance().Render(rc, shader);
        Player* player = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetWeapon());
        player->Render(rc, shader);

        //  エネミー描画
        EnemyManager::Instance().Render(rc, shader);

        shader->End(rc);
    }

    //3Dエフェクト描画
    {
        EffectManager::Instance().Render(rc.view, rc.projection);
    }

    // デバッグプリミティブの表示
    //TODOここも全部コメントアウト
    {
    //    プレイヤーデバッグプリミティブ描画
    //    PlayerManager::Instance().DrawDebugPrimitive();

    //    エネミーデバッグプリミティブ描画
    //    EnemyManager::Instance().DrawDebugPrimitive();

    //     グリッド描画
    //    DrawGrid(dc, 20, 10.0f);

    //     ライトのデバッグプリミティブの描画
    //    LightManager::Instance().DrawDebugPrimitive();

    //     ラインレンダラ描画実行
    //    graphics.GetLineRenderer()->Render(dc, camera.GetView(), camera.GetProjection());

    //     デバッグレンダラ描画実行
    //    graphics.GetDebugRenderer()->Render(dc, camera.GetView(), camera.GetProjection());
    }

}

// シャドウマップの描画
void SceneGame::RenderShadowmap()
{
    //Graphics& graphics = Graphics::Instance();
    //ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    //ID3D11RenderTargetView* rtv = nullptr;
    //ID3D11DepthStencilView* dsv = shadowmapDepthStencil->GetDepthStencilView().Get();

    //// 画面クリア
    //dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    //if (!mainDirectionalLight)
    //	return;

    //// レンダーターゲット設定
    //dc->OMSetRenderTargets(0, &rtv, dsv);

    //// ビューポートの設定
    //D3D11_VIEWPORT	vp = {};
    //vp.Width = static_cast<float>(shadowmapDepthStencil->GetWidth());
    //vp.Height = static_cast<float>(shadowmapDepthStencil->GetHeight());
    //vp.MinDepth = 0.0f;
    //vp.MaxDepth = 1.0f;
    //dc->RSSetViewports(1, &vp);

    //// 描画処理
    //RenderContext rc;
    //rc.deviceContext = dc;

    //// カメラパラメータ設定
    //{
    //	// 平行光源からカメラ位置を作成し、そこから原点の位置を見るように視線行列を作成
    //	DirectX::XMVECTOR LightPosition = DirectX::XMLoadFloat3(&mainDirectionalLight->GetDirection());
    //	LightPosition = DirectX::XMVectorScale(LightPosition, -250.0f);
    //	DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(LightPosition,
    //		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
    //		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

    //	// シャドウマップに描画したい範囲の射影行列を生成
    //	DirectX::XMMATRIX P = DirectX::XMMatrixOrthographicLH(shadowDrawRect, shadowDrawRect, 0.1f, 1000.0f);

    //	DirectX::XMStoreFloat4x4(&rc.view, V);
    //	DirectX::XMStoreFloat4x4(&rc.projection, P);

    //	DirectX::XMStoreFloat4x4(&lightViewProjection, V * P);
    //}

    //// 3Dモデル描画
    //{
    //	ModelShader* shader = graphics.GetShader(ModelShaderId::ShadowmapCaster);
    //	shader->Begin(rc);

    //	shader->Draw(rc, stage.get());
    //	shader->Draw(rc, jummo.get());
    //	shader->Draw(rc, uncle.get());

    //	shader->End(rc);
    //}
    //if (!mainDirectionalLight)
    //    return;

    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();

    //	シャドウマップの分割エリア定義

    //TODO ここで車道範囲決まるので要注意
    float	splitAreaTable[] =
    {
        Camera::Instance().GetNearZ(),	//	カメラのニアクリップ
        50.0f,
        100.0f,
        300.0f,
        Camera::Instance().GetFarZ(),	//	カメラのファークリップ
    };

    //	とりあえず4枚使用を大前提として作成しているのでアサートで止める。
    assert(ShadowmapCount == 4);	//	本来はstatic_assertの使用が望ましい

    //	カメラのパラメーターを取得
    DirectX::XMVECTOR	cameraFront = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&Camera::Instance().GetFront()));
    DirectX::XMVECTOR	cameraRight = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&Camera::Instance().GetRight()));
    DirectX::XMVECTOR	cameraUp = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&Camera::Instance().GetUp()));
    DirectX::XMVECTOR	cameraPos = DirectX::XMLoadFloat3(&Camera::Instance().GetEye());


    // ライトビュープロジェクション行列の作成
    DirectX::XMMATRIX	viewMatrix, projectionMatrix, viewProjectionMatrix;
    {
        // 平行光源からカメラ位置を作成し、そこから原点の位置を見るように視線行列を生成
        //原点ではなくプレイヤーを中心にしてみる？
        DirectX::XMVECTOR LightPosition = DirectX::XMLoadFloat3(&mainDirectionalLight->GetDirection());
        DirectX::XMVECTOR PlayerPosition = DirectX::XMLoadFloat3(&PlayerManager::Instance().GetPlayer(0)->GetPosition());
        float player_x = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetWeapon())->GetPosition().x;
        float player_y = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetWeapon())->GetPosition().y;
        float player_z = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetWeapon())->GetPosition().z;
        LightPosition = DirectX::XMVectorScale(LightPosition, -250.0f);
        PlayerPosition = DirectX::XMVectorScale(PlayerPosition, 250.0f);
        viewMatrix = DirectX::XMMatrixLookAtLH(LightPosition,
            DirectX::XMVectorSet(0.0f, -0.0f, 0.0f, 0.0f),
            //DirectX::XMVectorSet(player_x, player_y, player_z, 0.0f),
            DirectX::XMVectorSet(0.0f, 1.0f, 0.0, 0.0f));

        // シャドウマップに描画したい範囲の射影行列を生成
        projectionMatrix = DirectX::XMMatrixOrthographicLH(10000, 10000, 1.0f, 2000.0f);
        viewProjectionMatrix = viewMatrix * projectionMatrix;
    }

    for (int i = 0; i < ShadowmapCount; ++i)
    {
        float	nearDepth = splitAreaTable[i + 0];
        float	farDepth = splitAreaTable[i + 1];

        //	シャドウマップにレンダリング
        ID3D11RenderTargetView* rtv = nullptr;
        ID3D11DepthStencilView* dsv = shadowmapDepthStencil[i]->GetDepthStencilView().Get();
        //FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
        //dc->ClearRenderTargetView(rtv, color);

        // 画面クリア
        dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

        // レンダーターゲット設定
        dc->OMSetRenderTargets(0, &rtv, dsv);

        //	エリアを内包する視推台の8頂点を算出する
        DirectX::XMVECTOR	vertex[8];
        {
            //	エリアの近平面の中心からの上面までの距離を求める
            float	nearY = tanf(Camera::Instance().GetFovY() / 2.0f) * nearDepth;

            //	エリアの近平面の中心からの右面までの距離を求める
            float	nearX = nearY * Camera::Instance().GetAspect();

            //	エリアの遠平面の中心からの上面までの距離を求める
            float	farY = tanf(Camera::Instance().GetFovY() / 2.0f) * farDepth;

            //	エリアの遠平面の中心からの右面までの距離を求める
            float	farX = farY * Camera::Instance().GetAspect();

            //	エリアの近平面の中心座標を求める
            //DirectX::XMVECTOR	nearPosition = Camera::Instance().GetEye() + Camera::Instance().GetFront() * nearDepth;
            //DirectX::XMVECTOR	nearPosition = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&Camera::Instance().GetEye()), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetFront()), nearDepth));
            DirectX::XMVECTOR	nearPosition = DirectX::XMVectorAdd(cameraPos, DirectX::XMVectorScale(cameraFront, nearDepth));



            //	エリアの遠平面の中心座標を求める
            //DirectX::XMVECTOR	farPosition = Camera::Instance().GetEye() + Camera::Instance().GetFront() * farDepth;
            //DirectX::XMVECTOR	farPosition = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&Camera::Instance().GetEye()), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetFront()), farDepth));
            DirectX::XMVECTOR	farPosition = DirectX::XMVectorAdd(cameraPos, DirectX::XMVectorScale(cameraFront, farDepth));

            //	8頂点を求める
            {
                // 近平面の右上
                //vertex[0] = nearPosition + Camera::Instance().GetUp() * +nearY + Camera::Instance().GetRight() * +nearX;
                vertex[0] = DirectX::XMVectorAdd(nearPosition, DirectX::XMVectorAdd(DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetUp()), +nearY), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetRight()), +nearX)));


                // 近平面の左上
                //vertex[1] = nearPosition + Camera::Instance().GetUp() * +nearY + Camera::Instance().GetRight() * -nearX;
                vertex[1] = DirectX::XMVectorAdd(nearPosition, DirectX::XMVectorAdd(DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetUp()), +nearY), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetRight()), -nearX)));



                // 近平面の右下
                //vertex[2] = nearPosition + Camera::Instance().GetUp() * -nearY + Camera::Instance().GetRight() * +nearX;
                vertex[2] = DirectX::XMVectorAdd(nearPosition, DirectX::XMVectorAdd(DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetUp()), -nearY), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetRight()), +nearX)));



                // 近平面の左下
                //vertex[3] = nearPosition + Camera::Instance().GetUp() * -nearY + Camera::Instance().GetRight() * -nearX;
                vertex[3] = DirectX::XMVectorAdd(nearPosition, DirectX::XMVectorAdd(DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetUp()), -nearY), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetRight()), -nearX)));



                // 遠平面の右上
                //vertex[4] = farPosition + Camera::Instance().GetUp() * +farY + Camera::Instance().GetRight() * +farX;
                vertex[4] = DirectX::XMVectorAdd(farPosition, DirectX::XMVectorAdd(DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetUp()), +farY), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetRight()), +farX)));


                // 遠平面の左上
                //vertex[5] = farPosition + Camera::Instance().GetUp() * +farY + Camera::Instance().GetRight() * -farX;
                vertex[5] = DirectX::XMVectorAdd(farPosition, DirectX::XMVectorAdd(DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetUp()), +farY), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetRight()), -farX)));


                // 遠平面の右下
                //vertex[6] = farPosition + Camera::Instance().GetUp() * -farY + Camera::Instance().GetRight() * +farX;
                vertex[6] = DirectX::XMVectorAdd(farPosition, DirectX::XMVectorAdd(DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetUp()), -farY), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetRight()), +farX)));


                // 遠平面の左下
                //vertex[7] = farPosition + Camera::Instance().GetUp() * -farY + Camera::Instance().GetRight() * -farX;
                vertex[7] = DirectX::XMVectorAdd(farPosition, DirectX::XMVectorAdd(DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetUp()), -farY), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetRight()), -farX)));

            }
        }
        //	8頂点をライトビュープロジェクション空間に変換して、最大値、最小値を求める
        DirectX::XMFLOAT3	vertexMin(FLT_MAX, FLT_MAX, FLT_MAX), vertexMax(-FLT_MAX, -FLT_MAX, -FLT_MAX);
        for (auto& it : vertex)
        {
            DirectX::XMFLOAT3	p;
            DirectX::XMStoreFloat3(&p, DirectX::XMVector3TransformCoord(it, viewProjectionMatrix));

            vertexMin.x = min(p.x, vertexMin.x);
            vertexMin.y = min(p.y, vertexMin.y);
            vertexMin.z = min(p.z, vertexMin.z);
            vertexMax.x = max(p.x, vertexMax.x);
            vertexMax.y = max(p.y, vertexMax.y);
            vertexMax.z = max(p.z, vertexMax.z);
        }

        //	クロップ行列を求める

        float	xScale = 2.0f / (vertexMax.x - vertexMin.x);
        float	yScale = 2.0f / (vertexMax.y - vertexMin.y);

        float	xOffset = -0.5f * (vertexMax.x + vertexMin.x) * xScale;
        float	yOffset = -0.5f * (vertexMax.y + vertexMin.y) * yScale;

        DirectX::XMFLOAT4X4	clopMatrix;
        clopMatrix._11 = xScale;
        clopMatrix._12 = 0;
        clopMatrix._13 = 0;
        clopMatrix._14 = 0;
        clopMatrix._21 = 0;
        clopMatrix._22 = yScale;
        clopMatrix._23 = 0;
        clopMatrix._24 = 0;
        clopMatrix._31 = 0;
        clopMatrix._32 = 0;
        clopMatrix._33 = 1;
        clopMatrix._34 = 0;
        clopMatrix._41 = xOffset;
        clopMatrix._42 = yOffset;
        clopMatrix._43 = 0;
        clopMatrix._44 = 1;

        //lightViewProjection[i] = lightViewProjectionMatrix * clopMatrix;

        //	ライトビュープロジェクション行列にクロップ行列を乗算
        DirectX::XMStoreFloat4x4(&lightViewProjection[i], viewProjectionMatrix * DirectX::XMLoadFloat4x4(&clopMatrix));

        // ビューポートの設定
        D3D11_VIEWPORT	vp = {};
        vp.Width = static_cast<float>(shadowmapDepthStencil[i]->GetWidth());
        vp.Height = static_cast<float>(shadowmapDepthStencil[i]->GetHeight());
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        dc->RSSetViewports(1, &vp);

        // 描画処理
        RenderContext rc;
        rc.deviceContext = dc;
        DirectX::XMStoreFloat4x4(&rc.view, viewMatrix);
        DirectX::XMStoreFloat4x4(&rc.projection, projectionMatrix * DirectX::XMLoadFloat4x4(&clopMatrix));

        // 3Dモデル描画
        {
            ModelShader* shader = graphics.GetShader(ModelShaderId::ShadowmapCaster);
            shader->Begin(rc);

            //shader->Draw(rc, stage.get());
            //shader->Draw(rc, jummo.get());
            //shader->Draw(rc, uncle.get());

            //ステージ描画
            StageManager::Instance().Render(rc, shader);

            //プレイヤー描画
            PlayerManager::Instance().Render(rc, shader);

            //エネミー描画
            EnemyManager::Instance().Render(rc, shader);

            shader->End(rc);
        }
    }
}