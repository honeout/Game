#include <imgui.h>
#include "PlayerGun.h"
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraite.h"
#include "ProjectileHoming.h"
#include "LightManager.h"
#include "PlayerManager.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

double getRandomFromRange(double min, double max)
{
    return min + (max - min) * (rand() / (RAND_MAX + 1.0));
}

float getRandomInRange(float min, float max)
{
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

//コンストラクタ
PlayerGun::PlayerGun()
{
    //モデルのロード
    model = new Model("Data/Model/Player/gun_player.mdl");

    //体力の設定
    health = 15;
    maxHealth = 15;

    //モデルが大きいのでスケーリング
    scale.x = scale.y = scale.z = 0.02f;

    //ヒットエフェクト読み込み
    hitEffect = new Effect("Data/Effect/bossHitShotDamage.efk");
    invincible_hitEffect = new Effect("Data/Effect/invincible.efk");
    shotEffect = new Effect("Data/Effect/PlayerShot.efk");

    attackcount = 18;
    weapon = 0;
    bullets = 60;
    totalattackcount = 0;
    changetime = 0.0f;
    reloadtime = 0.0f;
    braketime = 0.0f;

    //カメラコントローラー初期化
    cameraController = new CameraController();

    UpdateTransform();

    //SEの設定
    shot = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Gun_Attack2.wav");
    reload = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Gun_Reload.wav");
    damaged = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Player_Damaged.wav");
    brake = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Player_Brake.wav");
    run_fast = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Run_Fast_Dirt.wav");
    run_slow = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Run_Slow_Dirt.wav");

    //待機ステートへ遷移
    TransitionIdle_GunState();

    // 乱数の初期化
    //srand(time(nullptr));
}

//デストラクタ
PlayerGun::~PlayerGun()
{
    //カメラコントローラー終了化
    if (cameraController != nullptr)
    {
        delete cameraController;
        cameraController = nullptr;
    }
    invincible_hitEffect->stop(invincible_hitEffect->GetHandle());
    delete  invincible_hitEffect;
    delete hitEffect;
    delete shotEffect;

    delete model;
}

//更新処理
void PlayerGun::Update(float elapsedTime)
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
        //魔法の床
    if (position.y >= 0)
    {
        position.y = -1;
    }
    if (position.y < -10.0f)
    {
        position.y = -1;
    }

    //ステート毎の処理
        switch (state)
        {
            //銃
        case State::Idle_Gun:
            UpdateIdle_GunState(elapsedTime);
            break;
        case State::Walk_Gun:
            UpdateWalk_GunState(elapsedTime);
            break;
        case State::Run_Gun:
            UpdateRun_GunState(elapsedTime);
            break;
        case State::Attack_Gun:
            UpdateAttack_GunState(elapsedTime);
            break;
        case State::Damage_Gun:
            UpdateDamage_GunState(elapsedTime);
            break;
        case State::Death_Gun:
            UpdateDeath_GunState(elapsedTime);
            break;
        case State::Dodge_Gun:
            UpdateDodge_GunState(elapsedTime);
            break;
        case State::Reload_Gun:
            UpdateReload_GunState(elapsedTime);
            break;
        case State::Change_Gun:
            UpdateChange_GunState(elapsedTime);
            break;
        case State::Brake_Gun:
            UpdateBrake_GunState(elapsedTime);
            break;
        }

        //光の追従
        //LightManager::Instance().GetLight(1)->SetPosition({position.x, position.y + 3.0f, position.z});

        //武器変更クールタイム
        changetimer++;

        //リロードタイム
        reloadtime++;

        //速力処理更新
        UpdateVelocity(elapsedTime);

        //無敵時間更新処理
        UpdateInvincibleTimer(elapsedTime);

        //弾丸更新処理
        projectileManager.Update(elapsedTime);

        //プレイヤーと敵との衝突処理
        CollisionPlayerVsEnemies();

        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();


        //弾丸と敵の衝突処理
        CollisionProjectilesVsEnemies();
        
        

        //オブジェクト行列を更新
        UpdateTransform();

        //モデルアニメーション更新処理
        model->UpdateAnimation(elapsedTime);

        //モデル行列更新
        model->UpdateTransform(transform);
        //↑オブジェクトの位置にモデル表示するため、
        //オブジェクトの行列をモデルに渡してモデルの行列を計算する

        rate++;
        braketime++;
}

