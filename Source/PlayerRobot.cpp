#include <imgui.h>
#include "PlayerRobot.h"
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraite.h"
#include "ProjectileHoming.h"
#include "LightManager.h"

static PlayerRobot* instance = nullptr;

//インスタンス取得
PlayerRobot& PlayerRobot::Instance()
{
    return *instance;
}

//コンストラクタ
PlayerRobot::PlayerRobot()
{
    //インクレディブルのロード
    //model = new Model("Data/Model/Mr.Incredible/Mr.Incredible.mdl");
    //ロボットのロード
    //model = new Model("Data/Model/Jammo/Jammo.mdl");

    //モデルのロード
    model = new Model("Data/Model/Player/dualsword_player.mdl");
    //model = new Model("Data/Model/Player/greatsword_player.mdl");
    //model = new Model("Data/Model/Player/gun_player.mdl");

    //体力の設定
    health = 15;
    maxHealth = 15;

    //モデルが大きいのでスケーリング
    scale.x = scale.y = scale.z = 0.02f;

    //ヒットエフェクト読み込み
    hitEffect = new Effect("Data/Effect/Hit.efk");

    attackcount = 0;
    weapon = 0;
    attackcount = 0;
    bullets = 60;
    totalattackcount = 0;
    changetime = 0.0f;

    //待機ステートへ遷移(大剣)
    TransitionIdle_GreatState();
}

//デストラクタ
PlayerRobot::~PlayerRobot()
{
    delete hitEffect;

    delete model;
}

//更新処理
void PlayerRobot::Update(float elapsedTime)
{
    //ステート毎の処理
    switch (state)
    {
        //武器未装着
    case State::Idle:
        UpdateIdleState(elapsedTime);
        break;
    case State::Walk:
        UpdateWalkState(elapsedTime);
        break;
    case State::Run:
        UpdateRunState(elapsedTime);
        break;
    case State::Jump:
        UpdateJumpState(elapsedTime);
        break;
    case State::Land:
        UpdateLandState(elapsedTime);
        break;
    case State::Attack:
        UpdateAttackState(elapsedTime);
        break;
    case State::Damage:
        UpdateDamageState(elapsedTime);
        break;
    case State::Death:
        UpdateDeathState(elapsedTime);
        break;
    case State::Revive:
        UpdateReviveState(elapsedTime);
        break;
    case State::Dodge:
        UpdateDodgeState(elapsedTime);
        break;
    case State::Guard:
        UpdateGuardState(elapsedTime);
        break;
    case State::Change:
        UpdateChangeState(elapsedTime);
        break;

        //大剣
    case State::Idle_Great:
        UpdateIdle_GreatState(elapsedTime);
        break;
    case State::Walk_Great:
        UpdateWalk_GreatState(elapsedTime);
        break;
    case State::Run_Great:
        UpdateRun_GreatState(elapsedTime);
        break;
    case State::Jump_Great:
        UpdateJump_GreatState(elapsedTime);
        break;
    case State::Land_Great:
        UpdateLand_GreatState(elapsedTime);
        break;
    case State::Attack_Great:
        UpdateAttack_GreatState(elapsedTime);
        break;
    case State::Damage_Great:
        UpdateDamage_GreatState(elapsedTime);
        break;
    case State::Death_Great:
        UpdateDeath_GreatState(elapsedTime);
        break;
    case State::Guard_Great:
        UpdateGuard_GreatState(elapsedTime);
        break;
    case State::Dodge_Great:
        UpdateDamage_GreatState(elapsedTime);
        break;
    case State::Change_Great:
        UpdateChange_GreatState(elapsedTime);
        break;

        //双剣
    case State::Idle_Dual:
        UpdateIdle_DualState(elapsedTime);
        break;
    case State::Walk_Dual:
        UpdateWalk_DualState(elapsedTime);
        break;
    case State::Run_Dual:
        UpdateRun_DualState(elapsedTime);
        break;
    case State::Jump_Dual:
        UpdateJump_DualState(elapsedTime);
        break;
    case State::Land_Dual:
        UpdateLand_DualState(elapsedTime);
        break;
    case State::Attack_Dual:
        UpdateAttack_DualState(elapsedTime);
        break;
    case State::Damage_Dual:
        UpdateDamage_DualState(elapsedTime);
        break;
    case State::Death_Dual:
        UpdateDeath_DualState(elapsedTime);
        break;
    case State::Dodge_Dual:
        UpdateDodge_DualState(elapsedTime);
        break;
    case State::Change_Dual:
        UpdateChange_DualState(elapsedTime);
        break;

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
    case State::Jump_Gun:
        UpdateJump_GunState(elapsedTime);
        break;
    case State::Land_Gun:
        UpdateLand_GunState(elapsedTime);
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
    case State::Change_Gun:
        UpdateChange_GunState(elapsedTime);
        break;

    }



    //光の追従
    //LightManager::Instance().GetLight(1)->SetPosition({position.x, position.y + 3.0f, position.z});

    //武器変更クールタイム
    changetime++;

    //速力処理更新
    UpdateVelocity(elapsedTime);

    //無敵時間更新処理
    UpdateInvincibleTimer(elapsedTime);

    //弾丸更新処理
    projectileManager.Update(elapsedTime);

    //プレイヤーと敵との衝突処理
    CollisionPlayerVsEnemies();

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
}

//描画処理
void PlayerRobot::Render(RenderContext& rc, ModelShader* shader)
{
    rc.toonData.edgeWidth = 0.01f;

    shader->Draw(rc, model);
    //↑描画する際に計算されたモデルの行列をこの中で利用

    //弾丸描画処理
    projectileManager.Render(rc, shader);
}

