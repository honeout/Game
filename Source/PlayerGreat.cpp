#include <imgui.h>
#include "PlayerGreat.h"
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraite.h"
#include "ProjectileHoming.h"
#include "LightManager.h"
#include "PlayerManager.h"
//#include "shiled.h"

//コンストラクタ
PlayerGreat::PlayerGreat()
{
    //モデルのロード
    model = new Model("Data/Model/Player/greatsword_player.mdl");
    
    
    //体力の設定
    health = 15;
    maxHealth = 15;

    //モデルが大きいのでスケーリング
    scale.x = scale.y = scale.z = 0.02f;

    //ヒットエフェクト読み込み
    hitEffect = new Effect("Data/Effect/slash.efk");
    invincible_hitEffect = new Effect("Data/Effect/invincible.efk");
    guardmode = new Effect("Data/Effect/invincible.efk");

    attackcount = 0;
    weapon = 0;
    attackcount = 0;
    bullets = 60;
    totalattackcount = 0;
    changetime = 0.0f;
    guardflag = false;

    //SEの設定
    attack_01 = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Swing7.wav");
    attack_02 = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Swing8.wav");
    attack_03 = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Swing9.wav");
    guard = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Guard.wav");
    damaged = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Player_Damaged.wav");
    brake = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Player_Brake.wav");
    run_fast = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Run_Fast_Dirt.wav");
    run_slow = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Run_Slow_Dirt.wav");



    UpdateTransform();

    //待機ステートへ遷移(大剣)
    TransitionIdle_GreatState();
    PlayerManager::Instance().SetGuardMode(false);

}

//デストラクタ
PlayerGreat::~PlayerGreat()
{
    invincible_hitEffect->stop(invincible_hitEffect->GetHandle());
    delete  invincible_hitEffect;
    delete hitEffect;



    if (guardmode != nullptr)
    {
        delete guardmode;
        guardmode = nullptr;
    }

    delete model;

}

//更新処理
void PlayerGreat::Update(float elapsedTime)
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
        case State::Brake_Great:
            UpdateBrake_GreatState(elapsedTime);
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
    if (attackcount == 2)
    {
        model->ReverseplaybackAnimation(elapsedTime);
    }
    else
    {
        model->UpdateAnimation(elapsedTime);
    }

    attacktime++;
    attackrate++;




    //モデル行列更新
    model->UpdateTransform(transform);
    //↑オブジェクトの位置にモデル表示するため、
    //オブジェクトの行列をモデルに渡してモデルの行列を計算する
}

//描画処理
void PlayerGreat::Render(RenderContext& rc, ModelShader* shader)
{
    rc.toonData.edgeWidth = 0.02f;

   

    shader->Draw(rc, model);
    //↑描画する際に計算されたモデルの行列をこの中で利用

  

    //弾丸描画処理
    projectileManager.Render(rc, shader);
}

//デバッグ用GUI描画
void PlayerGreat::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("PlayerGreat", nullptr, ImGuiWindowFlags_None))
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
void PlayerGreat::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //衝突判定用のデバッグ円柱を描画
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

    //弾丸デバッグプリミティブ描画
    projectileManager.DrawDebugPrimitive();

    //攻撃衝突用の左手ノードのデバッグ球を描画
    if (attackCollisionFlag)
    {
        Model::Node* swordBone = model->FindNode("master_Large_sword_01:J_hook_R_03");
        debugRenderer->DrawSphere(DirectX::XMFLOAT3(
            swordBone->worldTransform._41,
            swordBone->worldTransform._42,
            swordBone->worldTransform._43),
            greatRadius,
            DirectX::XMFLOAT4(1, 0, 0, 1)
        );
    }
}

//弾丸と敵の衝突処理
void PlayerGreat::CollisionProjectilesVsEnemies()
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




DirectX::XMFLOAT3 PlayerGreat::GetMoveVec() const
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
bool PlayerGreat::InputMove(float elapsedTime)
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

bool PlayerGreat::InputWalk(float elapsedTime)
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

bool PlayerGreat::InputRun(float elapsedTime)
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

        moveSpeed = 12.0f;
        maxMoveSpeed = 12.0f;

        //移動処理
        Move(moveVec.x, moveVec.z, moveSpeed);

        //旋回処理
        Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

        //進行ベクトルが0ベクトルでない場合は入力された
        return moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
    }
    return false;
}

bool PlayerGreat::InputDodge(float elpsedTime)
{
    //コントロールキーが押されたら

    return false;
}

//プレイヤーとエネミーとの衝突処理
void PlayerGreat::CollisionPlayerVsEnemies()
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
   
        }
    }
}