//描画処理
void PlayerGun::Render(RenderContext& rc, ModelShader* shader)
{
    rc.toonData.edgeWidth = 0.02f;

    shader->Draw(rc, model);
    //↑描画する際に計算されたモデルの行列をこの中で利用

    //弾丸描画処理
    projectileManager.Render(rc, shader);
}

//デバッグ用GUI描画
void PlayerGun::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("PlayerGun", nullptr, ImGuiWindowFlags_None))
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
void PlayerGun::DrawDebugPrimitive()
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
        Model::Node* leftHandBone = model->FindNode("mixamorig:LeftHand");
        debugRenderer->DrawSphere(DirectX::XMFLOAT3(
            leftHandBone->worldTransform._41,
            leftHandBone->worldTransform._42,
            leftHandBone->worldTransform._43),
            leftHandRadius,
            DirectX::XMFLOAT4(1, 0, 0, 1)
        );
    }
}

//弾丸と敵の衝突処理
void PlayerGun::CollisionProjectilesVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //すべての弾丸と全ての敵を総当りで衝突処理
    int projectileCount = projectileManager.GetProjectileCount();
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < projectileCount; ++i)
    {
        Projectile* projectile = projectileManager.GetProjectile(i);

        for (int j = 0; j < enemyCount; ++j)
        {
            Enemy* enemy = enemyManager.GetEnemy(j);




            //衝突処理
            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectSphereVsCylinder(
                projectile->GetPosition(),
                projectile->GetRadius(),
                enemy->GetPosition(),
                enemy->GetRadius(),
                enemy->GetHeight(),
                outPosition))
            {
                
                if (enemy->GetAnimationChange() == 1)
                {
                    invincible_hitEffect->Play(projectile->GetPosition(), 0.3f);
                    invincible_hitEffect->SetRotation(invincible_hitEffect->GetHandle(), projectile->GetAngle());
                    return;
                }

                //ダメージを与える
                if (enemy->ApplyDamage(1, 0.1f))
                {
                    //吹き飛ばす
                    {
                        DirectX::XMFLOAT3 impulse;
                        const float power = 10.0f;
                        const DirectX::XMFLOAT3& e = enemy->GetPosition();
                        const DirectX::XMFLOAT3& p = projectile->GetPosition();
                        float vx = e.x - p.x;
                        float vz = e.z - p.z;
                        float lengthXZ = sqrtf(vx * vx + vz * vz);
                        vx /= lengthXZ;
                        vz /= lengthXZ;

                        impulse.x = vx * power;
                        impulse.y = power * 0.5f;
                        impulse.z = vx * power;

                        //enemy->AddImpulse(impulse);
                    }

                    //ヒットエフェクト再生
                    {

                        //weponNode = model->FindNode("Greadsword");


                        //DirectX::XMFLOAT3 weponbreadposition =
                        //{
                        //    weponNode->worldTransform._41,
                        //    weponNode->worldTransform._42,
                        //    weponNode->worldTransform._43
                        //};

                        hitEffect->Play(projectile->GetPosition(), 0.4f);
                        hitEffect->SetRotation(hitEffect->GetHandle(), projectile->GetAngle());
                        //DirectX::XMFLOAT3 e = enemy->GetPosition();
                        //e.y += enemy->GetHeight() * 0.5f;
                        //hitEffect->Play(projectile->GetPosition(), 0.1f);
                    }

                    //弾丸破棄
                    projectile->Destroy();
                }
            }
        }
    }
}


