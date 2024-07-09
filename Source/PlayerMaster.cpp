#include <imgui.h>
#include "PlayerMaster.h"
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraite.h"
#include "ProjectileHoming.h"
#include "LightManager.h"
#include "PlayerManager.h"

//コンストラクタ
PlayerMaster::PlayerMaster()
{
    //モデルのロード
    model = new Model("Data/Model/Player/player_master.mdl");

    //体力の設定
    health = 15;
    maxHealth = 15;

    //モデルが大きいのでスケーリング
    scale.x = scale.y = scale.z = 0.02f;

    attackcount = 0;
    weapon = 0;

    changetime = 0.0f;

    //待機ステートへ遷移
    TransitionIdle_MasterState();
    weaponModel = new Weapon();
}

//デストラクタ
PlayerMaster::~PlayerMaster()
{
    //武器終了化
    delete weaponModel;
    weaponModel = nullptr;



    delete model;
}

//更新処理
void PlayerMaster::Update(float elapsedTime)
{
    //velocityとかも必要なら０にする
    //魔法の壁
    if (position.x >= 45.0f)
    {
        position.x = 45.0f;
    }
    if (position.x <= -45.0f)
    {
        position.x = -45.0f;
    }
    if (position.z >= 45.0f)
    {
        position.z = 45.0f;
    }
    if (position.z <= -45.0f)
    {
        position.z = -45.0f;
    }

    //魔法の床
    if (position.y < -8.627f)
    {
        position.y = -8.627f;
    }

    if (PlayerManager::Instance().GetWeapon() == 3)
    {
        //ステート毎の処理
        switch (state)
        {
            //双剣
        case State::Idle_Master:
            UpdateIdle_MasterState(elapsedTime);
            break;
        case State::Change_Master:
            UpdateChange_MasterState(elapsedTime);
            break;

        }

        //武器変更クールタイム
        changetimer++;

        //速力処理更新
        UpdateVelocity(elapsedTime);


        //オブジェクト行列を更新
        UpdateTransform();

        //モデルアニメーション更新処理
        model->UpdateAnimation(elapsedTime);


        weaponModel->Update(elapsedTime);

        //モデル行列更新
        model->UpdateTransform(transform);
        //↑オブジェクトの位置にモデル表示するため、
        //オブジェクトの行列をモデルに渡してモデルの行列を計算する
    }
}

//描画処理
void PlayerMaster::Render(RenderContext& rc, ModelShader* shader)
{
    rc.toonData.edgeWidth = 0.02f;


    weaponModel->Render(rc, shader);

    shader->Draw(rc, model);
    //↑描画する際に計算されたモデルの行列をこの中で利用

    //弾丸描画処理
    projectileManager.Render(rc, shader);
}

//デバッグ用GUI描画
void PlayerMaster::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("PlayerMaster", nullptr, ImGuiWindowFlags_None))
    {
        //トランスフォーム
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //位置
            ImGui::InputFloat3("Position", &position.x);

            //回転
            DirectX::XMFLOAT3 a;
            a.x = DirectX::XMConvertToDegrees(angle.x);
            a.y = DirectX::XMConvertToDegrees(angle.y);
            a.z = DirectX::XMConvertToDegrees(angle.z);
            ImGui::InputFloat3("Angle", &a.x);
            angle.x = DirectX::XMConvertToRadians(a.x);
            angle.y = DirectX::XMConvertToRadians(a.y);
            angle.z = DirectX::XMConvertToRadians(a.z);

            //スケール
            ImGui::InputFloat3("Scale", &scale.x);
        }

        //ムーブメント（動きの速さ）
        if (ImGui::CollapsingHeader("Movement", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //速度
            ImGui::InputFloat("MoveSpeed", &moveSpeed);

            //回転速度
            float ts;
            ts = DirectX::XMConvertToDegrees(turnSpeed);
            ImGui::InputFloat("TurnSpeed", &ts);
            turnSpeed = DirectX::XMConvertToRadians(ts);

            //ジャンプ速度
            ImGui::InputFloat("JumpSpeed", &jumpSpeed);

            //重力
            ImGui::InputFloat("Gravity", &gravity);

            //加速度？
            ImGui::InputFloat3("Velocity", &velocity.x);

            //今のステート
            ImGui::InputInt("State", &statenumber);

            //今の武器番号
            ImGui::InputInt("Weapon", &weapon);
        }

        //コリジョン
        if (ImGui::CollapsingHeader("Collision", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //半径
            ImGui::InputFloat("Radius", &radius);
        }
    }
    ImGui::End();
}

