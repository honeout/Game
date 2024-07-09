#include "EnemyBOSS.h"
#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "Player.h"
#include "PlayerManager.h"
#include "Collision.h"
#include "ProjectileStraite.h"
#include "ProjectileHoming.h"
#include "Input/Input.h"
#include "Camera.h"
#include "CameraController.h"
#include "EnemyManager.h"
#include "PlayerRobot.h"
#include "EnemeyNormalGun.h"
#include "EnemySkillGunCharge.h"
#include "EnemySkillGun.h"
#include "Projectile.h"
#include "UI.h"

static EnemyBOSS* instance = nullptr;

EnemyBOSS& EnemyBOSS::Instance()
{
    return *instance;
}

//コンストラクタ
EnemyBOSS::EnemyBOSS()
{
    //model = new Model("Data/Model/BOSS/Boss.mdl");
    model = new Model("Data/Model/BOSS/BossAnim8.mdl");

    //モデルが大きいのでスケーリング
    scale.x = scale.y = scale.z = 0.09f;
   
    //幅、高さ設定
    radius = 2.9f;
    height = 10.0f;

    health = 100;
    //health = 1;

    //damageEffect = new Effect("Data/Effect/lightning ballgun boss.efk");
    //hitEffect = new Effect("Data/Effect/lightning stricked.efk");
    chargeEffect = new Effect("Data/Effect/bossCharge.efk");
    smork = new Effect("Data/Effect/stageSmork.efk");
    
    changeopen = std::make_unique<Effect>("Data/Effect/eyeopen.efk");
    changeclose = std::make_unique<Effect>("Data/Effect/eyeclose.efk");

    // damage
    damageStamp = 3;
    damageGun = 1;
    damageCharge = 5;

    camerachange = true;

    fadeoutcamera = false;

    EnemyManager::Instance().SetDie(false);

    animationChange = AnimationChange::Normal;

    startboss = false;

    stoptime = 0;

    stumpaudio = Audio::Instance().LoadAudioSource("Data/Audio/Se/Enemy/Enemy_Stomp.wav");
    //chageaudio = Audio::Instance().LoadAudioSource("Data/Audio/Se/Enemy/Enemy_Stomp.wav");
    shotaudio = Audio::Instance().LoadAudioSource("Data/Audio/Se/Enemy/Enemy_Roar.wav");
    damageaudio = Audio::Instance().LoadAudioSource("Data/Audio/Se/Enemy/Enemy_Footsteps.wav");
    hotaudio = Audio::Instance().LoadAudioSource("Data/Audio/Se/Enemy/Juwel_Generation02.wav");
    iceaudio = Audio::Instance().LoadAudioSource("Data/Audio/Se/Enemy/Juwel_Generation01.wav");

    // ボスの状態変換
    changemode = 0;

    //徘徊ステートへ遷移
    //TransitionWanderState();
    TransitionAttackDemoState();
}

//デストラクタ
EnemyBOSS::~EnemyBOSS()
{
    //delete damageEffect;
    //delete hitEffect;

    smork->stop(smork->GetHandle());
    chargeEffect->stop(chargeEffect->GetHandle());

    changeopen->stop(changeopen->GetHandle());
    changeclose->stop(changeclose->GetHandle());

    delete chargeEffect;
    delete model;
    delete smork;
}

//更新処理
void EnemyBOSS::Update(float elapsedTime)
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


    //ステート毎の更新処理
    switch (state)
    {
    case State::Wander:
        UpdateWanderState(elapsedTime);
        break;

    case State::Idle:
        UpdateIdleState(elapsedTime);
        break;

    case State::Eye:
        UpdateEyeState(elapsedTime);
        break;

    case State::Pursuit:
        UpdatePursuitState(elapsedTime);
        break;

    case State::Attack:
        UpdateAttackState(elapsedTime);
        break;

    case State::AttackGun:
        UpdateAttackGun(elapsedTime);
        break;

    case State::IdleBattle:
        UpdateIdleBattleState(elapsedTime);
        break;

    case State::Damage:
        UpdateDamageState(elapsedTime);
        break;

    case State::Death:
        UpdateDeathState(elapsedTime);
        break;

    case State::AttackDemo:
        UpdateAttackDemoState(elapsedTime);
        break;
    }

    //速力更新処理
    UpdateVelocity(elapsedTime);

    projectileManager.Update(elapsedTime);



    switch (attackrand)
    {
        // 通常攻撃用当たり判定
    //case Attack::Attack_stamp:
    //    damage = damageStamp;
    //    CollisionStampVsPlayers();
    //    break;
        // 単発弾攻撃用当たり判定
    case Attack::Attack_Gun:
        damage = damageGun;

        CollisionProjectilesVsPlayers();
        break;
        // 大技攻撃用当たり判定
    case Attack::Attack_Charge:
        damage = damageCharge;
        CollisionChargeVsPlayers();
        break;
    }



    //無敵時間更新
    UpdateInvincibleTimer(elapsedTime);

    //弾丸更新処理

    //オブジェクト行列を更新
    UpdateTransform();


    switch (animationChange)
    {
    case AnimationChange::Normal:

        //モデルアニメーション更新
        model->UpdateAnimation(elapsedTime);


        animationConfirmation = false;
        break;
    case AnimationChange::doble:
        if (!reverse)
        {
            animationConfirmation = true;

            // 上半身用アニメーション
            model->UpdateUpeerBodyAnimation(elapsedTime, "body2");
            // 下半身用アニメーション
            model->UpdateLowerBodyAnimation(elapsedTime, "body2");
        }

        if (reverse)
            model->ReverseplaybackAnimation(elapsedTime);
        break;
    }


    //switch (animationChange)
    //{
    //case AnimationChange::Normal:
    //    if (!reverse)
    //    {
    //        //モデルアニメーション更新
    //        model->UpdateAnimation(elapsedTime);
    //    }
    //    if (reverse)
    //        model->ReverseplaybackAnimation(elapsedTime);
    //    animationConfirmation = false;
    //    break;
    //case AnimationChange::doble:
    //    if (!reverse)
    //    {
    //        animationConfirmation = true;

    //        // 上半身用アニメーション
    //        model->UpdateUpeerBodyAnimation(elapsedTime, "body2");
    //        // 下半身用アニメーション
    //        model->UpdateLowerBodyAnimation(elapsedTime, "body2");
    //    }

   
    //    break;
    //}

    //switch (animationChange)
    //{
    //case AnimationChange::Normal:
    //{
    //    // 逆再生
    //    if (reverse)
    //    {
    //       // model->ReverseplaybackAnimation(elapsedTime);
    //        model->ReverseplaybackAnimation(elapsedTime);
    //    }
    //    // 再生
    //    else
    //    {
    //        //モデルアニメーション更新
    //        model->UpdateAnimation(elapsedTime);
    //    }


    //    animationConfirmation = false;
    //    break;
    //}
    //case AnimationChange::doble:
    //{
    //    //if (!reverse)
    //    //{
    //    animationConfirmation = true;

    //    // 上半身用アニメーション
    //    model->UpdateUpeerBodyAnimation(elapsedTime, "body2");
    //    // 下半身用アニメーション
    //    model->UpdateLowerBodyAnimation(elapsedTime, "body2");


    //    break;
    //}
    //}



    //model->IsPlayUpeerBodyAnimation(elapsedTime);

    //モデル行列更新
    model->UpdateTransform(transform);

    //CollisionStampVsPlayers();

    //CollisionProjectilesVsPlayers();

    //CollisionChargeVsPlayers();

}