//デバッグ用GUI描画
void PlayerRobot::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("PlayerRobot", nullptr, ImGuiWindowFlags_None))
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
void PlayerRobot::DrawDebugPrimitive()
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
void PlayerRobot::CollisionProjectilesVsEnemies()
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
                //ダメージを与える
                if (enemy->ApplyDamage(1, 0.5f))
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

                        enemy->AddImpulse(impulse);
                    }

                    //ヒットエフェクト再生
                    {
                        DirectX::XMFLOAT3 e = enemy->GetPosition();
                        e.y += enemy->GetHeight() * 0.5f;
                        hitEffect->Play(e);
                    }

                    //弾丸破棄
                    projectile->Destroy();
                }
            }
        }
    }
}


//着地したときに呼ばれる
void PlayerRobot::OnLanding()
{
    jumpCount = 0;

    //下方向の速力が一定以上なら着地ステートへ
    if (velocity.y < gravity * 5.0f)
    {
        //ダメージ、死亡ステート時は着地したときにステート遷移しないようにする
        if (state != State::Damage && state != State::Death)
        {
            //着地ステートへ遷移
            if (weapon == 0)
            {
                TransitionLandState();
            }
            else if (weapon == 1)
            {
                TransitionIdle_GreatState();
            }
            else if (weapon == 2)
            {
                TransitionIdle_DualState();
            }
            else if (weapon == 3)
            {
                TransitionIdle_GunState();
            }

        }
    }
}

DirectX::XMFLOAT3 PlayerRobot::GetMoveVec() const
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
bool PlayerRobot::InputMove(float elapsedTime)
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

bool PlayerRobot::InputWalk(float elapsedTime)
{
    maxMoveSpeed = 5.0f;
    moveSpeed = 5.0f;

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

bool PlayerRobot::InputRun(float elapsedTime)
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
}

bool PlayerRobot::InputDodge(float elpsedTime)
{
    //コントロールキーが押されたら

    return false;
}

//プレイヤーとエネミーとの衝突処理
void PlayerRobot::CollisionPlayerVsEnemies()
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
                position = position;
                //押し出し後の位置設定
                //enemy->SetPosition(outPosition);
            }
        }
    }
}

//ノードと敵の衝突処理
void PlayerRobot::CollisionNodeVsEnemies(const char* nodeName, float nodeRadius)
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
                    hitEffect->Play(e);
                }
            }
        }
    }
}

//ジャンプ入力処理
bool PlayerRobot::InputJump()
{
    //ボタン入力でジャンプ（ジャンプ回数制限付き）
    GamePad& gamePad = Input::Instance().GetGamePad();
    if ((gamePad.GetButtonDown() & GamePad::BTN_A) || (GetAsyncKeyState(VK_SPACE) & 0x8001))
    {
        //ジャンプ回数制限
        if (jumpCount < jumpLimit)
        {
            //ジャンプ
            jumpCount++;
            Jump(jumpSpeed);

            //ジャンプ入力した
            return true;
        }
    }
    return false;
}

//弾丸入力処理
void PlayerRobot::InputProjectile()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    //直進弾丸発射
    if (gamePad.GetButtonDown() & GamePad::BTN_X)
    {
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
        ProjectileStraite* projectile = new ProjectileStraite(&projectileManager);
        projectile->Launch(dir, pos);
    }
    //追尾弾丸発射
    if (gamePad.GetButtonDown() & GamePad::BTN_Y)
    {
        //前方向
        DirectX::XMFLOAT3 dir;
        dir.x = sinf(angle.y);
        dir.y = 0.0f;
        dir.z = cosf(angle.y);

        //発射位置（プレイヤーの腰辺り）
        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y + height * 0.5f;
        pos.z = position.z;

        //ターゲット（デフォルトではプレイヤーの前方）
        DirectX::XMFLOAT3 target;
        target.x = pos.x + dir.x * 1000.0f;
        target.y = pos.y + dir.y * 1000.0f;
        target.z = pos.z + dir.z * 1000.0f;

        //一番近くの敵をターゲットにする
        float dist = FLT_MAX;
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();
        for (int i = 0; i < enemyCount; ++i)
        {
            //敵との距離判定
            Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
            DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
            float d;
            DirectX::XMStoreFloat(&d, D);
            if (d < dist)
            {
                dist = d;
                target = enemy->GetPosition();
                target.y += enemy->GetHeight() * 0.5f;
            }
        }
        //発射
        ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
        projectile->Launch(dir, pos, target);
    }
}

//攻撃入力処理
bool PlayerRobot::InputAttack()
{
    attackcount++;

    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();

    //TODO 後で戻す
    //if ((gamePad.GetButtonDown() & GamePad::BTN_B) || (mouse.GetButton() & Mouse::BTN_LEFT))
    //{
    //    return true;
    //}

    return false;
}

bool PlayerRobot::InputWeapon()
{
    if (GetAsyncKeyState('F') & 0x8000 && changetime >= 120.0f)
    {
        if (weapon == 0)    //武器なし（今はない）
        {
            weapon = 1;
        }
        else if (weapon == 1)   //大剣
        {
            weapon = 2;
        }
        else if (weapon == 2)   //双剣
        {
            weapon = 3;
        }
        else if (weapon == 3)   //銃
        {
            weapon = 1;
        }

        return true;
    }
    return false;
}

bool PlayerRobot::InputGuard(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();

    if ((gamePad.GetButtonDown() & GamePad::BTN_LEFT_TRIGGER) || (mouse.GetButton() & Mouse::BTN_RIGHT))
    {
        return true;
    }

    return false;
}

bool PlayerRobot::InputReload()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    if ((gamePad.GetButtonDown() & GamePad::BTN_Y) || (GetAsyncKeyState('R') & 0x8000))
    {
        return true;
    }
    return false;
}