//デバッグプリミティブ描画
void PlayerMaster::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //衝突判定用のデバッグ球を描画
    //debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

    //衝突判定用のデバッグ円柱を描画
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

    //弾丸デバッグプリミティブ描画
    projectileManager.DrawDebugPrimitive();

    //攻撃衝突用の左手ノードのデバッグ球を描画
    if (attackCollisionFlag)
    {
        Model::Node* swordBone_R = model->FindNode("master_Large_sword_02:J_hook_R_03");
        Model::Node* swordBone_L = model->FindNode("master_Large_sword_02:J_hook_L_03");
        debugRenderer->DrawSphere(DirectX::XMFLOAT3(
            swordBone_R->worldTransform._41,
            swordBone_R->worldTransform._42,
            swordBone_R->worldTransform._43),
            MasterRadius,
            DirectX::XMFLOAT4(1, 0, 0, 1)
        );
        debugRenderer->DrawSphere(DirectX::XMFLOAT3(
            swordBone_L->worldTransform._41,
            swordBone_L->worldTransform._42,
            swordBone_L->worldTransform._43),
            MasterRadius,
            DirectX::XMFLOAT4(1, 0, 0, 1)
        );
    }
}




DirectX::XMFLOAT3 PlayerMaster::GetMoveVec() const
{
    //入力情報を取得
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();     //左スティックX軸入力状態を取得
    float ay = gamePad.GetAxisLY();     //左スティックY軸入力状態を取得

    //カメラ方向とスティックの入力値によって進行方向を計算する
    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

    //移動ベクトルはXZ平面に水平なベクトルになるようにする

    //カメラ右方向ベクトルをXZ単位ベクトルに変換
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLength > 0.0f)
    {
        //単位ベクトル化
        cameraRightX /= cameraRightLength;
        cameraRightZ /= cameraRightLength;
    }

    //カメラ前方向ベクトルをXZ単位ベクトルに変換
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraRightLength > 0.0f)
    {
        //単位ベクトル化
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }

    //スティックの水平入力値をカメラ右方向に反映し、
    //スティックの垂直入力値をカメラ前方向に反映し、
    //進行ベクトルを計算する
    DirectX::XMFLOAT3 vec;
    vec.x = (cameraRightX * ax) + (cameraFrontX * ay);
    vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);
    //Y軸方向には移動しない
    vec.y = 0.0f;

    return vec;
}


//双剣
void PlayerMaster::TransitionIdle_MasterState()
{
    statenumber = 21;

    //双剣の待機に遷移
    state = State::Idle_Master;

    //待機アニメーション再生
    model->PlayAnimation(Anim_Master_Idle, true);
}

void PlayerMaster::UpdateIdle_MasterState(float elapsedTime)
{
  

    TransitionChange_MasterState();
}


void PlayerMaster::TransitionChange_MasterState()
{
    state = State::Change_Master;

    weapon = 3;
    changetime = 0.0f;

    //PlayerManager::Instance().SetStartAnimation(true);

    //武器変更モーション再生
    //model->PlayAnimation(Anim_Master_Change, false);
    model->PlayAnimation(Anim_Master_Switch, false);

    //PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetWeapon())
    //    ->SetChangeTimeMode(true);
    changetimemode = true;
}

void PlayerMaster::UpdateChange_MasterState(float elapsedTime)
{
    //プレイヤーの位置を常に取得し続ける
    PlayerManager& playerMgr = PlayerManager::Instance();


    //Model::Node* node = model->FindNode("player_right_arm1");
    //DirectX::XMFLOAT3 hand_position = {
    //    node->worldTransform._41,
    //    node->worldTransform._42,
    //    node->worldTransform._43
    //};


    //SetSetHandPositionR(hand_position);

    DirectX::XMFLOAT3 position_Master = position;
    DirectX::XMFLOAT3 angle_Master = angle;

    //playerMgr.GetPlayer(PlayerManager::Instance().GetWeapon())->
    //    SetHandPositionR(hand_position);


    shackChack = false;
        // 任意のアニメーション再生区間でのみ衝突処理をする
    float animationTime = model->GetCurrentAnimationSeconds();

    if (animationTime >= 0.5f && animationTime <= 0.8f)
        shackChack = true;


    if (!model->IsPlayAnimation())
    {
        changetimemode = false;
        DirectX::XMFLOAT3 position_Master = position;
        DirectX::XMFLOAT3 angle_Master = angle;
        playerMgr.SetWeaponFlag(true);
        //PlayerManager::Instance().SetWeapon(0);
        if(playerMgr.GetBeforeWeapon() >= 2)
        playerMgr.SetWeapon(0);
        else
        {
            playerMgr.SetWeapon(playerMgr.GetBeforeWeapon() + 1);
        }
        Player* player = playerMgr.GetPlayer(playerMgr.GetWeapon());
        position_Master.y += 0.1f;
        player->SetPosition(position_Master);
        player->SetAngle(angle_Master);
        player->SetChangeTimer(0.0f);
        player->SetHP(health);
        // 揺れない
        shackStrongChack = false;

        TransitionIdle_MasterState();
        position = { 0,1000000000000.0f,0 };
    }

    //武器変更モーションの再生が終われば次の武器へ変更
    //if (!model->IsPlayAnimation())
    //{
    //    //TODOManagerでmodel自体を変更する
    //}
}
