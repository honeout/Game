#pragma once

#include "Graphics/Model.h"
#include "Player.h"
#include "ProjectileManager.h"
#include "StateMachineRobot.h"
#include "Audio/Audio.h"

//ロボット
class PlayerDual : public Player
{
public:
    PlayerDual();
    ~PlayerDual() override;

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

    //ジャンプ入力処理
    bool InputJump();

    //弾丸入力処理
    void InputProjectile();

    //攻撃入力処理
    bool InputAttack();

    //武器変更
    bool InputWeapon();

    //ガード入力処理
    bool InputGuard(float elapsedTime);

    //リロード入力処理
    bool InputReload();

private:
    //双剣 DualBlades
    //待機ステートへ遷移
    void TransitionIdle_DualState();

    //待機ステート更新処理
    void UpdateIdle_DualState(float elapsedTime);

    //歩きステートへ遷移
    void TransitionWalk_DualState();

    //歩きステート更新処理
    void UpdateWalk_DualState(float elapsedTime);

    //走りステートへ遷移
    void TransitionRun_DualState();

    //走りステート更新処理
    void UpdateRun_DualState(float elapsedTime);


    //攻撃ステートへ遷移
    void TransitionAttack_DualState();

    //攻撃ステート更新処理
    void UpdateAttack_DualState(float elapsedTime);

    //ダメージステートへ遷移
    void TransitionDamage_DualState();

    //ダメージステート更新処理
    void UpdateDamage_DualState(float elapsedTime);

    //死亡ステートへ遷移
    void TransitionDeath_DualState();

    //死亡ステート更新処理
    void UpdateDeath_DualState(float elapsedTime);

    //回避ステートへ遷移
    void TransitionDodge_DualState();

    //回避ステート更新処理
    void UpdateDodge_DualState(float elapsedTime);

    //武器変更ステート遷移
    void TransitionChange_DualState();

    //武器変更ステート更新処理
    void UpdateChange_DualState(float elapsedTime);

    //ブレーキステート遷移
    void TransitionBrake_DualState();

    //ブレーキステート更新処理
    void UpdateBrake_DualState(float elapsedTime);

private:
    //ステート
    enum class State
    {
        //双剣 DualBlades
        Idle_Dual, //待機
        Walk_Dual, //歩き
        Run_Dual,  //走り
        Jump_Dual, //ジャンプ
        Land_Dual, //着地
        Attack_Dual, //攻撃
        Damage_Dual, //ダメージ
        Death_Dual,  //死亡
        Dodge_Dual,  //回避
        Change_Dual, //武器変更
        Brake_Dual, //ブレーキ
    };

private:
    //双剣アニメーション
    enum DualSwordAnimation
    {
        Anim_Dual_Idle, //待機
        Anim_Dual_Dash, //ダッシュ
        Anim_Dual_Attack_1,   //攻撃
        Anim_Dual_Attack_2,
        Anim_Dual_Attack_3,
        Anim_Dual_Damaged,
        Anim_Dual_Brake,
        Anim_Dual_Change,   //武器変更
        Anim_Dual_Dodge,    //回避
        Anim_Dual_GetHit,   //ヒット
        Anim_Dual_Death,    //死亡
        Anim_Dual_Jump,     //ジャンプ
        Anim_Dual_Falling,  //落ちている
        Anim_Dual_Landing,  //着地
    };
protected:


    //死亡したときに呼ばれる
    void OnDead() override;

    //ダメージを受けたときに呼ばれる
    void OnDamaged() override;

private:
    Model* model = nullptr;
    float moveSpeed = 5.0f;
    float turnSpeed = DirectX::XMConvertToRadians(720);
    float jumpSpeed = 20.0f;
    int jumpCount = 0;      //何回飛んだか
    int jumpLimit = 1;      //ジャンプ回数制限
    ProjectileManager projectileManager;
    Effect* hitEffect = nullptr;
    Effect* invincible_hitEffect = nullptr;
    State state = State::Idle_Dual;      //ステート
    float dualRadius = 0.8f;
    bool attackCollisionFlag = false;
    int statenumber = 0;
    int weapon = 0; //武器用変数
    float changetime = 0.0f;
    int attackcount = 0;
    int bullets = 60;   //残弾数
    float reloadtime = 0.0f;
    int totalattackcount = 0;
    float attacktime = 0.0f;
    float attackrate = 0.0f;
    std::unique_ptr<AudioSource> attack_01;
    std::unique_ptr<AudioSource> attack_02;
    std::unique_ptr<AudioSource> attack_03;
    std::unique_ptr<AudioSource> damaged;
    std::unique_ptr<AudioSource> brake;
    std::unique_ptr<AudioSource> run_fast;
    std::unique_ptr<AudioSource> run_slow;

    static const int MAX_POLYGON = 32;
    DirectX::XMFLOAT3					trailPositions[2][MAX_POLYGON];
    Model::Node* node;

    Model::Node* weponNode;

};