DirectX::XMFLOAT3 PlayerGun::GetMoveVec() const
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

//移動入力処理
bool PlayerGun::InputMove(float elapsedTime)
{
    //進行ベクトル取得
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    //移動処理
    Move(moveVec.x, moveVec.z, moveSpeed);

    //旋回処理
    Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

    //進行ベクトルが0ベクトルでない場合は入力された
    return moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
}

bool PlayerGun::InputWalk(float elapsedTime)
{
    maxMoveSpeed = 7.0f;
    moveSpeed = 7.0f;

    //run用のキーが入力されていない場合は歩き
    //進行ベクトル取得
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    //移動処理
    Move(moveVec.x, moveVec.z, moveSpeed);

    //旋回処理
    Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

    //進行ベクトルが0ベクトルでない場合は入力された
    return moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
}

bool PlayerGun::InputRun(float elapsedTime)
{
    //run用のキーが入力された場合は走り
    //進行ベクトル取得
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    //早く動くように
    if (GetKeyState(VK_SHIFT) & 0x8000)
    {

        moveVec.x *= 1.5f;
        moveVec.y *= 1.5f;
        moveVec.z *= 1.5f;

        moveSpeed = 15.0f;
        maxMoveSpeed = 15.0f;

        //移動処理
        Move(moveVec.x, moveVec.z, moveSpeed);

        //旋回処理
        Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

        //進行ベクトルが0ベクトルでない場合は入力された
        return moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
    }

    return false;
}

//プレイヤーとエネミーとの衝突処理
void PlayerGun::CollisionPlayerVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //すべての敵との総当たりで衝突処理
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        //衝突処理
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectCylinderVsCylinder(
            position,
            radius,
            height,
            enemy->GetPosition(),
            enemy->GetRadius(),
            enemy->GetHeight(),
            outPosition))
        {
            //敵の真上付近にあたったかを判定
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
            DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
            DirectX::XMFLOAT3 normal;
            DirectX::XMStoreFloat3(&normal, N);
            //上から踏んづけた場合は小ジャンプする
            if (normal.y > 0.8f)
            {
                //小ジャンプする
                Jump(jumpSpeed * 0.5f);
            }
            else
            {
                //position = position;
               /* if (GetMoveVec().x >= 0.0f)
                {
                    AddImpulse({ -5.0f, 0.0f, 0.0f });
                }
                else if (GetMoveVec().x < 0.0f)
                {
                    AddImpulse({ 5.0f, 0.0f, 0.0f });
                }
                if (GetMoveVec().z >= 0.0f)
                {
                    AddImpulse({ 0.0f, 0.0f, -5.0f });
                }
                else if (GetMoveVec().z < 0.0f)
                {
                    AddImpulse({ 0.0f, 0.0f, 5.0f });
                }

                TransitionBrake_GunState();*/
                //enemy->SetPosition(enemy->GetPosition());
            }
        }
    }
}

//ノードと敵の衝突処理
void PlayerGun::CollisionNodeVsEnemies(const char* nodeName, float nodeRadius)
{
    //ノード取得
    Model::Node* node = model->FindNode(nodeName);

    //ノード位置取得
    DirectX::XMFLOAT3 nodePosition;
    nodePosition.x = node->worldTransform._41;
    nodePosition.y = node->worldTransform._42;
    nodePosition.z = node->worldTransform._43;

    //指定のノードと全ての敵を総当りで衝突処理
    EnemyManager& enemyManager = EnemyManager::Instance();
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        //衝突処理
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectSphereVsCylinder(
            nodePosition, nodeRadius,
            enemy->GetPosition(),
            enemy->GetRadius(),
            enemy->GetHeight(),
            outPosition))
        {
            //ダメージを与える
            if (enemy->ApplyDamage(1, 0.5f))
            {
                //吹っ飛ばす
                {
                    const float power = 10.0f;
                    const DirectX::XMFLOAT3& e = enemy->GetPosition();
                    float vx = e.x - nodePosition.x;
                    float vz = e.z - nodePosition.z;
                    float lengthXZ = sqrtf(vx * vx + vz * vz);
                    vx /= lengthXZ;
                    vz /= lengthXZ;

                    DirectX::XMFLOAT3 impulse;
                    impulse.x = vx * power;
                    impulse.y = power * 0.5f;
                    impulse.z = vz * power;

                    enemy->AddImpulse(impulse);
                }
                //ヒットエフェクト再生
                {
                    DirectX::XMFLOAT3 e = enemy->GetPosition();
                    e.y += enemy->GetHeight() * 0.5f;
                    hitEffect->Play(e, 0.05f);
                }
            }
        }
    }

}

