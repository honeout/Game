#pragma once
#include <vector>
class EnemySlime;

class State_Slime
{
public:
    // コンストラクタ
    State_Slime(EnemySlime* enemy) :owner(enemy) {}
    virtual ~State_Slime() {}
    // 全て継承先で実装させる必要があるため純粋仮想関数で実装
    // ステートに入った時のメソッド
    virtual void Enter() = 0;
    // ステートで実行するメソッド
    virtual void Execute(float elapsedTime) = 0;
    // ステートから出ていくときのメソッド
    virtual void Exit() = 0;
protected:
    EnemySlime* owner;
};