//描画処理
void EnemyBOSS::Render(RenderContext& rc, ModelShader* shader)
{
    rc.toonData.edgeWidth = 0.1f;
    shader->Draw(rc, model);

    //弾丸描画処理
    projectileManager.Render(rc, shader);
}

//デバッグプリミティブ描画
void EnemyBOSS::DrawDebugPrimitive()
{
    //基底クラスのデバッグプリミティブ描画
    Enemy::DrawDebugPrimitive();

    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //縄張り範囲をデバッグ円柱描画
    debugRenderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f, DirectX::XMFLOAT4(0, 1, 0, 1));

    //ターゲット位置をデバッグ球を描画
    debugRenderer->DrawSphere(targetPosition, radius, DirectX::XMFLOAT4(1, 1, 0, 1));

    //索敵範囲をデバッグ円柱描画
    debugRenderer->DrawCylinder(position, searchRange, 1.0f, DirectX::XMFLOAT4(0, 0, 1, 1));

    //攻撃範囲をデバッグ円柱描画
    debugRenderer->DrawCylinder(position, attackRange, 1.0f, DirectX::XMFLOAT4(1, 0, 0, 1));

    //スタンプ攻撃範囲をデバッグ円柱描画
    debugRenderer->DrawCylinder(position, attack_stampRange, 1.0f, DirectX::XMFLOAT4(1, 0, 1, 1));
}

//縄張り設定
void EnemyBOSS::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
    territoryOrigin = origin;
    territoryRange = range;
}

//ターゲット位置をランダム設定
void EnemyBOSS::SetRandomTargetPosition()
{
    float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
    float range = Mathf::RandomRange(0.0f, territoryRange);
    targetPosition.x = territoryOrigin.x + sinf(theta) * range;
    targetPosition.y = territoryOrigin.y;
    targetPosition.z = territoryOrigin.z + cosf(theta) * range;
}

//プレイヤー索敵
bool EnemyBOSS::SearchPlayer()
{
    //プレイヤーとの高低差を考慮して３Dでの距離判定をする
    PlayerManager& playerMgr = PlayerManager::Instance();
    const DirectX::XMFLOAT3& playerPosition = playerMgr.Instance().GetPlayer(playerMgr.GetWeapon())->GetPosition();
    
    //BOSSが見える範囲 1000
    float vx = playerPosition.x - position.x;
    float vy = playerPosition.y - position.y;
    float vz = playerPosition.z - position.z;
    float dist = sqrtf(vx * vx + vy * vy + vz * vz);

    if (dist < searchRange)
    {
        float distXZ = sqrtf(vx * vx + vz * vz);
        //単位ベクトル化
        vx /= distXZ;
        vz /= distXZ;
        //全ポベクトル
        float frontX = sinf(angle.y);
        float frontZ = cosf(angle.y);
        //２角部クオルの内積地で前後判定
        float dot = (frontX * vx) + (frontZ * vz);
        if (dot > 0.0f)
        {
            return true;
        }
    }

    return false;
}