//ダメージを受けたときに呼ばれる
void PlayerRobot::OnDamaged()
{
    //ダメージステートへ遷移
    if (weapon == 0)
    {
        TransitionDamageState();
    }
    else if (weapon == 1)
    {
        TransitionDamage_GreatState();
    }
    else if (weapon == 2)
    {
        TransitionDamage_DualState();
    }
    else if (weapon == 3)
    {
        TransitionDamage_GunState();
    }
}

//死亡したときに呼ばれる
void PlayerRobot::OnDead()
{
    //死亡ステートへ遷移
    if (weapon == 0)
    {
        TransitionDeathState();
    }
    else if (weapon == 1)
    {
        TransitionDeath_GreatState();
    }
    else if (weapon == 2)
    {
        TransitionDeath_DualState();
    }
    else if (weapon == 3)
    {
        TransitionDeath_GunState();
    }
}

//待機ステートへ遷移
void PlayerRobot::TransitionIdleState()
{
    state = State::Idle;

    //待機アニメーション再生
    model->PlayAnimation(Anim_Dual_Idle, true);
}

//待機ステート更新処理
void PlayerRobot::UpdateIdleState(float elapsedTime)
{
    statenumber = 1;
    //移動入力処理
    //if (InputMove(elapsedTime))
    //{
    //    //移動ステートへ遷移
    //    TransitionMoveState();
    //}

    //あるき入力処理
    if (InputWalk(elapsedTime))
    {
        //歩きステートへ遷移
        TransitionWalkState();
    }

    //走り入力処理
    if (InputRun(elapsedTime))
    {
        //走りステートへ遷移
        TransitionRunState();
    }

    //ジャンプ入力処理
    if (InputJump())
    {
        //ジャンプステートへ遷移
        TransitionJumpState();
    }

    //弾丸入力処理
    InputProjectile();

    //攻撃入力処理
    if (InputAttack())
    {
        //攻撃ステートへ遷移
        TransitionAttackState();
    }

    //武器変更入力処理
    if (InputWeapon())
    {
        //武器変更ステートへ遷移
        TransitionChangeState();
    }

    //回避入力処理
    if (InputDodge(elapsedTime))
    {
        //回避ステートへ遷移
        TransitionDodgeState();
    }

    //ガード入力処理
    if (InputGuard(elapsedTime))
    {
        //ガードステートへ遷移
        TransitionGuardState();
    }
}

//移動ステートへ遷移
void PlayerRobot::TransitionMoveState()
{
    state = State::Move;

    //走りアニメーション再生
    model->PlayAnimation(Anim_Dual_Dash, true);
}

//移動ステート更新処理
void PlayerRobot::UpdateMoveState(float elapsedTime)
{
    //移動入力処理
    if (!InputMove(elapsedTime))
    {
        //待機ステートへ遷移
        TransitionIdleState();
    }

    //武器変更入力処理
    if (InputWeapon())
    {
        //武器変更ステートへ遷移
        TransitionChangeState();
    }

    //ジャンプ入力処理
    if (InputJump())
    {
        //ジャンプステートへ遷移
        TransitionJumpState();
    }

    //弾丸入力処理
    InputProjectile();

    //攻撃入力処理
    if (InputAttack())
    {
        //攻撃ステートへ遷移
        TransitionAttackState();
    }

}

//ジャンプステートへ遷移
void PlayerRobot::TransitionJumpState()
{
    state = State::Jump;

    //ジャンプアニメーション再生
    model->PlayAnimation(Anim_Jump, false);
}

//ジャンプステート更新処理
void PlayerRobot::UpdateJumpState(float elapsedTime)
{
    //あるき入力処理
    if (InputWalk(elapsedTime))
    {
        //歩きステートへ遷移
        TransitionWalkState();
    }

    //走り入力処理
    if (InputRun(elapsedTime))
    {
        //走りステートへ遷移
        TransitionRunState();
    }

    //ジャンプ入力処理
    if (InputJump())
    {
        //ジャンプステートへ遷移
        TransitionJumpState();
    }

    //弾丸入力処理
    InputProjectile();

    //攻撃入力処理
    if (InputAttack())
    {
        //攻撃ステートへ遷移
        TransitionAttackState();
    }


    //回避入力処理
    //if (InputDodge(elapsedTime))
    //{
    //    //回避ステートへ遷移
    //    TransitionDodgeState();
    //}

    //if (weapon == 0)
    //{
    //    //ガード入力処理
    //    InputGuard();
    //}

    //ジャンプ入力処理
    if (InputJump())
    {
        //ジャンプフリップアニメーション再生
        model->PlayAnimation(Anim_Jump_Flip, false);
    }
    //ジャンプアニメーション終了後
    if (!model->IsPlayAnimation())
    {
        //落下アニメーション再生
        model->PlayAnimation(Anim_Falling, true);
    }
}

//着地ステートへ遷移
void PlayerRobot::TransitionLandState()
{
    state = State::Land;

    //着地アニメーション再生
    model->PlayAnimation(Anim_Landing, false);
}

//着地ステート更新処理
void PlayerRobot::UpdateLandState(float elapsedTIme)
{
    //着地アニメーション終了後
    if (!model->IsPlayAnimation())
    {
        //待機ステートへ遷移
        TransitionIdleState();
    }
}

//攻撃ステートへ遷移
void PlayerRobot::TransitionAttackState()
{
    state = State::Attack;

    //攻撃アニメーション再生
    model->PlayAnimation(Anim_Attack, false);
}

//攻撃ステート更新処理
void PlayerRobot::UpdateAttackState(float elapsedTime)
{
    //攻撃アニメーション終了後
    if (!model->IsPlayAnimation())
    {
        //待機ステートへ遷移
        TransitionIdleState();
    }

    //任意のアニメーション再生区間でのみ衝突判定処理をする
    float animationTime = model->GetCurrentAnimationSeconds();
    attackCollisionFlag = animationTime >= 0.3f && animationTime <= 0.4f;
    if (attackCollisionFlag)
    {
        //左手ノードとエネミーの衝突処理
        CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);
    }
}