//ジャンプ入力処理
bool PlayerGun::InputJump()
{
    //ボタン入力でジャンプ（ジャンプ回数制限付き）
    //GamePad& gamePad = Input::Instance().GetGamePad();
    //if ((gamePad.GetButtonDown() & GamePad::BTN_A) || (GetAsyncKeyState(VK_SPACE) & 0x8001))
    //{
    //    //ジャンプ回数制限
    //    if (jumpCount < jumpLimit)
    //    {
    //        //ジャンプ
    //        jumpCount++;
    //        Jump(jumpSpeed);

    //        //ジャンプ入力した
    //        return true;
    //    }
    //}
    return false;
}

//弾丸入力処理
void PlayerGun::InputProjectile()
{
    //GamePad& gamePad = Input::Instance().GetGamePad();
    //
    //直進弾丸発射
    //if (gamePad.GetButtonDown() & GamePad::BTN_X)
 //
    //追尾弾丸発射
    //if (gamePad.GetButtonDown() & GamePad::BTN_Y)
    //{
    //    //前方向
    //    DirectX::XMFLOAT3 dir;
    //    dir.x = sinf(angle.y);
    //    dir.y = 0.0f;
    //    dir.z = cosf(angle.y);
    //
    //    //発射位置（プレイヤーの腰辺り）
    //    DirectX::XMFLOAT3 pos;
    //    pos.x = position.x;
    //    pos.y = position.y + height * 0.5f;
    //    pos.z = position.z;
    //
    //    //ターゲット（デフォルトではプレイヤーの前方）
    //    DirectX::XMFLOAT3 target;
    //    target.x = pos.x + dir.x * 1000.0f;
    //    target.y = pos.y + dir.y * 1000.0f;
    //    target.z = pos.z + dir.z * 1000.0f;
    //
    //    //一番近くの敵をターゲットにする
    //    float dist = FLT_MAX;
    //    EnemyManager& enemyManager = EnemyManager::Instance();
    //    int enemyCount = enemyManager.GetEnemyCount();
    //    for (int i = 0; i < enemyCount; ++i)
    //    {
    //        //敵との距離判定
    //        Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
    //        DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
    //        DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
    //        DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
    //        DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
    //        float d;
    //        DirectX::XMStoreFloat(&d, D);
    //        if (d < dist)
    //        {
    //            dist = d;
    //            target = enemy->GetPosition();
    //            target.y += enemy->GetHeight() * 0.5f;
    //        }
    //    }
    //    //発射
    //    ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
    //    projectile->Launch(dir, pos, target);
    //}

    //前方向
    DirectX::XMFLOAT3 dir;
    dir.x = sinf(angle.y);
    dir.y = 0.0f;
    dir.z = cos(angle.y);

    //発射位置（プレイヤーの腰辺り）
    DirectX::XMFLOAT3 pos;
    pos.x = position.x;
    pos.y = position.y + height * 0.5f;
    pos.z = position.z;

    //発射
    if (attackcount >= 0)
    {
        ProjectileStraite* projectile = new ProjectileStraite(&projectileManager);
        projectile->Launch(dir, pos);
        attackcount--;
    }
}

