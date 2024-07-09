#pragma once

#include "Graphics/Model.h"
#include "Player.h"
#include "ProjectileManager.h"
#include "StateMachineRobot.h"
#include "Audio/Audio.h"
#include "shiled.h"

//ロボット
class PlayerGreat : public Player
{
public:
    PlayerGreat();
    ~PlayerGreat() override;

    //更新処理
    void Update(float elapsedTime) override;

    //描画処理
    void Render(RenderContext& rc, ModelShader* shader) override;

    //デバッグ用GUI描画
    void DrawDebugGUI();

    //デバッグプリミティブ描画
    void DrawDebugPrimitive();

    //弾丸と敵の衝突処理
    void CollisionProjectilesVsEnemies();

    //プレイヤーとエネミーとの衝突処理
    void CollisionPlayerVsEnemies();

    //ノードとエネミーの衝突処理
    void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);

    //モデル取得
    Model* GetModel() { return model; }

    //スティック入力値から移動ベクトルを取得
    DirectX::XMFLOAT3 GetMoveVec() const;

    //移動入力処理
    bool InputMove(float elapsedTime);

    //歩き入力処理
    bool InputWalk(float elapsedTime);

    //走り入力処理
    bool InputRun(float elpsedTime);

    //回避入力処理
    bool InputDodge(float elpsedTime);

    ////弾丸入力処理
    //void InputProjectile();

    //攻撃入力処理
    bool InputAttack();

    //武器変更
    bool InputWeapon();

    //ガード入力処理
    bool InputGuard(float elapsedTime);

    ////リロード入力処理
    //bool InputReload();

private:
    //大剣 GreatSword
    //待機ステートへ遷移
    void TransitionIdle_GreatState();

    //待機ステート更新処理
    void UpdateIdle_GreatState(float elapsedTime);

    //歩きステートへ遷移
    void TransitionWalk_GreatState();

    //歩きステート更新処理
    void UpdateWalk_GreatState(float elapsedTime);

    //走りステートへ遷移
    void TransitionRun_GreatState();

    //走りステート更新処理
    void UpdateRun_GreatState(float elapsedTime);

    ////ジャンプステートへ遷移
    //void TransitionJump_GreatState();

    ////ジャンプステート更新処理
    //void UpdateJump_GreatState(float elapsedTime);

    //着地ステートへ遷移
    void TransitionLand_GreatState();

    //着地ステート更新処理
    void UpdateLand_GreatState(float elapsedTIme);

    //攻撃ステートへ遷移
    void TransitionAttack_GreatState();

    //攻撃ステート更新処理
    void UpdateAttack_GreatState(float elapsedTime);

    //ダメージステートへ遷移
    void TransitionDamage_GreatState();

    //ダメージステート更新処理
    void UpdateDamage_GreatState(float elapsedTime);

    //死亡ステートへ遷移
    void TransitionDeath_GreatState();

    //死亡ステート更新処理
    void UpdateDeath_GreatState(float elapsedTime);

    ////回避ステートへ遷移
    //void TransitionDodge_GreatState();

    ////回避ステート更新処理
    //void UpdateDodge_GreatState(float elapsedTime);

    //ガードステートへ遷移
    void TransitionGuard_GreatState();

    //ガードステート更新処理
    void UpdateGuard_GreatState(float elapsedTime);

    //武器変更ステート遷移
    void TransitionChange_GreatState();

    //武器変更ステート更新処理
    void UpdateChange_GreatState(float elapsedTime);

    //ブレーキステート遷移
    void TransitionBrake_GreatState();

    //ブレーキステート更新処理
    void UpdateBrake_GreatState(float elapsedTime);

private:
    //ステート
    enum class State
    {
        //大剣 GreaatSword
        Idle_Great, //待機
        Walk_Great, //歩き
        Run_Great,  //走り
        Jump_Great, //ジャンプ
        Land_Great, //着地
        Attack_Great, //攻撃
        Damage_Great, //ダメージ
        Death_Great, //死亡
        Dodge_Great, //回避
        Guard_Great, //ガード
        Change_Great, //武器変更
        Brake_Great,
    };

private:
    //大剣アニメーション
    enum GreatSwordAnimation
    {
        Anim_Great_Idle, //待機
        Anim_Great_Dash, //ダッシュ
        Anim_Great_Attack_1,   //攻撃
        Anim_Great_Attack_2,
        Anim_Great_Attack_3,
        Anim_Great_Damaged,
        Anim_Great_Guard,
        Anim_Great_Guard_Damaged,
        Anim_Great_Brake,
        Anim_Great_Dodge,    //回避
        Anim_Great_Change,   //武器変更
        Anim_Great_GetHit,   //ヒット
        Anim_Great_Death,    //死亡
        Anim_Great_Jump,     //ジャンプ
        Anim_Great_Falling,  //落ちている
        Anim_Great_Landing,  //着地
    };
protected:
    ////着地したときに呼ばれる
    //void OnLanding() override;

    //死亡したときに呼ばれる
    void OnDead() override;

    //ダメージを受けたときに呼ばれる
    void OnDamaged() override;

private:
    Model* model = nullptr;
    Shiled* weaponModel = nullptr;
    float moveSpeed = 5.0f;
    float turnSpeed = DirectX::XMConvertToRadians(720);
    float jumpSpeed = 20.0f;
    int jumpCount = 0;      //何回飛んだか
    int jumpLimit = 1;      //ジャンプ回数制限
    ProjectileManager projectileManager;
    Effect* hitEffect = nullptr;
    Effect* invincible_hitEffect = nullptr;
    Effect* guardmode = nullptr;
    State state = State::Idle_Great;      //ステート
    float greatRadius = 0.8f;
    bool attackCollisionFlag = false;
    bool attackCollisionFlagLast = false;
    int statenumber = 0;
    int weapon = 0; //武器用変数
    float changetime = 0.0f;
    int attackcount = 0;
    int bullets = 60;   //残弾数
    float reloadtime = 0.0f;
    int totalattackcount = 0;
    float attacktime = 0.0f;
    float attackrate = 0.0f;

    // ガードの時間
    int guardtime;

    std::unique_ptr<AudioSource> attack_01;
    std::unique_ptr<AudioSource> attack_02;
    std::unique_ptr<AudioSource> attack_03;
    std::unique_ptr<AudioSource> guard;
    std::unique_ptr<AudioSource> damaged;
    std::unique_ptr<AudioSource> brake;
    std::unique_ptr<AudioSource> run_fast;
    std::unique_ptr<AudioSource> run_slow;
};