//ダメージステートへ遷移
void PlayerRobot::TransitionDamageState()
{
    state = State::Damage;

    //ダメージアニメーション再生
    model->PlayAnimation(Anim_GetHit1, false);
}

//ダメージステート更新処理
void PlayerRobot::UpdateDamageState(float elapsedTime)
{
    //ダメージアニメーションが終わったら待機ステートへ遷移
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}

//死亡ステートへ遷移
void PlayerRobot::TransitionDeathState()
{
    state = State::Death;

    //ダメージアニメーション再生
    model->PlayAnimation(Anim_Death, false);
}

//死亡ステート更新処理
void PlayerRobot::UpdateDeathState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        //ボタンを押したら復活ステートへ遷移
        GamePad& gamePad = Input::Instance().GetGamePad();
        if (gamePad.GetButtonDown() & GamePad::BTN_A)
        {
            TransitionReviveState();
        }
    }
}

//復活ステートへ遷移
void PlayerRobot::TransitionReviveState()
{
    state = State::Revive;

    //体力回復
    health = maxHealth;

    //復活アニメーション再生
    model->PlayAnimation(Anim_Revive, false);
}

//復活ステート更新処理
void PlayerRobot::UpdateReviveState(float elapsedTime)
{
    //復活アニメーション終了後に待機ステートへ遷移
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}

void PlayerRobot::TransitionDodgeState()
{
    state = State::Dodge;

    //無敵時間設定
    invincibleTimer = 1.0f;

    //とりあえずフリップアニメーションを再生
    model->PlayAnimation(Anim_Jump_Flip, false);
}

void PlayerRobot::UpdateDodgeState(float elapsedTime)
{
    //フリップアニメーション終了後、待機ステートへ遷移
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}

void PlayerRobot::TransitionWalkState()
{
    state = State::Walk;

    //歩きモーションを再生
    model->PlayAnimation(Anim_Dual_Dash, true);
}

void PlayerRobot::UpdateWalkState(float elapsedTime)
{
    statenumber = 2;
    //移動入力処理
    if (!InputWalk(elapsedTime))
    {
        //待機ステートへ遷移
        TransitionIdleState();
    }

    //走り入力処理
    if (InputRun(elapsedTime))
    {
        //走りステートへ遷移
        TransitionRunState();
    }

    //ジャンプ入力処理
    if (InputJump())
    {
        //ジャンプステートへ遷移
        TransitionJumpState();
    }

    //弾丸入力処理
    InputProjectile();

    //攻撃入力処理
    if (InputAttack())
    {
        //攻撃ステートへ遷移
        TransitionAttackState();
    }

    //武器変更入力処理
    if (InputWeapon())
    {
        //武器変更ステートへ遷移
        TransitionChangeState();
    }

    //回避入力処理
    if (InputDodge(elapsedTime))
    {
        //回避ステートへ遷移
        TransitionDodgeState();
    }
    //ガード入力処理
    if (InputGuard(elapsedTime))
    {
        //ガードステートへ遷移
        TransitionGuardState();
    }
}

void PlayerRobot::TransitionRunState()
{
    state = State::Run;

    //走りモーションを再生
    model->PlayAnimation(Anim_Dual_Dash, true);
}

void PlayerRobot::UpdateRunState(float elapsedTime)
{
    statenumber = 3;

    //走り入力処理
    if (!InputRun(elapsedTime))
    {
        //待機ステートへ遷移
        TransitionIdleState();
    }
    //ジャンプ入力処理
    if (InputJump())
    {
        //ジャンプステートへ遷移
        TransitionJumpState();
    }

    //弾丸入力処理
    InputProjectile();

    //攻撃入力処理
    if (InputAttack())
    {
        //攻撃ステートへ遷移
        TransitionAttackState();
    }

    //武器変更入力処理
    if (InputWeapon())
    {
        //武器変更ステートへ遷移
        TransitionChangeState();
    }

    //回避入力処理
    if (InputDodge(elapsedTime))
    {
        //回避ステートへ遷移
        TransitionDodgeState();
    }

    //ガード入力処理
    if (InputGuard(elapsedTime))
    {
        //ガードステートへ遷移
        TransitionGuardState();
    }
}

void PlayerRobot::TransitionGuardState()
{
    state = State::Guard;

    //ガードモーション再生
    model->PlayAnimation(Anim_GetHit1, true);
}

void PlayerRobot::UpdateGuardState(float elapsedTime)
{
    statenumber = 4;

    if (!InputGuard(elapsedTime))
    {
        //待機ステートへ遷移
        TransitionIdleState();
    }

    //歩き入力処理
    if (InputWalk(elapsedTime))
    {
        //歩きステートへ遷移
        TransitionWalkState();
    }

    //走り入力処理
    if (InputRun(elapsedTime))
    {
        //走りステートへ遷移
        TransitionRunState();
    }
    //ジャンプ入力処理
    if (InputJump())
    {
        //ジャンプステートへ遷移
        TransitionJumpState();
    }

    //弾丸入力処理
    InputProjectile();

    //攻撃入力処理
    if (InputAttack())
    {
        //攻撃ステートへ遷移
        TransitionAttackState();
    }

    //武器変更入力処理
    if (InputWeapon())
    {
        //武器変更ステートへ遷移
        TransitionChangeState();
    }

    //回避入力処理
    if (InputDodge(elapsedTime))
    {
        //回避ステートへ遷移
        TransitionDodgeState();
    }
}

void PlayerRobot::TransitionChangeState()
{
    state = State::Change;

    weapon = 1;
    changetime = 0.0f;

    //武器変更モーション再生
    model->PlayAnimation(Anim_Jump_Flip, false);
}

void PlayerRobot::UpdateChangeState(float elapsedTime)
{
    //武器変更モーションの再生が終われば次の武器へ変更
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_GreatState();
    }
}

