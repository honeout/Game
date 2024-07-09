#pragma once

#include "Graphics/Model.h"
#include "Player.h"
#include "ProjectileManager.h"
#include "StateMachineRobot.h"
#include "CameraController.h"
#include "Audio/Audio.h"

//ロボット
class PlayerGun : public Player
{
public:
    PlayerGun();
    ~PlayerGun() override;

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
    //銃 Gun
    //待機ステートへ遷移
    void TransitionIdle_GunState();

    //待機ステート更新処理
    void UpdateIdle_GunState(float elapsedTime);

    //歩きステートへ遷移
    void TransitionWalk_GunState();

    //歩きステート更新処理
    void UpdateWalk_GunState(float elapsedTime);

    //走りステートへ遷移
    void TransitionRun_GunState();

    //走りステート更新処理
    void UpdateRun_GunState(float elapsedTime);

    //攻撃ステートへ遷移
    void TransitionAttack_GunState();

    //攻撃ステート更新処理
    void UpdateAttack_GunState(float elapsedTime);

    //ダメージステートへ遷移
    void TransitionDamage_GunState();

    //ダメージステート更新処理
    void UpdateDamage_GunState(float elapsedTime);

    //死亡ステートへ遷移
    void TransitionDeath_GunState();

    //死亡ステート更新処理
    void UpdateDeath_GunState(float elapsedTime);

    //回避ステートへ遷移
    void TransitionDodge_GunState();

    //回避ステート更新処理
    void UpdateDodge_GunState(float elapsedTime);

    //リロードステートへ遷移
    void TransitionReload_GunState();

    //リロードステート更新処理
    void UpdateReload_GunState(float elapsedTime);

    //武器変更ステートへ遷移
    void TransitionChange_GunState();

    //武器変更ステート更新処理
    void UpdateChange_GunState(float elapsedTime);

    //ブレーキステート遷移
    void TransitionBrake_GunState();

    //ブレーキステート更新処理
    void UpdateBrake_GunState(float elapsedTime);

private:
    //ステート
    enum class State
    {
        //銃　Gun
        Idle_Gun, //待機
        Walk_Gun, //歩き
        Run_Gun,  //走り
        Jump_Gun, //ジャンプ
        Land_Gun, //着地
        Attack_Gun, //攻撃
        Damage_Gun, //ダメージ
        Death_Gun, //死亡
        Dodge_Gun, //回避
        Reload_Gun, //リロード
        Change_Gun, //武器変更
        Brake_Gun,
    };

private:
    //銃アニメーション
    enum GunAnimation
    {
        Anim_Gun_Idle, //待機
        Anim_Gun_Dash, //ダッシュ
        Anim_Gun_Dash_02,
        Anim_Gun_Attack,   //攻撃
        Anim_Gun_Damaged,
        Anim_Gun_Reload,
        Anim_Gun_Brake,
        Anim_Gun_Dodge,    //回避
        Anim_Gun_Change,   //武器変更
        Anim_Gun_GetHit,   //ヒット
        Anim_Gun_Death,    //死亡
        Anim_Gun_Jump,     //ジャンプ
        Anim_Gun_Falling,  //落ちている
        Anim_Gun_Landing,  //着地
    };

    enum class LockonState
    {
        NotLocked,
        Locked,
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
    Effect* shotEffect = nullptr;
    State state = State::Idle_Gun;      //ステート
    float leftHandRadius = 0.4f;
    bool attackCollisionFlag = false;
    int statenumber = 0;
    int weapon = 0; //武器用変数
    float changetime = 0.0f;
    
    int bullets = 60;   //残弾数
    float reloadtime = 0.0f;
    int totalattackcount = 0;
    float rate = 0.0f;
    float braketime = 0.0f;
    CameraController* cameraController = nullptr;
    std::unique_ptr<AudioSource> shot;
    std::unique_ptr<AudioSource> reload;
    std::unique_ptr<AudioSource> damaged;
    std::unique_ptr<AudioSource> brake;
    std::unique_ptr<AudioSource> run_fast;
    std::unique_ptr<AudioSource> run_slow;

    Model::Node* weponNode;
};