//ノードとプレイヤーの衝突処理
void EnemyBOSS::CollisionNodeVsPlayer(const char* nodeName, float nodeRadius)
{
    //ノードの位置と当たり判定を行う
    Model::Node* node = model->FindNode(nodeName);
    
    if (node != nullptr)
    {
        //ノードのワールド座標
        DirectX::XMFLOAT3 nodePosition(
            node->worldTransform._41,
            node->worldTransform._42,
            node->worldTransform._43
        );
        
        //当たり判定表示
        Graphics::Instance().GetDebugRenderer()->DrawSphere(
            nodePosition, nodeRadius, DirectX::XMFLOAT4(0, 0, 1, 1)
        );

        //プレイヤーと当たり判定
        Player* player = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetWeapon());
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectSphereVsCylinder(
            nodePosition,
            nodeRadius,
            player->GetPosition(),
            player->GetRadius(),
            player->GetHeight(),
            outPosition))
        {
            //ダメージを与える
            if (player->ApplyDamage(1, 5.0f))
            {
                //敵をふっとばすベクトルを算出
                DirectX::XMFLOAT3 vec;
                vec.x = outPosition.x - nodePosition.x;
                vec.z = outPosition.z - nodePosition.z;
                float length = sqrtf(vec.x * vec.x + vec.z * vec.z);
                vec.x /= length;
                vec.z /= length;
                
                //XZ平面にふっとばす力をかける
                float power = 20.0f;
                vec.x *= power;
                vec.z *= power;

                //Y方向にも力をかける
                vec.y = 5.0f;

                //吹っ飛ばす
                player->AddImpulse(vec);
            }
        }
    }
}

//目標地点へ移動
void EnemyBOSS::MoveToTarget(float elapsedTime, float speedRate)
{
    //ターゲット方向への進行ベクトルを算出
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vz * vz);
    vx /= dist;
    vz /= dist;

    //移動処理
    Move(vx, vz, moveSpeed * speedRate);
    Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
}

//徘徊ステートへ遷移
void EnemyBOSS::TransitionWanderState()
{
    state = State::Wander;

    //目標地点設定
    SetRandomTargetPosition();

    //static_cast<int>(animationChange);

    //歩きアニメーション再生
    model->PlayAnimation(Anim_Running, true);
}

//徘徊ステート更新処理
void EnemyBOSS::UpdateWanderState(float elapsedTime)
{
    //目標地点までXZ平面での距離判定
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float distSq = vx * vx + vz * vz;
    if (distSq < radius * radius)
    {
        shotTimer = 0;
        //待機ステートへ遷移
        TransitionIdleState();
    }

    //目標地点へ移動
    MoveToTarget(elapsedTime, 0.5f);

    //プレイヤー索敵
    if (SearchPlayer())
    {
        //見つかったら追跡ステートへ遷移
        TransitionPursuitState();
    }
}

//待機ステートへ遷移
void EnemyBOSS::TransitionIdleState()
{
    if (state != State::Eye)
    {
        randclass = rand() % 2;

        if (randclass == 1)
        {
            TransitionEyeState();
           
        }
    }
    if (randclass != 1)
    {

        state = State::Idle;



        //タイマーをランダム設定
        stateTimer = Mathf::RandomRange(3.0f, 5.0f);

        //待機アニメーション再生
        model->PlayAnimation(Anim_Idle, true);

        model->PlayUpeerBodyAnimation(Anim_Idle, true);

        shotgunpos = 1;

        // 範囲攻撃チャージ初期化
        chargetime = 0;
    }
    /*randclass = rand() % 5;*/

    //if (randclass == 1)
    //    TransitionEyeState();

    //attackrand = rand() % 3 + 1;

}

//待機ステート更新処理
void EnemyBOSS::UpdateIdleState(float elapsedTime)
{
    // 最初の即死防止
    if (UI::Instance().GetTime() >= 10500) return;
    stoptime += elapsedTime;

    //タイマー処理
    stateTimer -= elapsedTime;

    if (stateTimer < 0.0f)
    {
        shotstate = false;

        possession = 0;

        //徘徊ステートへ遷移
        TransitionWanderState();
    }

    if (!shotstate)
        //プレイヤー索敵
        if (SearchPlayer())
        {
            //見つかったら追跡ステートへ遷移
            TransitionPursuitState();
        }



    // 球が死んだら
    if (projectileManager.GetProjectileCount() == 0)
    {
        //TransitionEyeState();
        attackrand = 0;
    }

    // もし攻撃が無かったら範囲攻撃を初期化
    if (attackrand == 0)
    {
        colosionarea = 0;
    }
    
}

void EnemyBOSS::TransitionEyeState()
{
    state = State::Eye;
    model->PlayAnimation(Anim_Eye, false);
    // 逆再生
    if (animationChange == AnimationChange::Normal)
    {
        changeopen->Play(position);
      
        reverse = false;
        //return;
    }
    // 再生
    if (animationChange == AnimationChange::doble)
    {
        changeclose->Play(position);
   
        reverse = true;
        //return;
    }
    
    // 再生 閉じる
    //if (animationChange == AnimationChange::Normal)
    //{
    //    changeopen->Play(position);
    // 
    //    changemode = 1;
    //    reverse = false;
    //    model->PlayAnimation(Anim_Eye, false);
    //    
    //}
    //// 逆正再生 開眼
    //if (animationChange == AnimationChange::doble)
    //{
    //    changemode = 0;

    //    changeclose->Play(position);
    //    reverse = true;
    //    animationChange = AnimationChange::Normal;
    //    model->PlayAnimation(Anim_Eye, false);
    //}

    //    // 逆再生
    //if (animationChange == AnimationChange::Normal)
    //{
    //    changeopen->Play(position);

    //    reverse = true;

    //    return;
    //}
    //// 再生
    //if (animationChange == AnimationChange::doble)
    //{
    //    changeclose->Play(position);
    //    animationChange = AnimationChange::Normal;
    //    reverse = false;
    //    return;
    //}
}