//大剣
void PlayerRobot::TransitionIdle_GreatState()
{
    statenumber = 11;

    //大剣の待機に遷移
    state = State::Idle_Great;

    //待機アニメーション再生
    model->PlayAnimation(Anim_Idle, true);
}

void PlayerRobot::UpdateIdle_GreatState(float elapsedTime)
{
    //武器変更入力処理
    if (InputWeapon())
    {
        //武器変更ステートへ遷移
        TransitionChange_GreatState();
    }

    //歩き入力処理
    if (InputWalk(elapsedTime))
    {
        //歩きステートへ遷移
        TransitionWalk_GreatState();
    }

    //走り入力処理
    if (InputRun(elapsedTime))
    {
        //走りステートへ遷移
        TransitionRun_GreatState();
    }

    //ジャンプ入力処理
    if (InputJump())
    {
        //ジャンプステートへ遷移
        TransitionJump_GreatState();
    }

    //攻撃入力処理
    if (InputAttack())
    {
        //攻撃ステートへ遷移
        TransitionAttack_GreatState();
    }

    //回避入力処理
    if (InputDodge(elapsedTime))
    {
        //回避ステートへ遷移
        TransitionDodge_GreatState();
    }

    //ガード入力処理
    if (InputGuard(elapsedTime))
    {
        //ガードステートへ遷移
        TransitionGuard_GreatState();
    }
}


void PlayerRobot::TransitionWalk_GreatState()
{
    statenumber = 12;

    //大剣の歩きに遷移
    state = State::Walk_Great;

    //歩きモーションの再生
    model->PlayAnimation(Anim_Walking, true);
}

void PlayerRobot::UpdateWalk_GreatState(float elapsedTime)
{
    //武器変更入力処理
    if (InputWeapon())
    {
        //武器変更ステートへ遷移
        TransitionChange_GreatState();
    }

    //移動入力処理
    if (!InputWalk(elapsedTime))
    {
        //待機ステートへ遷移
        TransitionIdle_GreatState();
    }

    //走り入力処理
    if (InputRun(elapsedTime))
    {
        //走りステートへ遷移
        TransitionRun_GreatState();
    }

    //ジャンプ入力処理
    if (InputJump())
    {
        //ジャンプステートへ遷移
        TransitionJump_GreatState();
    }

    //攻撃入力処理
    if (InputAttack())
    {
        //攻撃ステートへ遷移
        TransitionAttack_GreatState();
    }

    //回避入力処理
    if (InputDodge(elapsedTime))
    {
        //回避ステートへ遷移
        TransitionDodge_GreatState();
    }

    //ガード入力処理
    if (InputGuard(elapsedTime))
    {
        //ガードステートへ遷移
        TransitionGuard_GreatState();
    }
}

void PlayerRobot::TransitionRun_GreatState()
{
    statenumber = 13;
    state = State::Run_Great;

    //走りモーションを再生
    model->PlayAnimation(Anim_Running, true);
}

void PlayerRobot::UpdateRun_GreatState(float elapsedTime)
{
    //武器変更入力処理
    if (InputWeapon())
    {
        //武器変更ステートへ遷移
        TransitionChange_GreatState();
    }

    //走り入力処理
    if (!InputRun(elapsedTime))
    {
        //待機ステートへ遷移
        TransitionIdle_GreatState();
    }
    //ジャンプ入力処理
    if (InputJump())
    {
        //ジャンプステートへ遷移
        TransitionJump_GreatState();
    }

    //攻撃入力処理
    if (InputAttack())
    {
        //攻撃ステートへ遷移
        TransitionAttack_GreatState();
    }

    //回避入力処理
    if (InputDodge(elapsedTime))
    {
        //回避ステートへ遷移
        TransitionDodge_GreatState();
    }

    //ガード入力処理
    if (InputGuard(elapsedTime))
    {
        //ガードステートへ遷移
        TransitionGuard_GreatState();
    }
}

void PlayerRobot::TransitionJump_GreatState()
{
    statenumber = 14;
    state = State::Jump_Great;

    //ジャンプアニメーション再生
    model->PlayAnimation(Anim_Jump, false);
}

void PlayerRobot::UpdateJump_GreatState(float elapsedTime)
{
    //武器変更入力処理
    if (InputWeapon())
    {
        //武器変更ステートへ遷移
        TransitionChange_GreatState();
    }

    //歩き入力処理
    if (InputWalk(elapsedTime))
    {
        //歩きステートへ遷移
        TransitionWalk_GreatState();
    }

    //走り入力処理
    if (InputRun(elapsedTime))
    {
        //走りステートへ遷移
        TransitionRun_GreatState();
    }

    //攻撃入力処理
    if (InputAttack())
    {
        //攻撃ステートへ遷移
        TransitionAttack_GreatState();
    }

    //ジャンプアニメーション終了後
    if (!model->IsPlayAnimation())
    {
        //落下アニメーション再生
        model->PlayAnimation(Anim_Falling, true);
    }
}

void PlayerRobot::TransitionLand_GreatState()
{
    statenumber = 15;
    state = State::Land_Great;

    //着地アニメーション再生
    model->PlayAnimation(Anim_Landing, false);
}

void PlayerRobot::UpdateLand_GreatState(float elapsedTIme)
{
    //着地アニメーション終了後
    if (!model->IsPlayAnimation())
    {
        //待機ステートへ遷移
        TransitionIdle_GreatState();
    }
}

void PlayerRobot::TransitionAttack_GreatState()
{
    statenumber = 16;
    state = State::Attack_Great;

    //攻撃アニメーション再生
    model->PlayAnimation(Anim_Attack, false);
}

