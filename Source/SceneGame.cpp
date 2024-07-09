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

// �V���h�E�}�b�v�̃T�C�Y
static const UINT SHADOWMAP_SIZE = 2048;

// ������
void SceneGame::Initialize()
{
    //BGM������
    Game_Bgm = Audio::Instance().LoadAudioSource("Data/Audio/Bgm/Scene/Game_01.wav");
    //Game_Bgm = Audio::Instance().LoadAudioSource("Data/Audio/Bgm/Scene/Game_02.wav");
    BgmStart = false;

    //�X�e�[�W������
    StageManager& stageManager = StageManager::Instance();
    StageMain* stageMain = new StageMain();
    stageManager.Register(stageMain);

    //�v���C���[������
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


    // �L�^�J����
    //DirectX::XMFLOAT3(0, 75, 75),	//���_
    //�J���������ݒ�
    Graphics& graphics = Graphics::Instance();
    Camera& camera = Camera::Instance();
    camera.SetLookAt(
        DirectX::XMFLOAT3(0, 75, 75),	//���_
        DirectX::XMFLOAT3(0, 0, 0),		//�����_
        DirectX::XMFLOAT3(0, 1, 0)		//�����
    );
    camera.SetPerspectiveFov(
        DirectX::XMConvertToRadians(45),							//��p
        graphics.GetScreenWidth() / graphics.GetScreenHeight(),		//��ʔ䗦
        1.0f,														//�N���b�v����
        10000.0f														//�V
    );

    //�J�����R���g���[���[������
    cameraController = new CameraController();


    //�G�l�~�[������
        EnemyManager& enemyManager = EnemyManager::Instance();


    //#if 0
    //    EnemySlime* slime = new EnemySlime();
    //    slime->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 5));
    //    enemyManager.Register(slime);
    //#else
    //#endif

        //2�l�ȏ�̓G
        //for (int i = 0; i < 10; ++i)
        //{
        //    EnemySlime* slime = new EnemySlime();
        //    slime->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 5));
        //    slime->SetTerritory(slime->GetPosition(), 10.0f);
        //    enemyManager.Register(slime);
        //}

        //�P�l�̓G
        EnemyBOSS* Boss = new EnemyBOSS();
        Boss->SetPosition(DirectX::XMFLOAT3(2.0f, 5, 5));
        Boss->SetTerritory(Boss->GetPosition(), 10.0f);
        enemyManager.Register(Boss);

    //�Q�[�W�X�v���C�g
    //gauge = new Sprite();

    // �}�X�N�e�N�X�`���̓ǂݍ���
    maskTexture = std::make_unique<Texture>("Data/Texture/dissolve_animation2.png");
    //maskTexture = new Texture("Data/Texture/dissolve_animation2.png");

    dissolveThreshold = 0.0f;
    edgeThreshold = 0.2f;		// ����臒l
    edgeColor = { 1,0,0,1 };	// ���̐F

    // ���s������ǉ�
    {
        mainDirectionalLight = new Light(LightType::Directional);
        mainDirectionalLight->SetPosition({ 0,-200,0 });
        mainDirectionalLight->SetDirection({ 1, -1, -1 });
        LightManager::Instance().Register(mainDirectionalLight);
    }

    // �_������ǉ�
    {
        Light* light = new Light(LightType::Point);
        light->SetPosition(DirectX::XMFLOAT3(20, 10, 0));
        light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
        LightManager::Instance().Register(light);
    }

    // �X�|�b�g���C�g�̒ǉ�
    {
        Light* light = new Light(LightType::Spot);
        light->SetPosition(DirectX::XMFLOAT3(-30, 20, 0));
        light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
        light->SetDirection(DirectX::XMFLOAT3(+1, -1, 0));
        light->SetRange(40.0f);
        LightManager::Instance().Register(light);
    }


    // �V�����`��^�[�Q�b�g�̐���
    {
        Graphics& graphics = Graphics::Instance();
        renderTarget = std::make_unique<RenderTarget>(
            static_cast<UINT>(graphics.GetScreenWidth()),
            static_cast<UINT>(graphics.GetScreenHeight()),
            DXGI_FORMAT_R8G8B8A8_UNORM);
    }

    // �V���h�E�}�b�v�p�ɐ[�x�X�e���V���̐���
    {
        //Graphics& graphics = Graphics::Instance();
        //shadowmapDepthStencil = std::make_unique<DepthStencil>(SHADOWMAP_SIZE, SHADOWMAP_SIZE);
        for (auto& it : shadowmapDepthStencil)
        {
            it = std::make_unique<DepthStencil>(SHADOWMAP_SIZE, SHADOWMAP_SIZE);
        }

    }

    // �|�X�g�v���Z�X�`��N���X����
    {
        postprocessingRenderer = std::make_unique<PostprocessingRenderer>();
        // �V�[���e�N�X�`����ݒ肵�Ă���
        ShaderResourceViewData srvData;
        srvData.srv = renderTarget->GetShaderResourceView();
        srvData.width = renderTarget->GetWidth();
        srvData.height = renderTarget->GetHeight();
        postprocessingRenderer->SetSceneData(srvData);
    }
    // �X�J�C�{�b�N�X�e�N�X�`���̓ǂݍ���
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
        //// �e�N�X�`����ǂݍ���
        texture = std::make_unique<Texture>("Data/Texture/Title.png");

        // �X�v���C�g
        sprite = std::make_unique<Sprite>();
        sprite->SetShaderResourceView(texture->GetShaderResourceView(), texture->GetWidth(), texture->GetHeight());

        fadeout = std::make_unique<Sprite>("Data/Sprite/fadeout.png");

    }

    

    // �J�����ύX�O��update��x�ʂ�

    changeangle = false;
}