//攻撃入力処理
bool PlayerGun::InputAttack()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();

    //TODO 後で戻す
    if ((gamePad.GetButtonDown() & GamePad::BTN_B) || (mouse.GetButton() & Mouse::BTN_LEFT))
    {
        return true;
    }

    return false;
}

bool PlayerGun::InputWeapon()
{
    if ((GetAsyncKeyState(VK_SPACE) & 0x8001) && changetimer >= changetimermax)
    {
        //if (weapon == 0)    //武器なし（今はない）
        //{
        //    weapon = 1;
        //}
        //else if (weapon == 1)   //大剣
        //{
        //    weapon = 2;
        //}
        //else if (weapon == 2)   //双剣
        //{
        //    weapon = 3;
        //}
        //else if (weapon == 3)   //銃
        //{
        //    weapon = 1;
        //}

        return true;
    }
    return false;
}

bool PlayerGun::InputGuard(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();

    if ((gamePad.GetButtonDown() & GamePad::BTN_LEFT_TRIGGER) || (mouse.GetButton() & Mouse::BTN_RIGHT))
    {
        return true;
    }

    return false;
}

bool PlayerGun::InputReload()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();

    if (((gamePad.GetButtonDown() & GamePad::BTN_Y) || (GetAsyncKeyState('R') & 0x8000)) && reloadtime > 90.0f || (mouse.GetButton() & Mouse::BTN_RIGHT) && reloadtime > 90.0f)
    {
        reloadtime = 0.0f;
        return true;
    }
    return false;
}

//ダメージを受けたときに呼ばれる
void PlayerGun::OnDamaged()
{
    TransitionDamage_GunState();
}

//死亡したときに呼ばれる
void PlayerGun::OnDead()
{
    TransitionDeath_GunState();
}

//銃
void PlayerGun::TransitionIdle_GunState()
{
    statenumber = 31;

    //銃の待機に遷移
    state = State::Idle_Gun;

    //待機アニメーション再生
    model->PlayAnimation(Anim_Gun_Idle, true);
}

void PlayerGun::UpdateIdle_GunState(float elapsedTime)
{
    //武器変更入力処理
    if (InputWeapon())
    {
        //武器変更ステートへ遷移
        TransitionChange_GunState();
    }

    //歩き入力処理
    if (InputWalk(elapsedTime))
    {
        //歩きステートへ遷移
        TransitionWalk_GunState();
    }

    //走り入力処理
    if (InputRun(elapsedTime))
    {
        //走りステートへ遷移
        TransitionRun_GunState();
    }


    //攻撃入力処理
    if (InputAttack() && attackcount >= 0)
    {
        //攻撃ステートへ遷移
        TransitionAttack_GunState();
    }

    //リロード入力処理
    if (InputReload())
    {
        TransitionReload_GunState();
    }
}

void PlayerGun::TransitionWalk_GunState()
{
    statenumber = 32;

    //銃の歩きに遷移
    state = State::Walk_Gun;

    run_slow->Play(true, 1.0f);
    //歩きモーションの再生
    model->PlayAnimation(Anim_Gun_Dash, true);
}

void PlayerGun::UpdateWalk_GunState(float elapsedTime)
{
    //武器変更入力処理
    if (InputWeapon())
    {
        run_slow->Stop();
        //武器変更ステートへ遷移
        TransitionChange_GunState();
    }

    //移動入力処理
    if (!InputWalk(elapsedTime))
    {
        run_slow->Stop();
        //待機ステートへ遷移
        TransitionIdle_GunState();
    }

    //走り入力処理
    if (InputRun(elapsedTime))
    {
        run_slow->Stop();
        //走りステートへ遷移
        TransitionRun_GunState();
    }

    //攻撃入力処理
    if (InputAttack() && attackcount >= 0)
    {
        run_slow->Stop();
        //攻撃ステートへ遷移
        TransitionAttack_GunState();
    }

    //リロード入力処理
    if (InputReload())
    {
        run_slow->Stop();
        TransitionReload_GunState();
    }
}