void PlayerRobot::UpdateAttack_GreatState(float elapsedTime)
{
    //攻撃アニメーション終了後
    if (!model->IsPlayAnimation())
    {
        //待機ステートへ遷移
        TransitionIdle_GreatState();
    }

    //任意のアニメーション再生区間でのみ衝突判定処理をする
    float animationTime = model->GetCurrentAnimationSeconds();
    attackCollisionFlag = animationTime >= 0.3f && animationTime <= 0.4f;
    if (attackCollisionFlag)
    {
        //左手ノードとエネミーの衝突処理
        CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);
    }
}

void PlayerRobot::TransitionDamage_GreatState()
{
    statenumber = 17;
    state = State::Damage_Great;

    //ダメージアニメーション再生
    model->PlayAnimation(Anim_GetHit1, false);
}

void PlayerRobot::UpdateDamage_GreatState(float elapsedTime)
{
    //ダメージアニメーションが終わったら待機ステートへ遷移
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_GreatState();
    }
}

void PlayerRobot::TransitionDeath_GreatState()
{
    statenumber = 18;
    state = State::Death_Great;

    //ダメージアニメーション再生
    model->PlayAnimation(Anim_Death, false);
}

void PlayerRobot::UpdateDeath_GreatState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        //アニメーション再生が終わったらゲームオーバーになるようにする
    }
}

void PlayerRobot::TransitionDodge_GreatState()
{
    statenumber = 19;
    state = State::Dodge_Great;

    //無敵時間設定
    invincibleTimer = 1.0f;

    //とりあえずフリップアニメーションを再生
    model->PlayAnimation(Anim_Jump_Flip, false);
}

void PlayerRobot::UpdateDodge_GreatState(float elapsedTime)
{
    //フリップアニメーション終了後、待機ステートへ遷移
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_GreatState();
    }
}

void PlayerRobot::TransitionGuard_GreatState()
{
    statenumber = 20;
    state = State::Guard_Great;

    //ガードモーション再生
    model->PlayAnimation(Anim_GetHit1, true);
}

void PlayerRobot::UpdateGuard_GreatState(float elapsedTime)
{
    if (!InputGuard(elapsedTime))
    {
        //待機ステートへ遷移
        TransitionIdle_GreatState();
    }

    //歩き入力処理
    if (InputWalk(elapsedTime))
    {
        //歩きステートへ遷移
        TransitionWalk_GreatState();
    }

    //走り入力処理
    if (InputRun(elapsedTime))
    {
        //走りステートへ遷移
        TransitionRun_GreatState();
    }
    //ジャンプ入力処理
    if (InputJump())
    {
        //ジャンプステートへ遷移
        TransitionJump_GreatState();
    }

    //攻撃入力処理
    if (InputAttack())
    {
        //攻撃ステートへ遷移
        TransitionAttack_GreatState();
    }

    //回避入力処理
    if (InputDodge(elapsedTime))
    {
        //回避ステートへ遷移
        TransitionDodge_GreatState();
    }
}

void PlayerRobot::TransitionChange_GreatState()
{
    state = State::Change_Great;
    weapon = 2;
    changetime = 0.0f;

    //武器変更モーション再生
    model->PlayAnimation(Anim_Jump_Flip, false);
}

void PlayerRobot::UpdateChange_GreatState(float elapsedTime)
{
    //武器変更モーションの再生が終われば次の武器へ変更
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_DualState();
    }
}

//双剣
void PlayerRobot::TransitionIdle_DualState()
{
    statenumber = 21;

    //双剣の待機に遷移
    state = State::Idle_Dual;

    //待機アニメーション再生
    model->PlayAnimation(Anim_Idle, true);
}

void PlayerRobot::UpdateIdle_DualState(float elapsedTime)
{
    //武器変更入力処理
    if (InputWeapon())
    {
        //武器変更ステートへ遷移
        TransitionChange_DualState();
    }

    //歩き入力処理
    if (InputWalk(elapsedTime))
    {
        //歩きステートへ遷移
        TransitionWalk_DualState();
    }

    //走り入力処理
    if (InputRun(elapsedTime))
    {
        //走りステートへ遷移
        TransitionRun_DualState();
    }

    //ジャンプ入力処理
    if (InputJump())
    {
        //ジャンプステートへ遷移
        TransitionJump_DualState();
    }

    //攻撃入力処理
    if (InputAttack())
    {
        //攻撃ステートへ遷移
        TransitionAttack_DualState();
    }

    //回避入力処理
    if (InputDodge(elapsedTime))
    {
        //回避ステートへ遷移
        TransitionDodge_DualState();
    }
}

void PlayerRobot::TransitionWalk_DualState()
{
    statenumber = 22;

    //双剣の歩きに遷移
    state = State::Walk_Dual;

    //歩きモーションの再生
    model->PlayAnimation(Anim_Walking, true);
}

void PlayerRobot::UpdateWalk_DualState(float elapsedTime)
{
    //武器変更入力処理
    if (InputWeapon())
    {
        //武器変更ステートへ遷移
        TransitionChange_DualState();
    }

    //移動入力処理
    if (!InputWalk(elapsedTime))
    {
        //待機ステートへ遷移
        TransitionIdle_DualState();
    }

    //走り入力処理
    if (InputRun(elapsedTime))
    {
        //走りステートへ遷移
        TransitionRun_DualState();
    }

    //ジャンプ入力処理
    if (InputJump())
    {
        //ジャンプステートへ遷移
        TransitionJump_DualState();
    }

    //攻撃入力処理
    if (InputAttack())
    {
        //攻撃ステートへ遷移
        TransitionAttack_DualState();
    }

    //回避入力処理
    if (InputDodge(elapsedTime))
    {
        //回避ステートへ遷移
        TransitionDodge_DualState();
    }
}

void PlayerRobot::TransitionRun_DualState()
{
    statenumber = 23;
    state = State::Run_Dual;

    //走りモーションを再生
    model->PlayAnimation(Anim_Running, true);
}