//ノードと敵の衝突処理
void PlayerGreat::CollisionNodeVsEnemies(const char* nodeName, float nodeRadius)
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

                    
                }
                //ヒットエフェクト再生
                {

                    hitEffect->Play(nodePosition, 0.2f);
                }
            }
        }
    }

}




//攻撃入力処理
bool PlayerGreat::InputAttack()
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

bool PlayerGreat::InputWeapon()
{
    //TODO Manager管理
    if ((GetAsyncKeyState(VK_SPACE) & 0x8001) && changetimer >= changetimermax)
    {


        return true;
    }
    return false;
}

bool PlayerGreat::InputGuard(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();

    if ((gamePad.GetButtonDown() & GamePad::BTN_LEFT_TRIGGER) || (mouse.GetButton() & Mouse::BTN_RIGHT))
    {
        guardflag = true;
        return true;
    }

    return false;
}


//ダメージを受けたときに呼ばれる
void PlayerGreat::OnDamaged()
{
    TransitionDamage_GreatState();
}

//死亡したときに呼ばれる
void PlayerGreat::OnDead()
{
    TransitionDeath_GreatState();
}
//大剣
void PlayerGreat::TransitionIdle_GreatState()
{
    statenumber = 11;

    //大剣の待機に遷移
    state = State::Idle_Great;

    //待機アニメーション再生
    model->PlayAnimation(Anim_Great_Idle, true);
}

void PlayerGreat::UpdateIdle_GreatState(float elapsedTime)
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
    if (InputAttack() && attackrate > 20.0f)
    {
        attackcount = 1;
        //攻撃ステートへ遷移
        TransitionAttack_GreatState();
    }


    //ガード入力処理
    if (InputGuard(elapsedTime))
    {
        //ガードステートへ遷移
        TransitionGuard_GreatState();
    }
}


void PlayerGreat::TransitionWalk_GreatState()
{
    statenumber = 12;

    //大剣の歩きに遷移
    state = State::Walk_Great;

    //歩きモーションの再生
    run_slow->Play(true, 1.0f);
    model->PlayAnimation(Anim_Great_Dash, true);
}

void PlayerGreat::UpdateWalk_GreatState(float elapsedTime)
{
    //武器変更入力処理
    if (InputWeapon())
    {
        run_slow->Stop();
        //武器変更ステートへ遷移
        TransitionChange_GreatState();
    }

    //移動入力処理
    if (!InputWalk(elapsedTime))
    {
        run_slow->Stop();
        //待機ステートへ遷移
        TransitionIdle_GreatState();
    }

    //走り入力処理
    if (InputRun(elapsedTime))
    {
        run_slow->Stop();
        //走りステートへ遷移
        TransitionRun_GreatState();
    }


    //攻撃入力処理
    if (InputAttack() && attackrate > 20.0f)
    {
        run_slow->Stop();
        attackcount = 1;
        //攻撃ステートへ遷移
        TransitionAttack_GreatState();
    }


    //ガード入力処理
    if (InputGuard(elapsedTime))
    {
        run_slow->Stop();
        //ガードステートへ遷移
        TransitionGuard_GreatState();
    }
}

void PlayerGreat::TransitionRun_GreatState()
{
    statenumber = 13;
    state = State::Run_Great;

    //走りモーションを再生
    run_slow->Play(true, 1.0f);
    model->PlayAnimation(Anim_Great_Dash, true);
}

void PlayerGreat::UpdateRun_GreatState(float elapsedTime)
{
    //武器変更入力処理
    if (InputWeapon())
    {
        run_slow->Stop();
        //武器変更ステートへ遷移
        TransitionChange_GreatState();
    }

    //走り入力処理
    if (!InputRun(elapsedTime))
    {
        run_slow->Stop();
        //ブレーキステートへ遷移
        TransitionBrake_GreatState();
    }


    //攻撃入力処理
    if (InputAttack() && attackrate > 20.0f)
    {
        run_slow->Stop();
        attackcount = 1;
        //攻撃ステートへ遷移
        TransitionAttack_GreatState();
    }


    //ガード入力処理
    if (InputGuard(elapsedTime))
    {
        run_slow->Stop();
        //ガードステートへ遷移
        TransitionGuard_GreatState();
    }
}


void PlayerGreat::TransitionAttack_GreatState()
{
    statenumber = 16;
    state = State::Attack_Great;

    //攻撃アニメーション再生
    if (attackcount == 1)
    {
        attack_01->Play(false, 1.0f);
        model->PlayAnimation(Anim_Great_Attack_1, false);
        attacktime = 0.0f;
    }

    if (attackcount == 2)
    {
        attack_02->Play(false, 1.0f);
        model->PlayAnimation(Anim_Great_Attack_1, false);
        attacktime = 0.0f;
    }

    if (attackcount == 3)
    {
        attack_03->Play(false, 1.0f);
        model->PlayAnimation(Anim_Great_Attack_3, false);
        attacktime = 0.0f;
    }
}

