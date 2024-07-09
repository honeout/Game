#include <imgui.h>
#include "PlayerDual.h"
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraite.h"
#include "ProjectileHoming.h"
#include "LightManager.h"
#include "PlayerManager.h"
#include "Graphics/Model.h"

//コンストラクタ
PlayerDual::PlayerDual()
{
    //モデルのロード
    model = new Model("Data/Model/Player/dualsword_player.mdl");

    //体力の設定
    health = 15;
    maxHealth = 15;

    //モデルが大きいのでスケーリング
    scale.x = scale.y = scale.z = 0.02f;

    //ヒットエフェクト読み込み
    hitEffect = new Effect("Data/Effect/slash.efk");
    invincible_hitEffect = new Effect("Data/Effect/invincible.efk");
    
    attackcount = 0;
    weapon = 0;
    attackcount = 0;
    bullets = 60;
    totalattackcount = 0;
    changetime = 0.0f;

    node = model->FindNode("player_upper");

    position.z = 33;

    //SEの設定
    attack_01 = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Swing7.wav");
    attack_02 = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Swing8.wav");
    attack_03 = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Swing9.wav");
    damaged = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Player_Damaged.wav");
    brake = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Player_Brake.wav");
    run_fast = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Run_Fast_Dirt.wav");
    run_slow = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Run_Slow_Dirt.wav");

    //オブジェクト行列を更新
    UpdateTransform();

    //モデル行列更新
    model->UpdateTransform(transform);

    //待機ステートへ遷移
    TransitionIdle_DualState();
}

//デストラクタ
PlayerDual::~PlayerDual()
{
    invincible_hitEffect->stop(invincible_hitEffect->GetHandle());
    delete  invincible_hitEffect;

    delete hitEffect;

    delete model;
}

//更新処理
void PlayerDual::Update(float elapsedTime)
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
    if (position.y >= 1)
    {
        position.y = -1;
    }
    if (position.y < -10.0f)
    {
        position.y = -1;
    }


    if (PlayerManager::Instance().GetWeapon() == 0)
    {
        //ステート毎の処理
        switch (state)
        {
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
        case State::Attack_Dual:
            UpdateAttack_DualState(elapsedTime);
            ////ノード取得
            //Model::Node* leftnode = model->FindNode("master_Large_sword_02:J_blade_L_01");
            //Model::Node* rightnode = model->FindNode("master_Large_sword_02:J_blade_R_01");
            //{
            //    // 最後の頂点を除いてすべてを1つずつ後ろにずらす
            //    for (int i = MAX_POLYGON - 1; i > 0; --i)
            //    {
            //        trailPositions[0][i] = trailPositions[0][i - 1];
            //        trailPositions[1][i] = trailPositions[1][i - 1];
            //    }
            //}
            //{
            //    // 剣の原点から根本と先端までのオフセット値
            //    DirectX::XMVECTOR RootOffset = DirectX::XMVectorSet(0, 0, 0.3f, 0);
            //    DirectX::XMVECTOR TipOffset = DirectX::XMVectorSet(0, 0, 2.3f, 0);
            //    //武器のノードの0番のワールド行列を用いて
            //    //それぞれのオフセットに乗算して位置を算出する
            //    // 剣の位置を取得し、頂点バッファに保存
            //    DirectX::XMMATRIX WeaponRootTransform;
            //    DirectX::XMVECTOR RootPosition, TipPosition;
            //    WeaponRootTransform = DirectX::XMLoadFloat4x4(&leftnode->worldTransform);
            //    RootPosition = DirectX::XMVector3TransformCoord(RootOffset, WeaponRootTransform);
            //    TipPosition = DirectX::XMVector3TransformCoord(TipOffset, WeaponRootTransform);
            //    DirectX::XMStoreFloat3(&trailPositions[0][0], RootPosition);
            //    DirectX::XMStoreFloat3(&trailPositions[1][0], TipPosition);
            //}
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
        case State::Brake_Dual:
            UpdateBrake_DualState(elapsedTime);
            break;
        }


        //武器変更クールタイム
        changetimer++;

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

        attacktime++;
        attackrate++;

        //モデル行列更新
        model->UpdateTransform(transform);
        //↑オブジェクトの位置にモデル表示するため、
        //オブジェクトの行列をモデルに渡してモデルの行列を計算する
    }
}