void EnemyBOSS::UpdateEyeState(float elapsedTime)
{

    if (!model->IsPlayAnimation() && reverse == true)
    {
        changeclose->stop(changeclose->GetHandle());
        animationChange = AnimationChange::Normal;
        reverse = false;
        randclass = 0;
        TransitionIdleState();
    }
    if (!model->IsPlayAnimation() && reverse == false)
    {
        animationChange = AnimationChange::doble;
        changeopen->stop(changeopen->GetHandle());
        reverse = false;
        randclass = 0;
        TransitionIdleState();
    }


    //if (!model->IsPlayAnimation() && changemode == 1)
    //{

    //    reverse = false;
    //    changeopen->stop(changeopen->GetHandle());
    //    animationChange = AnimationChange::doble;
    //    
    //    TransitionIdleState();

    //}

    //if (!model->IsPlayAnimation() && changemode == 0)
    //{

    //    reverse = false;
    //    changeclose->stop(changeclose->GetHandle());

    //    TransitionIdleState();

    //}


    //if (!model->IsPlayAnimation() && reverse == false)
    //{
    //    //animationChange = AnimationChange::doble;
    //    changeclose->stop(changeclose->GetHandle());
    //    reverse = false;
    //    TransitionIdleState();
    //}

    //if (!model->IsPlayAnimation() && reverse == true)
    //{

    //    changeopen->stop(changeopen->GetHandle());
    //    reverse = false;
    //    animationChange = AnimationChange::doble;
    //    TransitionIdleState();
    //}
}

//追跡ステートへ遷移
void EnemyBOSS::TransitionPursuitState()
{
    state = State::Pursuit;

    //数秒間追跡するタイマーをランダム設定
    stateTimer = Mathf::RandomRange(3.0f, 5.0f);

    //歩きアニメーションを再生
    model->PlayAnimation(Anim_Running, true);

    model->PlayUpeerBodyAnimation(Anim_Eye, true);
}

//追跡ステート更新処理
void EnemyBOSS::UpdatePursuitState(float elapsedTime)
{

    
        PlayerManager& playerMgr = PlayerManager::Instance();

        //目標地点をプレイヤー位置に設定
        targetPosition = playerMgr.Instance().GetPlayer(playerMgr.GetWeapon())->GetPosition();

        //目標地点へ移動
        MoveToTarget(elapsedTime, 1.0f);

        //タイマー処理
        stateTimer -= elapsedTime;
        if (stateTimer < 0.0f)
        {
            //待機ステートへ遷移
            TransitionIdleState();
        }

        //プレイヤーに近づくと攻撃ステートへ遷移
        float vx = targetPosition.x - position.x;
        float vy = targetPosition.y - position.y;
        float vz = targetPosition.z - position.z;
        float dist = sqrtf(vx * vx + vy * vy + vz * vz);

        if (dist < attack_stampRange)
        {
            //攻撃ステートへ遷移
            TransitionAttackState();
        }
        else if (dist < attackRange)
        {
            // 射撃ステートへ遷移
            TransitionAttackGun();
        }

        if (shotstate)
        {
            TransitionIdleState();
        }
    
}

//攻撃ステートへ遷移
void EnemyBOSS::TransitionAttackState()
{
    state = State::Attack;

   // //攻撃アニメーション再生
   //model->PlayAnimation(Anim_AttackCharge, false);

   //model->PlayUpeerBodyAnimation(Anim_AttackCharge, false);
 
    //state = State::Attack;

    ////攻撃アニメーション再生
    //model->PlayAnimation(Anim_Attack_Stamp, false);

    //model->PlayUpeerBodyAnimation(Anim_Attack_Stamp, false);
 
    //if (attackrand == Attack::Attack_stamp)
    //{
    model->PlayAnimation(Anim_Attack_Stamp, false);
    model->PlayUpeerBodyAnimation(Anim_Attack_Stamp, false);
    
    chackeaudiostump = false;

}

//攻撃ステート更新処理
void EnemyBOSS::UpdateAttackState(float elapsedTime)
{
    ////任意のアニメーション再生区間でのみ衝突判定処理をする
    //float animationTime = model->GetCurrentAnimationSeconds();
    //if (animationTime >= 1.0f && animationTime <= 5.00f)
    //{
    //    // 目玉ノードとプレイヤーの衝突処理
    //    CollisionNodeVsPlayer("boss_left_foot1", 5.0f);
    //}
    //{

    //   // NormalGun("blend_root", 2.5f);
    //}
    //フル画面
    ////攻撃アニメーションが終わったら戦闘待機ステートへ遷移
    //if (!model->IsPlayAnimation())
    //{
    //    TransitionIdleBattleState();
    //}
        // 任意のアニメーション再生区間でのみ衝突判定処理をする
    //float animationTime = model->GetCurrentAnimationSeconds();
    //if (animationTime >= 0.1f && animationTime <= 0.35f)
    //{
    //    // 目玉ノードとプレイヤーの衝突処理
    //    CollisionNodeVsPlayer("EyeBall", 4.0f);
    //}

    //// 攻撃アニメーションが終わったら戦闘待機ステートへ遷移
    //if (!model->IsPlayAnimation())
    //{
    //    TransitionIdleBattleState();
    //}

    //Attack::Attack_stamp:
    //任意のアニメーション再生区間でのみ衝突判定処理をする
    float animationTime = model->GetCurrentAnimationSeconds();
    attackCollisionFlag = animationTime >= 1.0f && animationTime <= 3.0f;
    if (attackCollisionFlag)
    {
        //attack_stampRange;
        stamp("boss_left_foot1", 5.0f);
    }
   
    if (animationTime >= 1.0f && animationTime <= 1.1f && !chackeaudiostump)
    {
        stumpaudio->Play(false, 1.0f);
        chackeaudiostump = true;
    }
    CollisionStampVsPlayers();

}

