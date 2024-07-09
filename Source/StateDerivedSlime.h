#pragma once
#include "EnemySlime.h"


// 徘徊ステートオブジェクト
class WanderState_Slime : public State_Slime
{
public:
    // コンストラクタ
    WanderState_Slime(EnemySlime* enemy) :State_Slime(enemy) {};
    // デストラクタ
    ~WanderState_Slime() {}
    // ステートに入った時のメソッド
    void Enter()override;
    // ステートで実行するメソッド
    void Execute(float elapsedTime)override;
    // ステートから出ていくときのメソッド
    void Exit()override;
};

// 待機ステートオブジェクト
class IdleState_Slime : public State_Slime
{
public:
    // コンストラクタ
    IdleState_Slime(EnemySlime* enemy) :State_Slime(enemy) {};
    // デストラクタ
    ~IdleState_Slime() {}
    // ステートに入った時のメソッド
    void Enter()override;
    // ステートで実行するメソッド
    void Execute(float elapsedTime)override;
    // ステートから出ていくときのメソッド
    void Exit()override;
};

// 追跡ステートオブジェクト
class PursuitState_Slime : public State_Slime
{
public:
    // コンストラクタ
    PursuitState_Slime(EnemySlime* enemy) :State_Slime(enemy) {};
    // デストラクタ
    ~PursuitState_Slime() {}
    // ステートに入った時のメソッド
    void Enter()override;
    // ステートで実行するメソッド
    void Execute(float elapsedTime)override;
    // ステートから出ていくときのメソッド
    void Exit()override;
};

// 攻撃ステートオブジェクト
class AttackState_Slime : public State_Slime
{
public:
    // コンストラクタ
    AttackState_Slime(EnemySlime* enemy) :State_Slime(enemy) {};
    // デストラクタ
    ~AttackState_Slime() {}
    // ステートに入った時のメソッド
    void Enter()override;
    // ステートで実行するメソッド
    void Execute(float elapsedTime)override;
    // ステートから出ていくときのメソッド
    void Exit()override;
};