void PlayerRobot::UpdateRun_DualState(float elapsedTime)
{
    //武器変更入力処理
    if (InputWeapon())
    {
        //武器変更ステートへ遷移
        TransitionChange_DualState();
    }

    //走り入力処理
    if (!InputRun(elapsedTime))
    {
        //待機ステートへ遷移
        TransitionIdle_DualState();
    }
    //ジャンプ入力処理
    if (InputJump())
    {
        //ジャンプステートへ遷移
        TransitionJump_DualState();
    }

    //攻撃入力処理
    if (InputAttack())
    {
        //攻撃ステートへ遷移
        TransitionAttack_DualState();
    }

    //回避入力処理
    if (InputDodge(elapsedTime))
    {
        //回避ステートへ遷移
        TransitionDodge_DualState();
    }
}

void PlayerRobot::TransitionJump_DualState()
{
    statenumber = 24;
    state = State::Jump_Dual;

    //ジャンプアニメーション再生
    model->PlayAnimation(Anim_Jump, false);
}

void PlayerRobot::UpdateJump_DualState(float elapsedTime)
{
    //武器変更入力処理
    if (InputWeapon())
    {
        //武器変更ステートへ遷移
        TransitionChange_DualState();
    }

    //歩き入力処理
    if (InputWalk(elapsedTime))
    {
        //歩きステートへ遷移
        TransitionWalk_DualState();
    }

    //走り入力処理
    if (InputRun(elapsedTime))
    {
        //走りステートへ遷移
        TransitionRun_DualState();
    }

    //攻撃入力処理
    if (InputAttack())
    {
        //攻撃ステートへ遷移
        TransitionAttack_DualState();
    }

    //ジャンプアニメーション終了後
    if (!model->IsPlayAnimation())
    {
        //落下アニメーション再生
        model->PlayAnimation(Anim_Falling, true);
    }
}

void PlayerRobot::TransitionLand_DualState()
{
    statenumber = 25;
    state = State::Land_Great;

    //着地アニメーション再生
    model->PlayAnimation(Anim_Landing, false);
}

void PlayerRobot::UpdateLand_DualState(float elapsedTIme)
{
    //着地アニメーション終了後
    if (!model->IsPlayAnimation())
    {
        //待機ステートへ遷移
        TransitionIdle_DualState();
    }
}

void PlayerRobot::TransitionAttack_DualState()
{
    statenumber = 26;
    state = State::Attack_Dual;

    //攻撃アニメーション再生
    model->PlayAnimation(Anim_Attack, false);
}

void PlayerRobot::UpdateAttack_DualState(float elapsedTime)
{
    //攻撃アニメーション終了後
    if (!model->IsPlayAnimation())
    {
        //待機ステートへ遷移
        TransitionIdle_DualState();
    }

    //任意のアニメーション再生区間でのみ衝突判定処理をする
    float animationTime = model->GetCurrentAnimationSeconds();
    attackCollisionFlag = animationTime >= 0.3f && animationTime <= 0.4f;
    if (attackCollisionFlag)
    {
        //左手ノードとエネミーの衝突処理
        CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);
    }
}

void PlayerRobot::TransitionDamage_DualState()
{
    statenumber = 27;
    state = State::Damage_Dual;

    //ダメージアニメーション再生
    model->PlayAnimation(Anim_GetHit1, false);
}

void PlayerRobot::UpdateDamage_DualState(float elapsedTime)
{
    //ダメージアニメーションが終わったら待機ステートへ遷移
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_DualState();
    }
}

void PlayerRobot::TransitionDeath_DualState()
{
    statenumber = 28;
    state = State::Death_Dual;

    //ダメージアニメーション再生
    model->PlayAnimation(Anim_Death, false);
}

void PlayerRobot::UpdateDeath_DualState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        //アニメーション再生が終わったらゲームオーバーになるようにする
    }
}

void PlayerRobot::TransitionDodge_DualState()
{
    statenumber = 29;
    state = State::Dodge_Dual;

    //無敵時間設定
    invincibleTimer = 1.0f;

    //とりあえずフリップアニメーションを再生
    model->PlayAnimation(Anim_Jump_Flip, false);
}

void PlayerRobot::UpdateDodge_DualState(float elapsedTime)
{
    //フリップアニメーション終了後、待機ステートへ遷移
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_DualState();
    }
}

void PlayerRobot::TransitionChange_DualState()
{
    state = State::Change_Dual;

    weapon = 3;
    changetime = 0.0f;

    //武器変更モーション再生
    model->PlayAnimation(Anim_Jump_Flip, false);
}

void PlayerRobot::UpdateChange_DualState(float elapsedTime)
{
    //武器変更モーションの再生が終われば次の武器へ変更
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_GunState();
    }
}

//銃
void PlayerRobot::TransitionIdle_GunState()
{
    statenumber = 31;

    //銃の待機に遷移
    state = State::Idle_Gun;

    //待機アニメーション再生
    model->PlayAnimation(Anim_Idle, true);
}

void PlayerRobot::UpdateIdle_GunState(float elapsedTime)
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

    //ジャンプ入力処理
    if (InputJump())
    {
        //ジャンプステートへ遷移
        TransitionJump_GunState();
    }

    //攻撃入力処理
    if (InputAttack())
    {
        //攻撃ステートへ遷移
        TransitionAttack_GunState();
    }

    //回避入力処理
    if (InputDodge(elapsedTime))
    {
        //回避ステートへ遷移
        TransitionDodge_GunState();
    }
}

void PlayerRobot::TransitionWalk_GunState()
{
    statenumber = 32;

    //銃の歩きに遷移
    state = State::Walk_Gun;

    //歩きモーションの再生
    model->PlayAnimation(Anim_Walking, true);
}

