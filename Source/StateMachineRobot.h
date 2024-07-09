#pragma once
#include <vector>
#include "StateBaseRobot.h"

class StateMachineRobot
{
public:
    // コンストラクタ
    StateMachineRobot() {}
    // デストラクタ
    ~StateMachineRobot();
    // 更新処理
    void Update(float elapsedTime);
    // ステートセット
    void SetState(int setstate);
    // ステート変更
    void ChangeState(int newState);
    // ステート登録
    void RegisterState(State_Robot* state);
    // 現在のステート番号取得
    int GetStateIndex();
private:
    // 現在のステート
    State_Robot* currentState = nullptr;
    // 各ステートを保持する配列
    std::vector<State_Robot*> statePool;
};