// �I����
void SceneGame::Finalize()
{
    //�G�l�~�[�I����
    EnemyManager::Instance().Clear();

    //�v���C���[�I����
    PlayerManager::Instance().Clear();

    //LightManager::Instance().Clear();


    //�J�����R���g���[���[�I����
    if (cameraController != nullptr)
    {
        delete cameraController;
        cameraController = nullptr;
    }

    UI::Instance().Clear();


    

    //�X�e�[�W�I����
    StageManager::Instance().Clear();

    Game_Bgm->Stop();
}

// �X�V����
void SceneGame::Update(float elapsedTime)
{
    //TODO�@�����I�Ƀt���X�N���[���ɂ��鏈��
    //Graphics& graphics = Graphics::Instance();
    //graphics.GetSwapChain()->SetFullscreenState(true, nullptr);

    //Bgm�J�n
    if (BgmStart == false)
    {
        Game_Bgm->Play(true, 0.6f);     //TODO Se�̂��߂ɉ��ʒ���������
        BgmStart = true;
    }
    //�X�e�[�W�X�V����
    StageManager::Instance().Update(elapsedTime);

    EnemyManager& enemyManager = EnemyManager::Instance();
    int enemyCount = enemyManager.GetEnemyCount();

    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        if (enemy->GetCameraChange())
        {
            //�v���C���[�X�V����
            //PlayerManager::Instance().Update(elapsedTime);
            Player* player = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetWeapon());
            player->Update(elapsedTime);
            UI::Instance().Update(elapsedTime);
        }
    }

    if (!PlayerManager::Instance().GetPlayer(3)->GetChangeTimeMode() )
    //�G�l�~�[�X�V����
    EnemyManager::Instance().Update(elapsedTime);

    //�G�t�F�N�g�X�V����
   EffectManager::Instance().Update(elapsedTime);

    //UI�X�V����




    // �J����
    if (!PlayerManager::Instance().GetPlayer(3)->GetChangeTimeMode())
    {
        DirectX::XMFLOAT3 target = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetWeapon())->GetPosition();
        target.y += 0.5f;										//�v���C���[�̍��ӂ�Ƀ^�[�Q�b�g��ݒ�
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
            //�J�����R���g���[���[�X�V����
            DirectX::XMFLOAT3 target = enemy->GetPosition();
            target.y += 3.5f;										//�v���C���[�̍��ӂ�Ƀ^�[�Q�b�g��ݒ�
            cameraController->SetTarget(target);

            cameraController->UpdateDemo(elapsedTime);
            // �A�b�v�ő��
            if (enemy->GetRastApCamera())
            {
                // �A�b�v����
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

        //// �v���C���[�̃J���������o���B
        //if (PlayerManager::Instance().GetPlayer(3)->GetChangeTimeMode())
        //{
        //    DirectX::XMFLOAT3 target = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetWeapon())->GetPosition();
        //    target.y += 0.5f;										//�v���C���[�̍��ӂ�Ƀ^�[�Q�b�g��ݒ�
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

    //// 2D�}�X�N�V�F�[�_
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


    // enemy���S

    if (EnemyManager::Instance().GetDie())
    {
        SceneManager::Instance().ChangeScene(new SceneClear);
    }

    //TODOplayer���ŌĂ�
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

// �`�揈��
void SceneGame::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();


    // �V���h�E�}�b�v�̕`��
    //RenderShadowmap();

    // �[�x�l�`��
    RenderDepth();

    // 3D ��Ԃ̕`���ʂ̃o�b�t�@�ɑ΂��čs��
    Render3DScene();

    // �������ݐ���o�b�N�o�b�t�@�ɕς��ăI�t�X�N���[�������_�����O�̌��ʂ�`�悷��
    {
        ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
        ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

        // ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
        FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
        dc->ClearRenderTargetView(rtv, color);
        dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        dc->OMSetRenderTargets(1, &rtv, dsv);

        // �r���[�|�[�g�̐ݒ�
        D3D11_VIEWPORT vp = {};
        vp.Width = graphics.GetScreenWidth();
        vp.Height = graphics.GetScreenHeight();
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        dc->RSSetViewports(1, &vp);

        // �|�X�g�v���Z�X�������s��
        postprocessingRenderer->Render(dc);

    }

    // 2D�X�v���C�g�`��
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

        //// �`�揈��
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

    //2D�f�o�b�O�`��
    {

        PlayerManager::Instance().DrawDebugGUI();
    }

    // �f�o�b�O���̕\��
    //TODO �����[�X���͑S���R�����g�A�E�g������
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

// �O���b�h�`��
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

    // X��
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

    // Y��
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

    // Z��
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
// �[�x�l�`��
void SceneGame::RenderDepth()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = renderTarget->GetRenderTargetView().Get();
    ID3D11DepthStencilView* dsv = depthStencil->GetDepthStencilView().Get();
    //ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    // ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
    FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    // �r���[�|�[�g�̐ݒ�
    D3D11_VIEWPORT	vp = {};
    vp.Width = graphics.GetScreenWidth();
    vp.Height = graphics.GetScreenHeight();
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    dc->RSSetViewports(1, &vp);

    // �`�揈��
    RenderContext rc;
    rc.deviceContext = dc;

    // ���C�g�̏����l�ߍ���
    LightManager::Instance().PushRenderContext(rc);

    // �V���h�E�}�b�v�̐ݒ�
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


    // �J�����p�����[�^�ݒ�
    Camera& camera = Camera::Instance();
    rc.viewPosition.x = camera.GetEye().x;
    rc.viewPosition.y = camera.GetEye().y;
    rc.viewPosition.z = camera.GetEye().z;
    rc.viewPosition.w = 1;
    rc.view = camera.GetView();
    rc.projection = camera.GetProjection();

    // 2D�X�v���C�g���f��
    {

    }

    // 3D���f���`��
    {

        ModelShader* shader = graphics.GetShader(ModelShaderId::Default);
        shader->Begin(rc);

        //shader->Draw(rc, stage.get());
        //shader->Draw(rc, jummo.get());
        //shader->Draw(rc, uncle.get());

        //�X�e�[�W�`��
        StageManager::Instance().Render(rc, shader);

        //�v���C���[�`��
        PlayerManager::Instance().Render(rc, shader);



        //�G�l�~�[�`��
        //EnemyManager::Instance().Render(rc, shader);

        shader->End(rc);
    }



}