//戦闘待機ステートへ遷移
void EnemyBOSS::TransitionIdleBattleState()
{
    state = State::IdleBattle;

    //数秒間待機するタイマーをランダム設定
    stateTimer = Mathf::RandomRange(3.0f, 3.0f);

    shotstate = false;

    //戦闘待機アニメーション再生
    model->PlayAnimation(Anim_Idle, true);

    model->PlayUpeerBodyAnimation(Anim_Idle, true);

    shotgunpos = 1;

    // 範囲攻撃チャージ初期化
    chargetime = 0;

    // 球が死んだら
    if (projectileManager.GetProjectileCount() == 0)
    {
        attackrand = 0;
    }

    //randclass = rand() % 2;

    //if (randclass == 1)
    //    TransitionEyeState();
}

//戦闘待機ステート更新処理
void EnemyBOSS::UpdateIdleBattleState(float elapsedTime)
{
    //目標地点をプレイヤー位置に設定
    targetPosition = PlayerManager::Instance().GetPlayer(0)->GetPosition();

    if (projectileManager.GetProjectileCount() == 0)
    {
        attackrand = 0;
    }

    //タイマー処理
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        //プレイヤーが攻撃範囲にいた場合は攻撃ステートへ遷移
        float vx = targetPosition.x - position.x;
        float vy = targetPosition.y - position.y;
        float vz = targetPosition.z - position.z;
        float dist = sqrtf(vx * vx + vy * vy + vz * vz);
        if (dist < attackRange)
        {
            //攻撃ステートへ遷移
            TransitionAttackGun();


        }
        if (dist < attack_stampRange)
        {
            TransitionAttackState();
        }
        else
        {
            //徘徊ステートへ遷移
            TransitionWanderState();
        }

        //if (dist < attack_stampRange)
        //{
        //    TransitionAttackState();
        //}
        //else
        //{
        //    //徘徊ステートへ遷移
        //    TransitionWanderState();
        //}
    }
    MoveToTarget(elapsedTime, 0.0f);
    
    if (attackrand == 0)
    {
        colosionarea = 0;
    }
}


//ダメージステートへ遷移
void EnemyBOSS::TransitionDamageState()
{
/*    if (chargetime < chargetimeMax)
    {
      */  // 球が死んだら
    if (projectileManager.GetProjectileCount() == 0)
    {
        attackrand = 0;
    }

   // }
    state = State::Damage;

    //ダメージアニメーション再生
    model->PlayAnimation(Anim_GetHit1, false);
    model->PlayUpeerBodyAnimation(Anim_GetHit1, false);
}

//ダメージステート更新処理
void EnemyBOSS::UpdateDamageState(float elapsedTime)
{
    //ダメージアニメーションが終わったら戦闘待機ステートへ遷移
    if (!model->IsPlayAnimation())
    {
        TransitionIdleBattleState();
    }
}

//死亡ステートへ遷移
void EnemyBOSS::TransitionDeathState()
{
    state = State::Death;

    //ダメージアニメーション再生
    model->PlayAnimation(Anim_Death, false);
}

//死亡ステート更新処理
void EnemyBOSS::UpdateDeathState(float elapsedTime)
{
    //ダメージアニメーションが終わったら自分を履き
    if (!model->IsPlayAnimation())
    {
        EnemyManager::Instance().SetDie(true);
        Destroy();
    }
}


// スタンプとプレイヤーの当たり判定
void EnemyBOSS::CollisionStampVsPlayers()
{
    // 範囲攻撃半径初期化
    colosionarea = 0;

    // プレイヤー
    PlayerManager& playerMgr = PlayerManager::Instance();

    //すべての弾丸と全ての敵を総当りで衝突処理
    int projectileCount = projectileManager.GetProjectileCount();
    for (int i = 0; i < projectileCount; ++i)
    {
        Projectile* projectile = projectileManager.GetProjectile(i);
        DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

        debugRenderer->DrawSphere(projectile->GetPosition(), projectile->GetRadius(), { 1,1,1,1 });

        //衝突処理
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectSphereVsCylinder(
            projectile->GetPosition(),
            projectile->GetRadius(),
            playerMgr.GetPlayer(playerMgr.GetWeapon())->GetPosition(),
            playerMgr.GetPlayer(playerMgr.GetWeapon())->GetRadius(),
            playerMgr.GetPlayer(playerMgr.GetWeapon())->GetHeight(),
            outPosition))
        {
            if (!playerMgr.GetPlayer(playerMgr.GetWeapon())->GetGuardFlag()) {
                //吹き飛ばす
                {
                    //ダメージを与える
                    if (playerMgr.GetPlayer(playerMgr.GetWeapon())->ApplyDamage(damageStamp, 3.0f))
                    {
                        //吹き飛ばす
                        {
                            DirectX::XMFLOAT3 impulse;
                            const float power = 0.0f;
                            const DirectX::XMFLOAT3& e = playerMgr.GetPlayer(playerMgr.GetWeapon())->GetPosition();
                            const DirectX::XMFLOAT3& p = projectile->GetPosition();
                            float vx = e.x - p.x;
                            float vz = e.z - p.z;
                            float lengthXZ = sqrtf(vx * vx + vz * vz);
                            vx /= lengthXZ;
                            vz /= lengthXZ;

                            impulse.x = vx * 10.0f;
                            impulse.y = power * 0.0f;
                            impulse.z = vx * power;

                            playerMgr.GetPlayer(playerMgr.GetWeapon())->AddImpulse(impulse);
                        }

                        //ヒットエフェクト再生
                        {
                            //  DirectX::XMFLOAT3 p = player.GetPosition();
                            //  p.y += player.GetHeight() * 0.5f;
                              /*hitEffect->Play(p);*/
                        }

                        //d hitEffect->Play(player.GetPosition());
                         //弾丸破棄
                        projectile->Destroy();
                    }
                }
            }
        }
    }
}

