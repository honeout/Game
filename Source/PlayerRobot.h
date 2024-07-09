#pragma once

#include "Graphics/Model.h"
#include "Player.h"
#include "ProjectileManager.h"
#include "StateMachineRobot.h"

//アニメーション
enum class RobotAnimation
{
    Attack,            //攻撃
    Death,             //死亡
    Falling,           //落ちる
    GetHit1,           //ヒット１
    GetHit2,           //ヒット２
    Idle,              //待機
    Jump,              //ジャンプ
    Jump_Flip,         //
    Landing,           //
    Revive,            //起き上がり
    Running,           //走る
    Walking            //歩く
};

//ロボット
class PlayerRobot : public Player
{
public:
    PlayerRobot();
    ~PlayerRobot() override;


    //インスタンス取得
    static PlayerRobot& Instance();

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
    //武器未装着
    //待機ステートへ遷移
    void TransitionIdleState();

    //待機ステート更新処理
    void UpdateIdleState(float elapsedTime);

    //移動ステートへ遷移
    void TransitionMoveState();

    //移動ステート更新処理
    void UpdateMoveState(float elapsedTime);

    //ジャンプステートへ遷移
    void TransitionJumpState();

    //ジャンプステート更新処理
    void UpdateJumpState(float elapsedTime);

    //着地ステートへ遷移
    void TransitionLandState();

    //着地ステート更新処理
    void UpdateLandState(float elapsedTIme);

    //攻撃ステートへ遷移
    void TransitionAttackState();

    //攻撃ステート更新処理
    void UpdateAttackState(float elapsedTime);

    //ダメージステートへ遷移
    void TransitionDamageState();

    //ダメージステート更新処理
    void UpdateDamageState(float elapsedTime);

    //死亡ステートへ遷移
    void TransitionDeathState();

    //死亡ステート更新処理
    void UpdateDeathState(float elapsedTime);

    //復活ステートへ遷移
    void TransitionReviveState();

    //復活ステート更新処理
    void UpdateReviveState(float elapsedTime);

    //回避ステートへ遷移
    void TransitionDodgeState();

    //回避ステート更新処理
    void UpdateDodgeState(float elapsedTime);

    //歩きステートへ遷移
    void TransitionWalkState();

    //歩きステート更新処理
    void UpdateWalkState(float elapsedTime);

    //走りステートへ遷移
    void TransitionRunState();

    //走りステート更新処理
    void UpdateRunState(float elapsedTime);

    //ガードステートへ遷移
    void TransitionGuardState();

    //ガードステート更新処理
    void UpdateGuardState(float elapsedTime);

    //武器変更ステートへ遷移
    void TransitionChangeState();

    //武器変更ステート更新処理
    void UpdateChangeState(float elapsedTime);

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

    //ジャンプステートへ遷移
    void TransitionJump_GreatState();

    //ジャンプステート更新処理
    void UpdateJump_GreatState(float elapsedTime);

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

    //回避ステートへ遷移
    void TransitionDodge_GreatState();

    //回避ステート更新処理
    void UpdateDodge_GreatState(float elapsedTime);

    //ガードステートへ遷移
    void TransitionGuard_GreatState();

    //ガードステート更新処理
    void UpdateGuard_GreatState(float elapsedTime);

    //武器変更ステート遷移
    void TransitionChange_GreatState();

    //武器変更ステート更新処理
    void UpdateChange_GreatState(float elapsedTime);

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

    //ジャンプステートへ遷移
    void TransitionJump_DualState();

    //ジャンプステート更新処理
    void UpdateJump_DualState(float elapsedTime);

    //着地ステートへ遷移
    void TransitionLand_DualState();

    //着地ステート更新処理
    void UpdateLand_DualState(float elapsedTIme);

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

    //ジャンプステートへ遷移
    void TransitionJump_GunState();

    //ジャンプステート更新処理
    void UpdateJump_GunState(float elapsedTime);

    //着地ステートへ遷移
    void TransitionLand_GunState();

    //着地ステート更新処理
    void UpdateLand_GunState(float elapsedTIme);

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

    

private:
    //ステート
    enum class State
    {
        //武器未装着
        Idle,        //待機ステート
        Move,        //移動ステート
        Jump,        //ジャンプステート
        Land,        //着地ステート
        Attack,      //攻撃ステート
        Damage,      //ダメージステート
        Death,       //死亡ステート
        Revive,      //復活ステート
        Dodge,       //回避ステート
        Walk,        //歩きステート
        Run,         //走りステート
        Guard,       //ガードステート
        Change,      //武器変更ステート

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
    };

private:
    //アニメーション
    enum Animation
    {
        Anim_Attack,            //攻撃
        Anim_Death,             //死亡
        Anim_Falling,           //落ちる
        Anim_GetHit1,           //ヒット１
        Anim_GetHit2,           //ヒット２
        Anim_Idle,              //待機
        Anim_Jump,              //ジャンプ
        Anim_Jump_Flip,         //
        Anim_Landing,           //
        Anim_Revive,            //起き上がり
        Anim_Running,           //走る
        Anim_Walking            //歩く
    };

    //双剣アニメーション
    enum DualSwordAnimation
    {
        Anim_Dual_Idle, //待機
        Anim_Dual_Dash, //ダッシュ
    };

    //大剣アニメーション
    enum GreatSwordAnimation
    {
        Anim_Great_Idle,    //待機
        Anim_Great_Dash,    //ダッシュ
    };

    //銃アニメーション
    enum GunAnimation
    {
        Anim_Gun_Idle,  //待機
        Anim_Gun_Dash,  //ダッシュ
    };

    //public:
    //    //ステート用enumクラス
    //    enum class Robot_State
    //    {
    //        Robot_Idle,           //待機
    //        Robot_Walk,           //歩き
    //        Robot_Run,            //走り
    //        Robot_Dodge,          //回避
    //        Robot_Jump,           //ジャンプ
    //        Robot_Land,           //着地
    //        Robot_Attack,         //攻撃
    //        Robot_Damage,         //ダメージ
    //        Robot_Dead,           //死亡
    //    };


protected:
    //着地したときに呼ばれる
    void OnLanding() override;

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
    State state = State::Idle;      //ステート
    float leftHandRadius = 0.4f;
    bool attackCollisionFlag = false;
    int statenumber = 0;
    int weapon = 0; //武器用変数
    float changetime = 0.0f;
    int attackcount = 0;
    int bullets = 60;   //残弾数
    float reloadtime = 0.0f;
    int totalattackcount = 0;

  
  

    //StateMachine実装用
    //Robot_State robotstate = Robot_State::Robot_Idle;   //初期ステート
    //StateMachineRobot* stateMachineRobot = nullptr;   //StateMachineの生成
};