void PlayerRobot::UpdateWalk_GunState(float elapsedTime)
{
    //武器変更入力処理
    if (InputWeapon())
    {
        //武器変更ステートへ遷移
        TransitionChange_GunState();
    }

    //移動入力処理
    if (!InputWalk(elapsedTime))
    {
        //待機ステートへ遷移
        TransitionIdle_GunState();
    }

    //走り入力処理
    if (InputRun(elapsedTime))
    {
        //走りステートへ遷移
        TransitionRun_GunState();
    }

    //ジャンプ入力処理
    if (InputJump())
    {
        //ジャンプステートへ遷移
        TransitionJump_GunState();
    }

    //攻撃入力処理
    if (InputAttack())
    {
        //攻撃ステートへ遷移
        TransitionAttack_GunState();
    }

    //回避入力処理
    if (InputDodge(elapsedTime))
    {
        //回避ステートへ遷移
        TransitionDodge_GunState();
    }
}

void PlayerRobot::TransitionRun_GunState()
{
    statenumber = 33;
    state = State::Run_Gun;

    //走りモーションを再生
    model->PlayAnimation(Anim_Running, true);
}

void PlayerRobot::UpdateRun_GunState(float elapsedTime)
{
    //武器変更入力処理
    if (InputWeapon())
    {
        //武器変更ステートへ遷移
        TransitionChange_GunState();
    }

    //走り入力処理
    if (!InputRun(elapsedTime))
    {
        //待機ステートへ遷移
        TransitionIdle_GunState();
    }
    //ジャンプ入力処理
    if (InputJump())
    {
        //ジャンプステートへ遷移
        TransitionJump_GunState();
    }

    //攻撃入力処理
    if (InputAttack())
    {
        //攻撃ステートへ遷移
        TransitionAttack_GunState();
    }

    //回避入力処理
    if (InputDodge(elapsedTime))
    {
        //回避ステートへ遷移
        TransitionDodge_GunState();
    }
}

void PlayerRobot::TransitionJump_GunState()
{
    statenumber = 34;
    state = State::Jump_Gun;

    //ジャンプアニメーション再生
    model->PlayAnimation(Anim_Jump, false);
}

void PlayerRobot::UpdateJump_GunState(float elapsedTime)
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
    if (InputAttack())
    {
        //攻撃ステートへ遷移
        TransitionAttack_GunState();
    }

    //ジャンプアニメーション終了後
    if (!model->IsPlayAnimation())
    {
        //落下アニメーション再生
        model->PlayAnimation(Anim_Falling, true);
    }
}

void PlayerRobot::TransitionLand_GunState()
{
    statenumber = 35;
    state = State::Land_Gun;

    //着地アニメーション再生
    model->PlayAnimation(Anim_Landing, false);
}

void PlayerRobot::UpdateLand_GunState(float elapsedTIme)
{
    //着地アニメーション終了後
    if (!model->IsPlayAnimation())
    {
        //待機ステートへ遷移
        TransitionIdle_DualState();
    }
}

void PlayerRobot::TransitionAttack_GunState()
{
    statenumber = 36;
    state = State::Attack_Gun;

    //攻撃アニメーション再生
    model->PlayAnimation(Anim_Attack, false);
}

void PlayerRobot::UpdateAttack_GunState(float elapsedTime)
{
    //攻撃アニメーション終了後
    if (!model->IsPlayAnimation())
    {
        //待機ステートへ遷移
        TransitionIdle_DualState();
    }

    //任意のアニメーション再生区間でのみ衝突判定処理をする
    float animationTime = model->GetCurrentAnimationSeconds();
    attackCollisionFlag = animationTime >= 0.3f && animationTime <= 0.4f;
    if (attackCollisionFlag)
    {
        //左手ノードとエネミーの衝突処理
        CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);
    }
}

void PlayerRobot::TransitionDamage_GunState()
{
    statenumber = 37;
    state = State::Damage_Gun;

    //ダメージアニメーション再生
    model->PlayAnimation(Anim_GetHit1, false);
}

void PlayerRobot::UpdateDamage_GunState(float elapsedTime)
{
    //ダメージアニメーションが終わったら待機ステートへ遷移
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_GunState();
    }
}

void PlayerRobot::TransitionDeath_GunState()
{
    statenumber = 38;
    state = State::Death_Gun;

    //ダメージアニメーション再生
    model->PlayAnimation(Anim_Death, false);
}

void PlayerRobot::UpdateDeath_GunState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        //アニメーション再生が終わったらゲームオーバーになるようにする
    }
}

void PlayerRobot::TransitionDodge_GunState()
{
    statenumber = 39;
    state = State::Dodge_Gun;

    //無敵時間設定
    invincibleTimer = 1.0f;

    //とりあえずフリップアニメーションを再生
    model->PlayAnimation(Anim_Jump_Flip, false);
}

void PlayerRobot::UpdateDodge_GunState(float elapsedTime)
{
    //フリップアニメーション終了後、待機ステートへ遷移
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_DualState();
    }
}

void PlayerRobot::TransitionReload_GunState()
{
    statenumber = 40;
    state = State::Reload_Gun;

    //リロードアニメーション再生
    model->PlayAnimation(Anim_Revive, false);
}

void PlayerRobot::UpdateReload_GunState(float elapsedTime)
{
    //リロードにかかる時間
    //reloadtime--;

    if (!model->IsPlayAnimation())
    {
        //弾薬数を元に戻す
        bullets = 60;

        //アニメーションが終わったら待機状態に戻る
        TransitionIdle_GunState();
    }

}

void PlayerRobot::TransitionChange_GunState()
{
    state = State::Change_Gun;

    weapon = 0;
    changetime = 0.0f;

    //武器変更モーション再生
    model->PlayAnimation(Anim_Jump_Flip, false);
}

void PlayerRobot::UpdateChange_GunState(float elapsedTime)
{
    //武器変更モーションの再生が終われば次の武器へ変更
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}