void PlayerGun::TransitionRun_GunState()
{
    statenumber = 33;
    state = State::Run_Gun;

    //走りモーションを再生
    run_fast->Play(true, 1.0f);
    model->PlayAnimation(Anim_Gun_Dash_02, true);
}

void PlayerGun::UpdateRun_GunState(float elapsedTime)
{
    //武器変更入力処理
    if (InputWeapon())
    {
        run_fast->Stop();
        //武器変更ステートへ遷移
        TransitionChange_GunState();
    }

    //走り入力処理
    if (!InputRun(elapsedTime))
    {
        run_fast->Stop();
        //ブレーキステートへ遷移
        TransitionBrake_GunState();

        //待機ステートへ遷移
        //TransitionIdle_GunState();
    }

    //攻撃入力処理
    if (InputAttack() && attackcount >= 0)
    {
        run_fast->Stop();
        //攻撃ステートへ遷移
        TransitionAttack_GunState();
    }

    //リロード入力処理
    if (InputReload())
    {
        run_fast->Stop();
        TransitionReload_GunState();
    }
}

void PlayerGun::TransitionAttack_GunState()
{
    statenumber = 36;
    state = State::Attack_Gun;

    //攻撃アニメーション再生
    model->PlayAnimation(Anim_Gun_Attack, false);
}

void PlayerGun::UpdateAttack_GunState(float elapsedTime)
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
    float cameraFrontY = cameraFront.y;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraRightLength > 0.0f)
    {
        //単位ベクトル化
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
        cameraFrontY /= cameraFrontLength;
    }

    //スティックの水平入力値をカメラ右方向に反映し、
    //スティックの垂直入力値をカメラ前方向に反映し、
    //進行ベクトルを計算する
    DirectX::XMFLOAT3 vec;
    vec.x = (cameraFrontX * 1.0f);
    vec.z = (cameraFrontZ * 1.0f);
    //Y軸方向には移動しない
    vec.y = 0.0f;

    //進行ベクトル取得
    //DirectX::XMFLOAT3 moveVec = GetMoveVec();

    //旋回処理
    Turn(elapsedTime, vec.x, vec.z, turnSpeed);

    UpdateTransform();

    {
        //カメラコントローラー更新処理
        DirectX::XMFLOAT3 target = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetWeapon())->GetPosition();
        target.y += 0.5f;										//プレイヤーの腰辺りにターゲットを設定
        cameraController->SetTarget(target);
        cameraController->Update(elapsedTime);


        // 0.1から0.8の範囲の乱数生成
        //double randomNumber = getRandomFromRange(0.1, 1.0);
        float randomNumber = getRandomInRange(-0.2f, 0.5f);

        //前方向
        DirectX::XMFLOAT3 dir;
        dir.x = sinf(angle.y);
        dir.y = angle.x;
        dir.z = cos(angle.y);

        //発射位置（プレイヤーの腰辺り）
        DirectX::XMFLOAT3 pos;
        pos.x = position.x + randomNumber;
        pos.y = position.y + height * 0.5f + randomNumber;
        pos.z = position.z + randomNumber;

        //発射
        if (attackcount >= 0 && rate > 3.0f)
        {
            ProjectileStraite* projectile = new ProjectileStraite(&projectileManager);
            projectile->Launch(dir, pos);
            shot->Play(false, 1.0f);
            shotEffect->Play({ pos.x, pos.y,pos.z }, 0.05f);
            attackcount--;
            rate = 0.0f;
        }
    }


    //攻撃アニメーション終了後
    if (!model->IsPlayAnimation() || attackcount <= 0)
    {
        shot->Stop();
        //待機ステートへ遷移
        TransitionIdle_GunState();
    }


}