//単発弾の当たり判定
void EnemyBOSS::CollisionProjectilesVsPlayers()
{
    // 単発攻撃半径初期化
    colosionarea = 0;

    // プレイヤー
    PlayerManager& playerMgr = PlayerManager::Instance();

    //すべての弾丸と全ての敵を総当りで衝突処理
    int projectileCount = projectileManager.GetProjectileCount();
    for (int i = 0; i < projectileCount; ++i)
    {
        Projectile* projectile = projectileManager.GetProjectile(i);
        DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

        debugRenderer->DrawSphere(projectile->GetPosition(), projectile->GetRadius(), { 1,1,1,1 });

        //衝突処理
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectSphereVsCylinder(
            projectile->GetPosition(),
            projectile->GetRadius(),
            playerMgr.GetPlayer(playerMgr.GetWeapon())->GetPosition(),
            playerMgr.GetPlayer(playerMgr.GetWeapon())->GetRadius(),
            playerMgr.GetPlayer(playerMgr.GetWeapon())->GetHeight(),
            outPosition))
        {
            if (!playerMgr.GetPlayer(playerMgr.GetWeapon())->GetGuardFlag()) {
                //吹き飛ばす

                   //ダメージを与える
                if (playerMgr.GetPlayer(playerMgr.GetWeapon())->ApplyDamage(damageGun, 0.95f))
                {

                    damageaudio->Play(false, 1);
                    //DirectX::XMFLOAT3 impulse;
                   // const float power = 10.0f;
                    const DirectX::XMFLOAT3& e = playerMgr.GetPlayer(playerMgr.GetWeapon())->GetPosition();
                    const DirectX::XMFLOAT3& p = projectile->GetPosition();
                    float vx = e.x - p.x;
                    float vz = e.z - p.z;
                    float lengthXZ = sqrtf(vx * vx + vz * vz);
                    vx /= lengthXZ;
                    vz /= lengthXZ;

                   // impulse.x = vx * power;
                    //impulse.y = power * 0.0f;
                    //impulse.z = vx * power;

                   // playerMgr.GetPlayer(playerMgr.GetWeapon())->AddImpulse(impulse);
                }

                //ヒットエフェクト再生
                {
                    //  DirectX::XMFLOAT3 p = player.GetPosition();
                    //  p.y += player.GetHeight() * 0.5f;
                      /*hitEffect->Play(p);*/
                }
            }
            //d hitEffect->Play(player.GetPosition());
             //弾丸破棄
            projectile->Destroy();
        }
    }
}
    


//範囲攻撃
void EnemyBOSS::CollisionChargeVsPlayers()
{ // 当たり判定の拡大 打つ動作の必要がなくなったら
    if (shotstate)
        //colosionarea += 1.9f;
    Graphics::Instance().GetDebugRenderer()->DrawSphere(oldposition, colosionarea, DirectX::XMFLOAT4(1, 0, 1, 1));

    // プレイヤー
    PlayerManager& playerMgr = PlayerManager::Instance();

    //すべての弾丸と全ての敵を総当りで衝突処理
    int projectileCount = projectileManager.GetProjectileCount();
    for (int i = 0; i < projectileCount; ++i)
    {
        Projectile* projectile = projectileManager.GetProjectile(i);

        //衝突処理
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectSphereVsCylinder(
            projectile->GetPosition(),
            colosionarea,
            playerMgr.GetPlayer(playerMgr.GetWeapon())->GetPosition(),
            playerMgr.GetPlayer(playerMgr.GetWeapon())->GetRadius(),
            playerMgr.GetPlayer(playerMgr.GetWeapon())->GetHeight(),
            outPosition))
        {
            if (!playerMgr.GetPlayer(playerMgr.GetWeapon())->GetGuardFlag()) {
                //ダメージを与える
                if (playerMgr.GetPlayer(playerMgr.GetWeapon())->ApplyDamage(damageCharge, 0.95f))
                {

                    //吹き飛ばす
                    {
                        damageaudio->Play(false, 1);
                        /*                  DirectX::XMFLOAT3 impulse;
                                          const float power = 0.0f;*/
                        const DirectX::XMFLOAT3& e = playerMgr.GetPlayer(playerMgr.GetWeapon())->GetPosition();
                        const DirectX::XMFLOAT3& p = projectile->GetPosition();
                        float vx = e.x - p.x;
                        float vz = e.z - p.z;
                        float lengthXZ = sqrtf(vx * vx + vz * vz);
                        vx /= lengthXZ;
                        vz /= lengthXZ;

                        //impulse.x = vx * power;
                        ////impulse.y = power * 1.0f;
                        //impulse.z = vx * power;

                        //playerMgr.GetPlayer(playerMgr.GetWeapon())->AddImpulse(impulse);
                    }

                    //ヒットエフェクト再生
                    {
                        //DirectX::XMFLOAT3 p = player.GetPosition();
                        //p.y += player.GetHeight() * 0.5f;
                        /*hitEffect->Play(p);*/
                    }

                    //弾丸破棄
                   // projectile->Destroy();
                  // hitEffect->Play(playerMgr.GetPlayer(playerMgr.GetWeapon())->GetPosition());

                }
            }
        }
    }
}