// 3D��Ԃ̕`��
void SceneGame::Render3DScene()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = renderTarget->GetRenderTargetView().Get();
    //ID3D11DepthStencilView* dsv = depthStencil->GetDepthStencilView().Get();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    // ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
    FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    // �r���[�|�[�g�̐ݒ�
    D3D11_VIEWPORT	vp = {};
    vp.Width = graphics.GetScreenWidth();
    vp.Height = graphics.GetScreenHeight();
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    dc->RSSetViewports(1, &vp);

    // �`�揈��
    RenderContext rc;
    rc.deviceContext = dc;

    // ���C�g�̏����l�ߍ���
    LightManager::Instance().PushRenderContext(rc);

    // �V���h�E�}�b�v�̐ݒ�
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


    // �J�����p�����[�^�ݒ�
    Camera& camera = Camera::Instance();
    rc.viewPosition.x = camera.GetEye().x;
    rc.viewPosition.y = camera.GetEye().y;
    rc.viewPosition.z = camera.GetEye().z;
    rc.viewPosition.w = 1;
    rc.view = camera.GetView();
    rc.projection = camera.GetProjection();

    // �X�J�C�{�b�N�X�̕`��
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

    // 3D���f���`��i�t�H���V�F�[�_�[�j
    {

        //ModelShader* shader = graphics.GetShader(ModelShaderId::Phong);
        //shader->Begin(rc);

        //shader->Draw(rc, stage.get());
        //shader->Draw(rc, jummo.get());
        //shader->Draw(rc, uncle.get());

        //�X�e�[�W�`��
        //StageManager::Instance().Render(rc, shader);

        ////�v���C���[�`��
        //PlayerManager::Instance().Render(rc, shader);

        ////�G�l�~�[�`��
        //EnemyManager::Instance().Render(rc, shader);

        //shader->End(rc);
    }

    //�g�D�[���V�F�[�_�[
    {
        rc.toonData.edgeTexture = depthStencil->GetShaderResourceView().Get();
        rc.toonData.edgeWidth = toonData.edgeWidth;
        rc.toonData.edgeColor = toonData.edgeColor;
        ModelShader* shader = graphics.GetShader(ModelShaderId::Toon);
        shader->Begin(rc);

        //shader->Draw(rc, stage.get());
        //shader->Draw(rc, jummo.get());
        //shader->Draw(rc, uncle.get());

        //�X�e�[�W�`��
        StageManager::Instance().Render(rc, shader);

        // �v���C���[�`��
        //PlayerManager::Instance().Render(rc, shader);
        Player* player = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetWeapon());
        player->Render(rc, shader);

        //  �G�l�~�[�`��
        EnemyManager::Instance().Render(rc, shader);

        shader->End(rc);
    }

    //3D�G�t�F�N�g�`��
    {
        EffectManager::Instance().Render(rc.view, rc.projection);
    }

    // �f�o�b�O�v���~�e�B�u�̕\��
    //TODO�������S���R�����g�A�E�g
    {
    //    �v���C���[�f�o�b�O�v���~�e�B�u�`��
    //    PlayerManager::Instance().DrawDebugPrimitive();

    //    �G�l�~�[�f�o�b�O�v���~�e�B�u�`��
    //    EnemyManager::Instance().DrawDebugPrimitive();

    //     �O���b�h�`��
    //    DrawGrid(dc, 20, 10.0f);

    //     ���C�g�̃f�o�b�O�v���~�e�B�u�̕`��
    //    LightManager::Instance().DrawDebugPrimitive();

    //     ���C�������_���`����s
    //    graphics.GetLineRenderer()->Render(dc, camera.GetView(), camera.GetProjection());

    //     �f�o�b�O�����_���`����s
    //    graphics.GetDebugRenderer()->Render(dc, camera.GetView(), camera.GetProjection());
    }

}