void PlayerGreat::UpdateAttack_GreatState(float elapsedTime)
{
    if (InputAttack() && attacktime > 30.0f && attackcount == 1)
    {
        attackcount = 2;
        attacktime = 0.0f;
        //攻撃ステートへ繊維
        TransitionAttack_GreatState();
    }

    if (attackcount == 2 && InputAttack() && !model->IsPlayAnimation() && attacktime < 60.0f)
    {
        attackcount = 3;
        attacktime = 0.0f;
        TransitionAttack_GreatState();
    }

    //任意のアニメーション再生区間でのみ衝突判定処理をする
    float animationTime = model->GetCurrentAnimationSeconds();
    attackCollisionFlag = animationTime >= 0.0f && animationTime <= 1.0f;

    CollisionNodeVsEnemies("master_Large_sword_01:J_hook_R_03", greatRadius);

    //攻撃アニメーション終了後
    if ((!model->IsPlayAnimation() && !InputAttack()) || attacktime >= 60.0f)
    {
        attackCollisionFlag = false;
        attackrate = 0.0f;
        //待機ステートへ遷移
        TransitionIdle_GreatState();
        attackcount = 0;
    }
}

void PlayerGreat::TransitionDamage_GreatState()
{
    statenumber = 17;
    state = State::Damage_Great;
    run_slow->Stop();

    //ダメージアニメーション再生
    if (guardflag)
    {
        model->PlayAnimation(Anim_Great_Guard_Damaged, false);
    }
    else
    {
        model->PlayAnimation(Anim_Great_Damaged, false);
    }
}

void PlayerGreat::UpdateDamage_GreatState(float elapsedTime)
{

    //ダメージアニメーションが終わったら待機ステートへ遷移
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_GreatState();
    }
}

void PlayerGreat::TransitionDeath_GreatState()
{
    statenumber = 18;
    state = State::Death_Great;
    run_slow->Stop();

    //ダメージアニメーション再生
    model->PlayAnimation(Anim_Great_Death, false);
}

void PlayerGreat::UpdateDeath_GreatState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        //アニメーション再生が終わったらゲームオーバーになるようにする
    }
}


void PlayerGreat::TransitionGuard_GreatState()
{
    statenumber = 20;
    state = State::Guard_Great;
    guardflag = true;
    guardtime = 0;

   

    //ガードモーション再生
    //guard->Play(false, 1.0f);
    model->PlayAnimation(Anim_Great_Guard, false);
   
    //guardmode->Play(position, 0.5f);
    PlayerManager::Instance().SetGuardMode(true);
}

void PlayerGreat::UpdateGuard_GreatState(float elapsedTime)
{
    //前方向
    DirectX::XMFLOAT3 dir;
    dir.x = sinf(angle.y);
    dir.y = 0.0f;
    dir.z = cos(angle.y);

    guardtime++;
    if (guardtime >= 60)
    {
    
      
        guardtime = 0;
    }
    if (!InputGuard(elapsedTime))
    {
     
        guardflag = false;
        PlayerManager::Instance().SetGuardMode(false);
        //待機ステートへ遷移
        TransitionIdle_GreatState();
    }

}

void PlayerGreat::TransitionChange_GreatState()
{
    state = State::Change_Great;
    weapon = 2;
    changetime = 0.0f;

    //武器変更モーション再生
    model->PlayAnimation(Anim_Great_Change, false);


}

void PlayerGreat::UpdateChange_GreatState(float elapsedTime)
{
    PlayerManager::Instance().SetStartAnimation(true);
    DirectX::XMFLOAT3 position_great = position;
    DirectX::XMFLOAT3 angle_great = angle;
    PlayerManager::Instance().SetWeaponFlag(true);
    PlayerManager::Instance().SetBeforeWeapon(1);
    PlayerManager::Instance().SetWeapon(3);
    Player* player = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetWeapon());
    player->SetPosition(position_great);
    player->SetAngle(angle_great);
    player->SetChangeTimer(0.0f);
    player->SetHP(health);
    TransitionIdle_GreatState();
    position = { 0,1000000000000.0f,0 };

}

void PlayerGreat::TransitionBrake_GreatState()
{
    state = State::Brake_Great;

    run_slow->Stop();
    brake->Play(false, 1.0f);
    model->PlayAnimation(Anim_Great_Brake, false);
}

void PlayerGreat::UpdateBrake_GreatState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_GreatState();
    }
}