//描画処理
void PlayerDual::Render(RenderContext& rc, ModelShader* shader)
{
    rc.toonData.edgeWidth = 0.02f;


    shader->Draw(rc, model);
    //↑描画する際に計算されたモデルの行列をこの中で利用

    //弾丸描画処理
    projectileManager.Render(rc, shader);
}

//デバッグ用GUI描画
void PlayerDual::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("PlayerDual", nullptr, ImGuiWindowFlags_None))
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

            ImGui::InputInt("bonebone", &node->now, 0, 10);
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
void PlayerDual::DrawDebugPrimitive()
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
            dualRadius,
            DirectX::XMFLOAT4(1, 0, 0, 1)
        );
        debugRenderer->DrawSphere(DirectX::XMFLOAT3(
            swordBone_L->worldTransform._41,
            swordBone_L->worldTransform._42,
            swordBone_L->worldTransform._43),
            dualRadius,
            DirectX::XMFLOAT4(1, 0, 0, 1)
        );
    }
}

//弾丸と敵の衝突処理
void PlayerDual::CollisionProjectilesVsEnemies()
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




DirectX::XMFLOAT3 PlayerDual::GetMoveVec() const
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
bool PlayerDual::InputMove(float elapsedTime)
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

bool PlayerDual::InputWalk(float elapsedTime)
{
    maxMoveSpeed = 10.0f;
    moveSpeed = 10.0f;

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

bool PlayerDual::InputRun(float elapsedTime)
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

        moveSpeed = 25.0f;
        maxMoveSpeed = 25.0f;

        //移動処理
        Move(moveVec.x, moveVec.z, moveSpeed);

        //旋回処理
        Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

        //進行ベクトルが0ベクトルでない場合は入力された
        return moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
    }
    return false;
}

bool PlayerDual::InputDodge(float elpsedTime)
{
    //コントロールキーが押されたら

    return false;
}

//プレイヤーとエネミーとの衝突処理
void PlayerDual::CollisionPlayerVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();
    DirectX::XMFLOAT3 nowposition = position;

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

                //if (GetMoveVec().x >= 0.0f)
                //{
                //    AddImpulse({ -5.0f, 0.0f, 0.0f });
                //}
                //else if (GetMoveVec().x < 0.0f)
                //{
                //    AddImpulse({ 5.0f, 0.0f, 0.0f });
                //}
                //if (GetMoveVec().z >= 0.0f)
                //{
                //    AddImpulse({ 0.0f, 0.0f, -5.0f });
                //}
                //else if (GetMoveVec().z < 0.0f)
                //{
                //    AddImpulse({ 0.0f, 0.0f, 5.0f });
                //}

                //TransitionBrake_DualState();
                //enemy->SetPosition(enemy->GetPosition());
            }
        }
    }
}

//ノードと敵の衝突処理
void PlayerDual::CollisionNodeVsEnemies(const char* nodeName, float nodeRadius)
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
            if (enemy->GetAnimationChange() == 0)
            {
                invincible_hitEffect->Play(nodePosition, 0.3f);
                return;
            }
            //ダメージを与える
            if (enemy->ApplyDamage(1, 0.2f))
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

                    //enemy->AddImpulse(impulse);
                }
                //ヒットエフェクト再生
                {
                    //DirectX::XMFLOAT3 e = enemy->GetPosition();
                    //e.y += enemy->GetHeight() * 0.5f;


                    hitEffect->Play(nodePosition,0.2f);

                }
            }
        }
    }
}

//ジャンプ入力処理
bool PlayerDual::InputJump()
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
void PlayerDual::InputProjectile()
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
bool PlayerDual::InputAttack()
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

//TODOここはManagerで管理しないとうまくいかない
bool PlayerDual::InputWeapon()
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

bool PlayerDual::InputGuard(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();

    if ((gamePad.GetButtonDown() & GamePad::BTN_LEFT_TRIGGER) || (mouse.GetButton() & Mouse::BTN_RIGHT))
    {
        return true;
    }

    return false;
}

bool PlayerDual::InputReload()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    if ((gamePad.GetButtonDown() & GamePad::BTN_Y) || (GetAsyncKeyState('R') & 0x8000))
    {
        return true;
    }
    return false;
}