void PlayerGun::TransitionDamage_GunState()
{
    statenumber = 37;
    state = State::Damage_Gun;

    run_slow->Stop();
    run_fast->Stop();

    //ダメージアニメーション再生
    model->PlayAnimation(Anim_Gun_Damaged, false);
}

void PlayerGun::UpdateDamage_GunState(float elapsedTime)
{
    //ダメージアニメーションが終わったら待機ステートへ遷移
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_GunState();
    }
}

void PlayerGun::TransitionDeath_GunState()
{
    statenumber = 38;
    state = State::Death_Gun;
    run_slow->Stop();
    run_fast->Stop();

    //ダメージアニメーション再生
    model->PlayAnimation(Anim_Gun_Death, false);
}

void PlayerGun::UpdateDeath_GunState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        //アニメーション再生が終わったらゲームオーバーになるようにする
    }
}

void PlayerGun::TransitionDodge_GunState()
{
    statenumber = 39;
    state = State::Dodge_Gun;

    //無敵時間設定
    invincibleTimer = 1.0f;

    //とりあえずフリップアニメーションを再生
    model->PlayAnimation(Anim_Gun_Dodge, false);
}

void PlayerGun::UpdateDodge_GunState(float elapsedTime)
{
    //フリップアニメーション終了後、待機ステートへ遷移
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_GunState();
    }
}

void PlayerGun::TransitionReload_GunState()
{
    statenumber = 40;
    state = State::Reload_Gun;

    //リロードアニメーション再生
    model->PlayAnimation(Anim_Gun_Reload, false);
    reload->Play(false, 1.0f);
}

void PlayerGun::UpdateReload_GunState(float elapsedTime)
{
    //リロードにかかる時間
    //reloadtime--;

    //移動入力が入ったら中断して移動
    //if (InputWalk(elapsedTime))
    //{
    //    TransitionWalk_GunState();
    //}
    //if (InputRun(elapsedTime))
    //{
    //    TransitionRun_GunState();
    //}

    if (!model->IsPlayAnimation() && reloadtime >= 60.0f)
    {
        //弾薬数を元に戻す
        reloadtime = 0.0f;
        attackcount = 18;

        //アニメーションが終わったら待機状態に戻る
        TransitionIdle_GunState();
    }

}

void PlayerGun::TransitionChange_GunState()
{
    state = State::Change_Gun;

    weapon = 0;
    changetime = 0.0f;

    //武器変更モーション再生
    model->PlayAnimation(Anim_Gun_Reload, false);
}

void PlayerGun::UpdateChange_GunState(float elapsedTime)
{
    projectileManager.Clear();
    PlayerManager::Instance().SetStartAnimation(true);
    DirectX::XMFLOAT3 position_gun = position;
    DirectX::XMFLOAT3 angle_gun = angle;
    PlayerManager::Instance().SetWeaponFlag(true);
    PlayerManager::Instance().SetBeforeWeapon(2);
    PlayerManager::Instance().SetWeapon(3);
    Player* player = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetWeapon());
    player->SetPosition(position_gun);
    player->SetAngle(angle_gun);
    player->SetChangeTimer(0.0f);
    player->SetHP(health);
    TransitionIdle_GunState();
    position = { 0,1000000000000.0f,0 };
    //武器変更モーションの再生が終われば次の武器へ変更
    //if (!model->IsPlayAnimation())
    //{
    //    //
    //}
}

void PlayerGun::TransitionBrake_GunState()
{
    state = State::Brake_Gun;

    braketime = 0.0f;
    run_slow->Stop();
    model->PlayAnimation(Anim_Gun_Brake, false);
    brake->Play(false, 1.0f);
}

void PlayerGun::UpdateBrake_GunState(float elapsedTime)
{
    if (!model->IsPlayAnimation() || braketime > 30.0f)
    {
        braketime = 0.0f;
        TransitionIdle_GunState();
    }
}