// �V���h�E�}�b�v�̕`��
void SceneGame::RenderShadowmap()
{
    //Graphics& graphics = Graphics::Instance();
    //ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    //ID3D11RenderTargetView* rtv = nullptr;
    //ID3D11DepthStencilView* dsv = shadowmapDepthStencil->GetDepthStencilView().Get();

    //// ��ʃN���A
    //dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    //if (!mainDirectionalLight)
    //	return;

    //// �����_�[�^�[�Q�b�g�ݒ�
    //dc->OMSetRenderTargets(0, &rtv, dsv);

    //// �r���[�|�[�g�̐ݒ�
    //D3D11_VIEWPORT	vp = {};
    //vp.Width = static_cast<float>(shadowmapDepthStencil->GetWidth());
    //vp.Height = static_cast<float>(shadowmapDepthStencil->GetHeight());
    //vp.MinDepth = 0.0f;
    //vp.MaxDepth = 1.0f;
    //dc->RSSetViewports(1, &vp);

    //// �`�揈��
    //RenderContext rc;
    //rc.deviceContext = dc;

    //// �J�����p�����[�^�ݒ�
    //{
    //	// ���s��������J�����ʒu���쐬���A�������猴�_�̈ʒu������悤�Ɏ����s����쐬
    //	DirectX::XMVECTOR LightPosition = DirectX::XMLoadFloat3(&mainDirectionalLight->GetDirection());
    //	LightPosition = DirectX::XMVectorScale(LightPosition, -250.0f);
    //	DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(LightPosition,
    //		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
    //		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

    //	// �V���h�E�}�b�v�ɕ`�悵�����͈͂̎ˉe�s��𐶐�
    //	DirectX::XMMATRIX P = DirectX::XMMatrixOrthographicLH(shadowDrawRect, shadowDrawRect, 0.1f, 1000.0f);

    //	DirectX::XMStoreFloat4x4(&rc.view, V);
    //	DirectX::XMStoreFloat4x4(&rc.projection, P);

    //	DirectX::XMStoreFloat4x4(&lightViewProjection, V * P);
    //}

    //// 3D���f���`��
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

    //	�V���h�E�}�b�v�̕����G���A��`

    //TODO �����Ŏԓ��͈͌��܂�̂ŗv����
    float	splitAreaTable[] =
    {
        Camera::Instance().GetNearZ(),	//	�J�����̃j�A�N���b�v
        50.0f,
        100.0f,
        300.0f,
        Camera::Instance().GetFarZ(),	//	�J�����̃t�@�[�N���b�v
    };

    //	�Ƃ肠����4���g�p���O��Ƃ��č쐬���Ă���̂ŃA�T�[�g�Ŏ~�߂�B
    assert(ShadowmapCount == 4);	//	�{����static_assert�̎g�p���]�܂���

    //	�J�����̃p�����[�^�[���擾
    DirectX::XMVECTOR	cameraFront = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&Camera::Instance().GetFront()));
    DirectX::XMVECTOR	cameraRight = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&Camera::Instance().GetRight()));
    DirectX::XMVECTOR	cameraUp = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&Camera::Instance().GetUp()));
    DirectX::XMVECTOR	cameraPos = DirectX::XMLoadFloat3(&Camera::Instance().GetEye());


    // ���C�g�r���[�v���W�F�N�V�����s��̍쐬
    DirectX::XMMATRIX	viewMatrix, projectionMatrix, viewProjectionMatrix;
    {
        // ���s��������J�����ʒu���쐬���A�������猴�_�̈ʒu������悤�Ɏ����s��𐶐�
        //���_�ł͂Ȃ��v���C���[�𒆐S�ɂ��Ă݂�H
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

        // �V���h�E�}�b�v�ɕ`�悵�����͈͂̎ˉe�s��𐶐�
        projectionMatrix = DirectX::XMMatrixOrthographicLH(10000, 10000, 1.0f, 2000.0f);
        viewProjectionMatrix = viewMatrix * projectionMatrix;
    }

    for (int i = 0; i < ShadowmapCount; ++i)
    {
        float	nearDepth = splitAreaTable[i + 0];
        float	farDepth = splitAreaTable[i + 1];

        //	�V���h�E�}�b�v�Ƀ����_�����O
        ID3D11RenderTargetView* rtv = nullptr;
        ID3D11DepthStencilView* dsv = shadowmapDepthStencil[i]->GetDepthStencilView().Get();
        //FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
        //dc->ClearRenderTargetView(rtv, color);

        // ��ʃN���A
        dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

        // �����_�[�^�[�Q�b�g�ݒ�
        dc->OMSetRenderTargets(0, &rtv, dsv);

        //	�G���A�����鎋�����8���_���Z�o����
        DirectX::XMVECTOR	vertex[8];
        {
            //	�G���A�̋ߕ��ʂ̒��S����̏�ʂ܂ł̋��������߂�
            float	nearY = tanf(Camera::Instance().GetFovY() / 2.0f) * nearDepth;

            //	�G���A�̋ߕ��ʂ̒��S����̉E�ʂ܂ł̋��������߂�
            float	nearX = nearY * Camera::Instance().GetAspect();

            //	�G���A�̉����ʂ̒��S����̏�ʂ܂ł̋��������߂�
            float	farY = tanf(Camera::Instance().GetFovY() / 2.0f) * farDepth;

            //	�G���A�̉����ʂ̒��S����̉E�ʂ܂ł̋��������߂�
            float	farX = farY * Camera::Instance().GetAspect();

            //	�G���A�̋ߕ��ʂ̒��S���W�����߂�
            //DirectX::XMVECTOR	nearPosition = Camera::Instance().GetEye() + Camera::Instance().GetFront() * nearDepth;
            //DirectX::XMVECTOR	nearPosition = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&Camera::Instance().GetEye()), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetFront()), nearDepth));
            DirectX::XMVECTOR	nearPosition = DirectX::XMVectorAdd(cameraPos, DirectX::XMVectorScale(cameraFront, nearDepth));



            //	�G���A�̉����ʂ̒��S���W�����߂�
            //DirectX::XMVECTOR	farPosition = Camera::Instance().GetEye() + Camera::Instance().GetFront() * farDepth;
            //DirectX::XMVECTOR	farPosition = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&Camera::Instance().GetEye()), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetFront()), farDepth));
            DirectX::XMVECTOR	farPosition = DirectX::XMVectorAdd(cameraPos, DirectX::XMVectorScale(cameraFront, farDepth));

            //	8���_�����߂�
            {
                // �ߕ��ʂ̉E��
                //vertex[0] = nearPosition + Camera::Instance().GetUp() * +nearY + Camera::Instance().GetRight() * +nearX;
                vertex[0] = DirectX::XMVectorAdd(nearPosition, DirectX::XMVectorAdd(DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetUp()), +nearY), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetRight()), +nearX)));


                // �ߕ��ʂ̍���
                //vertex[1] = nearPosition + Camera::Instance().GetUp() * +nearY + Camera::Instance().GetRight() * -nearX;
                vertex[1] = DirectX::XMVectorAdd(nearPosition, DirectX::XMVectorAdd(DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetUp()), +nearY), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetRight()), -nearX)));



                // �ߕ��ʂ̉E��
                //vertex[2] = nearPosition + Camera::Instance().GetUp() * -nearY + Camera::Instance().GetRight() * +nearX;
                vertex[2] = DirectX::XMVectorAdd(nearPosition, DirectX::XMVectorAdd(DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetUp()), -nearY), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetRight()), +nearX)));



                // �ߕ��ʂ̍���
                //vertex[3] = nearPosition + Camera::Instance().GetUp() * -nearY + Camera::Instance().GetRight() * -nearX;
                vertex[3] = DirectX::XMVectorAdd(nearPosition, DirectX::XMVectorAdd(DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetUp()), -nearY), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetRight()), -nearX)));



                // �����ʂ̉E��
                //vertex[4] = farPosition + Camera::Instance().GetUp() * +farY + Camera::Instance().GetRight() * +farX;
                vertex[4] = DirectX::XMVectorAdd(farPosition, DirectX::XMVectorAdd(DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetUp()), +farY), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetRight()), +farX)));


                // �����ʂ̍���
                //vertex[5] = farPosition + Camera::Instance().GetUp() * +farY + Camera::Instance().GetRight() * -farX;
                vertex[5] = DirectX::XMVectorAdd(farPosition, DirectX::XMVectorAdd(DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetUp()), +farY), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetRight()), -farX)));


                // �����ʂ̉E��
                //vertex[6] = farPosition + Camera::Instance().GetUp() * -farY + Camera::Instance().GetRight() * +farX;
                vertex[6] = DirectX::XMVectorAdd(farPosition, DirectX::XMVectorAdd(DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetUp()), -farY), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetRight()), +farX)));


                // �����ʂ̍���
                //vertex[7] = farPosition + Camera::Instance().GetUp() * -farY + Camera::Instance().GetRight() * -farX;
                vertex[7] = DirectX::XMVectorAdd(farPosition, DirectX::XMVectorAdd(DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetUp()), -farY), DirectX::XMVectorScale(DirectX::XMLoadFloat3(&Camera::Instance().GetRight()), -farX)));

            }
        }
        //	8���_�����C�g�r���[�v���W�F�N�V������Ԃɕϊ����āA�ő�l�A�ŏ��l�����߂�
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

        //	�N���b�v�s������߂�

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

        //	���C�g�r���[�v���W�F�N�V�����s��ɃN���b�v�s�����Z
        DirectX::XMStoreFloat4x4(&lightViewProjection[i], viewProjectionMatrix * DirectX::XMLoadFloat4x4(&clopMatrix));

        // �r���[�|�[�g�̐ݒ�
        D3D11_VIEWPORT	vp = {};
        vp.Width = static_cast<float>(shadowmapDepthStencil[i]->GetWidth());
        vp.Height = static_cast<float>(shadowmapDepthStencil[i]->GetHeight());
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        dc->RSSetViewports(1, &vp);

        // �`�揈��
        RenderContext rc;
        rc.deviceContext = dc;
        DirectX::XMStoreFloat4x4(&rc.view, viewMatrix);
        DirectX::XMStoreFloat4x4(&rc.projection, projectionMatrix * DirectX::XMLoadFloat4x4(&clopMatrix));

        // 3D���f���`��
        {
            ModelShader* shader = graphics.GetShader(ModelShaderId::ShadowmapCaster);
            shader->Begin(rc);

            //shader->Draw(rc, stage.get());
            //shader->Draw(rc, jummo.get());
            //shader->Draw(rc, uncle.get());

            //�X�e�[�W�`��
            StageManager::Instance().Render(rc, shader);

            //�v���C���[�`��
            PlayerManager::Instance().Render(rc, shader);

            //�G�l�~�[�`��
            EnemyManager::Instance().Render(rc, shader);

            shader->End(rc);
        }
    }
}