//ダメージを受けたときに呼ばれる
void PlayerDual::OnDamaged()
{
    TransitionDamage_DualState();
}

//死亡したときに呼ばれる
void PlayerDual::OnDead()
{
    TransitionDeath_DualState();
}

//双剣
void PlayerDual::TransitionIdle_DualState()
{
    statenumber = 21;

    //双剣の待機に遷移
    state = State::Idle_Dual;

    //待機アニメーション再生
    model->PlayAnimation(Anim_Dual_Idle, true);
}

void PlayerDual::UpdateIdle_DualState(float elapsedTime)
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
    if (InputAttack() && attackrate > 30.0f)
    {
        attackcount = 1;
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

void PlayerDual::TransitionWalk_DualState()
{
    statenumber = 22;

    //双剣の歩きに遷移
    state = State::Walk_Dual;

    //歩きモーションの再生
    run_slow->Play(true, 1.0f);
    model->PlayAnimation(Anim_Dual_Dash, true);
}

void PlayerDual::UpdateWalk_DualState(float elapsedTime)
{
    //武器変更入力処理
    if (InputWeapon())
    {
        run_slow->Stop();
        //武器変更ステートへ遷移
        TransitionChange_DualState();
    }

    //移動入力処理
    if (!InputWalk(elapsedTime))
    {
        run_slow->Stop();
        //待機ステートへ遷移
        TransitionIdle_DualState();
    }

    //走り入力処理
    if (InputRun(elapsedTime))
    {
        run_slow->Stop();
        //走りステートへ遷移
        TransitionRun_DualState();
    }



    //攻撃入力処理
    if (InputAttack() && attackrate > 30.0f)
    {
        run_slow->Stop();
        attackcount = 1;
        //攻撃ステートへ遷移
        TransitionAttack_DualState();
    }

    //回避入力処理
    if (InputDodge(elapsedTime))
    {
        run_slow->Stop();
        //回避ステートへ遷移
        TransitionDodge_DualState();
    }
}

void PlayerDual::TransitionRun_DualState()
{
    statenumber = 23;
    state = State::Run_Dual;

    //走りモーションを再生
    run_fast->Play(true, 1.0f);
    model->PlayAnimation(Anim_Dual_Dash, true);
}

void PlayerDual::UpdateRun_DualState(float elapsedTime)
{
    //武器変更入力処理
    if (InputWeapon())
    {
        run_fast->Stop();
        //武器変更ステートへ遷移
        TransitionChange_DualState();
    }

    //走り入力処理
    if (!InputRun(elapsedTime))
    {
        run_fast->Stop();
        //ブレーキステートへ
        TransitionBrake_DualState();

        //待機ステートへ遷移
        //TransitionIdle_DualState();
    }

    //攻撃入力処理
    if (InputAttack() && attackrate > 30.0f)
    {
        run_fast->Stop();
        attackcount = 1;
        //攻撃ステートへ遷移
        TransitionAttack_DualState();
    }

    //回避入力処理
    if (InputDodge(elapsedTime))
    {
        run_fast->Stop();
        //回避ステートへ遷移
        TransitionDodge_DualState();
    }
}


void PlayerDual::TransitionAttack_DualState()
{
    statenumber = 26;
    state = State::Attack_Dual;

    //攻撃アニメーション再生
    if (attackcount == 1)
    {
        attack_01->Play(false, 1.0f);
        model->PlayAnimation(Anim_Dual_Attack_1, false);
        attacktime = 0.0f;
    }

    if (attackcount == 2)
    {
        attack_02->Play(false, 1.0f);
        model->PlayAnimation(Anim_Dual_Attack_2, false);
        attacktime = 0.0f;
    }

    if (attackcount == 3)
    {
        attack_03->Play(true, 1.0f);
        model->PlayAnimation(Anim_Dual_Attack_3, false);
        attacktime = 0.0f;
    }
}

void PlayerDual::UpdateAttack_DualState(float elapsedTime)
{
    if (InputAttack() && attacktime > 52.0f && attackcount == 1)
    {
        attackcount = 2;
        attacktime = 0.0f;
        //攻撃ステートへ繊維
        TransitionAttack_DualState();
    }

    if (attackcount == 2 && InputAttack() && !model->IsPlayAnimation() && attacktime < 60.0f)
    {
        attackcount = 3;
        attacktime = 0.0f;
        TransitionAttack_DualState();
    }

    //任意のアニメーション再生区間でのみ衝突判定処理をする
    float animationTime = model->GetCurrentAnimationSeconds();
    //attackCollisionFlag = animationTime >= 0.0f && animationTime <= 1.0f;
    attackCollisionFlag = true;

    CollisionNodeVsEnemies("master_Large_sword_02:J_hook_R_03", dualRadius);
    CollisionNodeVsEnemies("master_Large_sword_02:J_hook_L_03", dualRadius);

    //攻撃アニメーション終了後
    if ((!model->IsPlayAnimation() && !InputAttack()) || attacktime >= 120.0f)
    {
        attack_03->Stop();
        attackCollisionFlag = false;
        attackrate = 0.0f;
        //待機ステートへ遷移
        TransitionIdle_DualState();
        attackcount = 0;
    }
}

void PlayerDual::TransitionDamage_DualState()
{
    statenumber = 27;
    state = State::Damage_Dual;

    run_slow->Stop();
    run_fast->Stop();

    //ダメージアニメーション再生
    model->PlayAnimation(Anim_Dual_Damaged, false);
}

void PlayerDual::UpdateDamage_DualState(float elapsedTime)
{
    //ダメージアニメーションが終わったら待機ステートへ遷移
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_DualState();
    }
}

