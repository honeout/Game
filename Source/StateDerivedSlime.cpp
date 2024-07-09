#include "StateDerivedSlime.h"
#include "PlayerRobot.h"
#include "Mathf.h"

// Stateを基底クラスとして各種Stateクラスを用意する。
// 各種Enter関数の内容は各Transition○○State関数を参考に
// 各種Execute関数の内容は各Update○○State関数を参考に

// 徘徊ステートに入った時のメソッド
void WanderState_Slime::Enter()
{

    owner->SetRandomTargetPosition();
    owner->GetModel()->PlayAnimation(static_cast<int>(SlimeAnimation::RunFWD), true);
}

// 徘徊ステートで実行するメソッド
void WanderState_Slime::Execute(float elapsedTime)
{
    // 目的地点までのXZ平面での距離判定
    DirectX::XMFLOAT3 position = owner->GetPosition();
    DirectX::XMFLOAT3 targetPosition = owner->GetTargetPosition();
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float distSq = vx * vx + vz * vz;

    // 目的地へ着いた
    float radius = owner->GetRadius();
    if (distSq < radius * radius)
    {
        // 待機ステートへ遷移
        // ChangeStateクラスでStateを切り替える
        owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::Slime_State::Slime_Idle));
    }
    // 目的地点へ移動
    owner->MoveToTarget(elapsedTime, 0.5f);
    // プレイヤー索敵
    if (owner->SearchPlayer())
    {
        // 見つかったら追跡ステートへ遷移
        // ChangeStateクラスでStateを切り替える
        owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::Slime_State::Slime_Pursuit));
    }

}

// 徘徊ステートから出ていくときのメソッド
void WanderState_Slime::Exit()
{

}

void IdleState_Slime::Enter()
{

}

void IdleState_Slime::Execute(float elapsedTime)
{
}

void IdleState_Slime::Exit()
{
}

void PursuitState_Slime::Enter()
{
}

void PursuitState_Slime::Execute(float elapsedTime)
{
}

void AttackState_Slime::Enter()
{
}

void AttackState_Slime::Execute(float elapsedTime)
{
}

void AttackState_Slime::Exit()
{
}