void EnemyBOSS::TransitionAttackGun()
{
    state = State::AttackGun;                  
     //攻撃種類
    attackrand = rand() % 2 + 1;

    // 範囲攻撃なら特別エフェクトを
    if (attackrand == Attack::Attack_Charge)
    {
        model->PlayAnimation(Anim_AttackCharge, false);
        model->PlayUpeerBodyAnimation(Anim_AttackCharge, false);
        chargeEffect->Play(DirectX::XMFLOAT3{ position.x,position.y + height, position.z });
        //model->PlayAnimation(Anim_Walking, true);

    }
    else
    {
        //model->PlayAnimation(Anim_Attack_Stamp, false);

        model->PlayAnimation(Anim_Attack_NormalGun, false);
        model->PlayUpeerBodyAnimation(Anim_Attack_NormalGun, false);
        
        //SE_Shot->Play(false);
        shotaudio->Play(false, 1);
    }

}

void EnemyBOSS::UpdateAttackGun(float elapsedTime)
{ 
    // 任意のアニメーション再生区間でのみ衝突判定処理をする
    float animationTime = model->GetCurrentAnimationSeconds();
    PlayerManager& playerMgr = PlayerManager::Instance();
       
    ////攻撃
    switch (attackrand)
    {
    //case Attack::Attack_stamp:
    //    //任意のアニメーション再生区間でのみ衝突判定処理をする
    //    attackCollisionFlag = animationTime >= 1.0f && animationTime <= 3.0f;
    //    if (attackCollisionFlag)
    //    { 
    //        //attack_stampRange;
    //        stamp("boss_left_foot1", 5.0f);
    //    }
    //    break;

    case Attack::Attack_Gun:
    {
        attackCollisionFlag = animationTime >= 0.8f && animationTime <= 2.0f; //1.0f, 2.0f
        if (attackCollisionFlag)
        {
            //attackRange;
            NormalGun("body2", 10.0f);
        }
        break;
    }
    case Attack::Attack_Charge:
    {
        ++chargetime;
        chargeEffect->SetPosition(chargeEffect->GetHandle(), DirectX::XMFLOAT3{ position.x,position.y + height * 2,position.z });
        attackCollisionFlag = animationTime >= 0.5f && animationTime <= 10.0f;//0.5f, 10.0f
        if (chargetime >= chargetimeMax)
        {   // Character.h のところの　int chargetimeMax = 260;　にするときれいに治る 
            //attackRange;

            ChargeGun("boss_left_hand1", 10.0f);
        }

        if (animationTime >= 1.0f && animationTime <= 1.1f)
            hotaudio->Play(false, 1);
        if (animationTime >= 1.9f && animationTime <= 2.0f)
        {
            hotaudio->Stop();
            shotaudio->Play(false, 1);
        }
        break;
    }
    }    

    if (shotstate)
    {
        
        shotaudio->Stop();
        TransitionIdleState();
    }
}

// 映像用
void EnemyBOSS::TransitionAttackDemoState()
{
    state = State::AttackDemo;

    //ダメージアニメーション再生
    model->PlayAnimation(Anim_GetHit1, false);

    camerachange = false;
}
// 映像用animation
void EnemyBOSS::UpdateAttackDemoState(float elapsedTime)
{
    //ノード取得
    Model::Node* nodeL = model->FindNode("boss_left_foot1");
    Model::Node* nodeR = model->FindNode("boss_right_foot1");
    
    // 足元の情報left
    DirectX::XMFLOAT3 footPosL = {
        nodeL->worldTransform._41,
        nodeL->worldTransform._42,
        nodeL->worldTransform._43 + 1
    };

    // 足元の情報right
    DirectX::XMFLOAT3 footPosR = {
        nodeR->worldTransform._41,
        nodeR->worldTransform._42,
        nodeR->worldTransform._43 + 1
    };

    // 任意のアニメーション再生区間でのみ衝突処理をする
    float animationTime = model->GetCurrentAnimationSeconds();

    // if以外なら
    // カメラ揺れ
    shackChack = false;
    //// カメラアップ
    rastapcamera = false;

    // エフェクト停止
    smork->stop(smork->GetHandle());
   
    // 任意のアニメーション再生区間でのみ衝突処理をする
    //attackCollisionFlag = animationTime >= 1.0f && animationTime <= 3.0f;
    
    // 時間内なら揺れる
    // 左足
    if (animationTime >= 0.8f && animationTime <= 1.4f || 
        animationTime >= 4.7f && animationTime <= 5.0f 
        
        )
    {
        shackChack = true;
        //smork->Play(footPosL, 0.7f);
        
    
    }
    // 音
    if (animationTime >= 0.8f && animationTime <= 0.9f ||
        animationTime >= 4.7f && animationTime <= 4.9||
        animationTime >= 3.8f && animationTime <= 4.0f
        )
    {
        stumpaudio->Play(false, 1);
    }
    else
    {
        stumpaudio->Stop();
    }
    
    //if (animationTime >= 4.7f && animationTime <= 4.9f)
    //    stumpaudio->Play(false, 1);
    //if (animationTime >= 3.8f && animationTime <= 4.0f)
    //    stumpaudio->Play(false, 1);
    if (animationTime >= 6.9f && animationTime <= 7.5f)
        hotaudio->Play(false, 1);
    else
    {
        hotaudio->Stop();
    }
    // 右足
    if (animationTime >= 3.9f && animationTime <= 4.3f)
    {
        shackChack = true;
        //smork->Play(footPosR, 0.5f);
    }

    // 右足
    if (animationTime >= 6.7f && animationTime <= 7.6f)
    {
        shackChack = true;
        rastapcamera = true;
        //smork->Play(footPosR,0.6f);
    }

    if (animationTime >= 0.8f && animationTime <= 0.9f)
        smork->Play(footPosL, 0.7f);

    if (animationTime >= 4.7f && animationTime <= 4.8f)
        smork->Play(footPosL, 0.5f);


    if (animationTime >= 3.9f && animationTime <= 4.0f)
        smork->Play(footPosR, 0.5f);


    if (animationTime >= 6.7f && animationTime <= 6.7f)
        smork->Play(footPosR, 0.5f);
    // フェイドアウト開始
    if (animationTime >= 8.2f)
        fadeoutcamera = true;

    // animationが終わったらカメラの権利をplayerに戻す
    if (!model->IsPlayAnimation())
    {
        shackChack = false;
      
        camerachange = true;
        fadeoutcamera = false;
        //TransitionIdleState();
        TransitionEyeState();
    }
}