void PlayerDual::TransitionDeath_DualState()
{
    statenumber = 28;
    state = State::Death_Dual;

    run_slow->Stop();
    run_fast->Stop();

    //ダメージアニメーション再生
    model->PlayAnimation(Anim_Dual_Death, false);
}

void PlayerDual::UpdateDeath_DualState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        //TODOアニメーション再生が終わったらゲームオーバーになるようにする
    }
}

void PlayerDual::TransitionDodge_DualState()
{
    statenumber = 29;
    state = State::Dodge_Dual;

    //無敵時間設定
    invincibleTimer = 1.0f;

    //とりあえずフリップアニメーションを再生
    model->PlayAnimation(Anim_Dual_Dodge, false);
}

void PlayerDual::UpdateDodge_DualState(float elapsedTime)
{
    //フリップアニメーション終了後、待機ステートへ遷移
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_DualState();
    }
}

void PlayerDual::TransitionChange_DualState()
{
    state = State::Change_Dual;

    weapon = 3;
    changetime = 0.0f;

    //武器変更モーション再生
    model->PlayAnimation(Anim_Dual_Death, false);
}

void PlayerDual::UpdateChange_DualState(float elapsedTime)
{
    //光の追従
    //LightManager::Instance().GetLight(0)->SetPosition({ position.x, position.y + height, position.z });
    //LightManager::Instance().GetLight(1)->SetColor({ 0.0f, 0.0f,1.0f,1.0f });
    //LightManager::Instance().GetLight(0)->SetRange(1000000000.0f);
    //if (!model->IsPlayAnimation())
    //{
    PlayerManager::Instance().SetStartAnimation(true);
    DirectX::XMFLOAT3 position_dual = position;
    DirectX::XMFLOAT3 angle_dual = angle;
    PlayerManager::Instance().SetWeaponFlag(true);
    //PlayerManager::Instance().SetWeapon(1);
    PlayerManager::Instance().SetBeforeWeapon(0);
    PlayerManager::Instance().SetWeapon(3);
    Player* player = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetWeapon());
    player->SetPosition(position_dual);
    player->SetAngle(angle_dual);
    player->SetHP(health);
    player->SetChangeTimer(0.0f);
    TransitionIdle_DualState();
    position = { 0,1000000000000.0f,0 };
    //}

    //武器変更モーションの再生が終われば次の武器へ変更
    //if (!model->IsPlayAnimation())
    //{
    //    //TODOManagerでmodel自体を変更する
    //}
}

void PlayerDual::TransitionBrake_DualState()
{
    state = State::Brake_Dual;
    run_slow->Stop();
    run_slow->Stop();
    brake->Play(false, 1.0f);
    model->PlayAnimation(Anim_Dual_Brake, false);
}

void PlayerDual::UpdateBrake_DualState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_DualState();
    }
}
