#pragma once

#include "Graphics/Model.h"
#include "Player.h"
#include "ProjectileManager.h"
#include "StateMachineRobot.h"
#include "Weapon.h"

class PlayerMaster : public Player
{
public:
    PlayerMaster();
    ~PlayerMaster() override;

    //更新処理
    void Update(float elapsedTime) override;

    //描画処理
    void Render(RenderContext& rc, ModelShader* shader) override;

    //デバッグ用GUI描画
    void DrawDebugGUI();

    //デバッグプリミティブ描画
    void DrawDebugPrimitive();


    //モデル取得
    Model* GetModel() { return model; }

    //スティック入力値から移動ベクトルを取得
    DirectX::XMFLOAT3 GetMoveVec() const;


private:
    //双剣 MasterBlades
    //待機ステートへ遷移
    void TransitionIdle_MasterState();

    //待機ステート更新処理
    void UpdateIdle_MasterState(float elapsedTime);

    //武器変更ステート遷移
    void TransitionChange_MasterState();

    //武器変更ステート更新処理
    void UpdateChange_MasterState(float elapsedTime);


private:
    //ステート
    enum class State
    {
        //武器なし　（武器変更用）　Master
        Idle_Master, //待機
        Change_Master, //武器変更
    };

private:
    //アニメーション
    enum MasterSwordAnimation
    {
        Anim_Master_Idle, //待機
        Anim_Master_Dash, //ダッシュ
        Anim_Master_Brake,
        Anim_Master_Damaged,
        Anim_Master_Switch,
        Anim_Master_Attack_1,   //攻撃
        Anim_Master_Attack_2,
        Anim_Master_Attack_3,
        Anim_Master_Dodge,    //回避
        Anim_Master_Change,   //武器変更
        Anim_Master_GetHit,   //ヒット
        Anim_Master_Death,    //死亡
        Anim_Master_Jump,     //ジャンプ
        Anim_Master_Falling,  //落ちている
        Anim_Master_Landing,  //着地
    };


private:
    Model* model = nullptr;
    float moveSpeed = 5.0f;
    float turnSpeed = DirectX::XMConvertToRadians(720);
    float jumpSpeed = 20.0f;
    int jumpCount = 0;      //何回飛んだか
    int jumpLimit = 1;      //ジャンプ回数制限
    ProjectileManager projectileManager;
    State state = State::Idle_Master;      //ステート
    float MasterRadius = 0.8f;
    bool attackCollisionFlag = false;
    int statenumber = 0;
    int weapon = 0; //武器用変数
    float changetime = 0.0f;

    Weapon* weaponModel = nullptr;

    
};