//スタンプ攻撃
void EnemyBOSS::stamp(const char* nodeName, float nodeRadius)
{
    //任意のアニメーション再生区間でのみ衝突判定処理をする
    //float animationTime = model->GetCurrentAnimationSeconds();
    //if (animationTime >= 1.0f && animationTime <= 5.00f)
    //{
    //shotTimer -= 1;

        // 目玉ノードとプレイヤーの衝突処理
        CollisionNodeVsPlayer("boss_left_foot1", 5.0f);
    //}
    //{

       // NormalGun("blend_root", 2.5f);
   // }

    //攻撃アニメーションが終わったら戦闘待機ステートへ遷移
    if (!model->IsPlayAnimation() && animationChange== AnimationChange::Normal )
    {
        TransitionIdleBattleState();
    }
    if (!model->IsPlayAnimation() && !model->IsPlayUpeerBodyAnimation()&& animationChange == AnimationChange::doble)
    {
        TransitionIdleBattleState();
    }
}

//ノーマルガン
void EnemyBOSS::NormalGun(const char* nodeName, float nodeRadius)
{
    shotTimer -= 1;

    //ノード取得
    Model::Node* node = model->FindNode(nodeName);
    
    PlayerManager& playerMgr = PlayerManager::Instance();
    
    //ノード位置取得
    DirectX::XMFLOAT3 nodePosition;
    nodePosition.x = node->worldTransform._41;
    nodePosition.y = node->worldTransform._42;
    nodePosition.z = node->worldTransform._43;

    //if (shotTimer % num <= 0)
    if (shotTimer <= 0)
    {
        //前方向
        DirectX::XMFLOAT3 dir;

        dir.x = playerMgr.Instance().GetPlayer(playerMgr.GetWeapon())->GetPosition().x;
        dir.y = playerMgr.Instance().GetPlayer(playerMgr.GetWeapon())->GetPosition().y;
        dir.z = playerMgr.Instance().GetPlayer(playerMgr.GetWeapon())->GetPosition().z;

        //発射位置
        DirectX::XMFLOAT3 pos;
        pos.x = nodePosition.x;

        //方向ベクトルを出してプレイヤーのワルド座標を銃の位置までの計算し変換
        pos.y = nodePosition.y;
        pos.z = nodePosition.z;

        //発射
        EnemySkillGun* projectile = new EnemySkillGun(&projectileManager, "", "Data/Model/Bullet/Ruby.mdl"); //jewel(2).mdl
        projectile->Launch(dir, { pos.x,pos.y,pos.z }, {}, 20);

        shotTimer = 50;
        ++possession;
    }
    else if (possession == 5)
    {
        shotstate = true;
    }
}

//範囲攻撃
void EnemyBOSS::ChargeGun(const char* nodeName, float nodeRadius)
{
    shotTimer -= 1;

    //ノード取得
    Model::Node* node = model->FindNode(nodeName);

    //ノード位置取得
    DirectX::XMFLOAT3 nodePosition;
    nodePosition.x = node->worldTransform._41;
    nodePosition.y = node->worldTransform._42;
    nodePosition.z = node->worldTransform._43;

    

    //if (shotTimer % num <= 0)
    if (shotTimer <= 0)
    {

        //前方向
        DirectX::XMFLOAT3 dir;

        dir.x = sinf(/*this->angle.x +*/ shotgunpos - 0.5f);
        dir.y = 0.0f;//5.0f;
        dir.z = cosf(/*this->angle.y +*/ shotgunpos - 0.5f);
        
        //発射位置
        DirectX::XMFLOAT3 pos;
        pos.x = nodePosition.x;
        
        //方向ベクトルを出してプレイヤーのワルド座標を銃の位置までの計算し変換
        pos.y = nodePosition.y;
        pos.z = position.z;

        ++shotgunpos;
        
       //発射
        EnemySkillGunCharge* projectile = new EnemySkillGunCharge(&projectileManager, "", "Data/Model/Bullet/Ruby.mdl");
        projectile->Launch(dir, { pos.x,pos.y - 3.0f,pos.z }, {}, 25);

       oldposition = pos;

        shotTimer = 2;
        
        ++possession;
    }
    else if (possession == 70)
    {
        shotstate = true;
    }
}

//死亡したときに呼ばれる
void EnemyBOSS::OnDead()
{
    //死亡ステートへ遷移
    TransitionDeathState();
}

//ダメージを受けたときに呼ばれる
void EnemyBOSS::OnDamaged()
{
    //ダメージステートへ遷移
    //TransitionDamageState();
}
