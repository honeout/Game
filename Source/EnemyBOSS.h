#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"
#include "ProjectileManager.h"
#include "Effect.h"

#include <Audio/Audio.h>
#include <Audio/AudioSource.h>

//スライム
class EnemyBOSS : public Enemy
{
public:

    //インスタンス取得  
    static EnemyBOSS& Instance();

    EnemyBOSS();
    ~EnemyBOSS() override;

    //更新処理
    void Update(float elapsedTime) override;

    //描画処理
    void Render(RenderContext& rc, ModelShader* shader) override;

    //デバッグプリミティブ描画
    void DrawDebugPrimitive() override;

    //縄張り設定
    void SetTerritory(const DirectX::XMFLOAT3& origin, float range);

private:
    //ターゲット位置をランダム設定
    void SetRandomTargetPosition();

    //プレイヤー索敵
    bool SearchPlayer();

    //ノードとプレイヤーの衝突処理
    void CollisionNodeVsPlayer(const char* nodeName, float nodeRadius);

    //目標地点へ移動
    void MoveToTarget(float elapsedTime, float speedRate);

    //徘徊ステートへ遷移
    void TransitionWanderState();

    //徘徊ステート更新処理
    void UpdateWanderState(float elapsedTime);

    //待機ステートへ遷移
    void TransitionIdleState();

    //待機ステート更新処理
    void UpdateIdleState(float elapsedTime);

    //目の見開くステートへ遷移
    void TransitionEyeState();

    //目の見開くステート更新処理
    void UpdateEyeState(float elapsedTime);

    //追跡ステートへ遷移
    void TransitionPursuitState();

    //追跡ステート更新処理
    void UpdatePursuitState(float elapsedTime);

    //攻撃ステートへ遷移
    void TransitionAttackState();

    //攻撃ステート更新処理
    void UpdateAttackState(float elapsedTime);

    //戦闘待機ステートへ遷移
    void TransitionIdleBattleState();

    //戦闘待機ステート更新処理
    void UpdateIdleBattleState(float elapsedTime);

    //ダメージステートへ遷移
    void TransitionDamageState();

    //ダメージステート更新処理
    void UpdateDamageState(float elapsedTime);

    //死亡ステートへ遷移
    void TransitionDeathState();

    //死亡ステート更新処理
    void UpdateDeathState(float elapsedTime);

    //敵スタンプの衝突処理
    void CollisionStampVsPlayers();

    //敵弾丸の衝突処理
    void CollisionProjectilesVsPlayers();

    // 範囲攻撃の当たり判定
    void CollisionChargeVsPlayers();

    //遠距離攻撃ステートへ遷移
    void TransitionAttackGun();

    //遠距離攻撃ステート更新処理
    void UpdateAttackGun(float elapsedTime);

    // 攻撃でも用
    void TransitionAttackDemoState();
    // 攻撃でも用更新
    void UpdateAttackDemoState(float elapsedTime);

    //スタンプ攻撃
    void stamp(const char* nodeName, float nodeRadius);

    //通常弾攻撃 
    void NormalGun(const char* nodeName, float nodeRadius);

    //範囲チャージ攻撃 
    void ChargeGun(const char* nodeName, float nodeRadius);

   // void CollisionPlayerVsFlower();

   

private:
    //ステート
    enum class State
    {
        Wander,
        Idle,
        Eye,
        Pursuit,
        Attack,
        AttackGun,
        Damage,
        Death,
        IdleBattle,
        AttackDemo,
    };

    //アニメーション
    enum Animation
    {
        Anim_Running,           //走る
        Anim_Idle,              //待機
        Anim_AttackCharge,      //チャージ攻撃
        Anim_Attack_Stamp,      //スタンプ
        Anim_Eye,               //目の弱点
        Anim_Attack_NormalGun,         //ノーマルガン(単発弾)
        Anim_Death,             //死亡
        Anim_GetHit1,           //ヒット１
        //Anim_GetHit2,           //ヒット２
        Anim_movie              //ボス映像用
    };

    enum Attack
    {
        //Attack_stamp = 1,
        Attack_Gun= 1,
        Attack_Charge,
    };



protected:
    //死亡したときに呼ばれる
    void OnDead() override;

    //ダメージを受けたときに呼ばれる
    void OnDamaged() override;

private:

    int damageStamp = 0;
    int damageGun = 0;
    int damageCharge = 0;
    int damage = 0;

    int attackrand = 0;                     // 攻撃ランダム用

    int possession = 0;                     //連続射撃数
    
    float speed = 5.0f;
    float lifeTimer = 1.0f;

    // 攻撃する瞬間のアニメーション
    bool attackCollisionFlag = false;

    // チャージのエリアの増量
    float colosionarea = 0;

    Model* model = nullptr;
    State state = State::Wander;
    DirectX::XMFLOAT3 targetPosition = { 0,0,0 };
    DirectX::XMFLOAT3 territoryOrigin = { 0,0,0 };

    // コリジョンの位置更新
    DirectX::XMFLOAT3 oldposition = { 0,0,0 };

    float territoryRange = 100.0f;
    float moveSpeed = 3.0f;
    float turnSpeed = DirectX::XMConvertToRadians(560);
    float stateTimer = 0.0f;
    float searchRange = 1000000000.0f;
    float attackRange = 45.0f;
    float attack_stampRange = 8.0f;
    ProjectileManager projectileManager;
    
    Effect* damageEffect = nullptr;         // 溜め球エフェクト
    Effect* hitEffect = nullptr;            // 食らうエフェクト
    Effect* chargeEffect = nullptr;         // 溜めエフェクト
    Effect* smork = nullptr;                // 砂煙エフェクト

    std::unique_ptr<Effect> changeopen;     // 目開き状態
    std::unique_ptr<Effect> changeclose;    // 目閉じる状態


    float shotgunpos = 1;

    float                   moveRate = 0.0f;



    int randclass = 0;

    // 逆再生する
    bool reverse = false;

    float stoptime = 0;

    std::unique_ptr<AudioSource> stumpaudio;
    std::unique_ptr<AudioSource> chageaudio;
    std::unique_ptr<AudioSource> shotaudio;
    std::unique_ptr<AudioSource> damageaudio;
    std::unique_ptr<AudioSource> hotaudio;
    std::unique_ptr<AudioSource> iceaudio;

    bool chackeaudiostump = false;

    int changemode = 0;
public:
        int num = 0;
};