#include "StateMachineRobot.h"

StateMachineRobot::~StateMachineRobot()
{
    for (State_Robot* state : statePool)
    {
        delete state;
    }
    statePool.clear();
}
// 更新処理
void StateMachineRobot::Update(float elapsedTime)
{
    currentState->Execute(elapsedTime);
}
// ステートセット
void StateMachineRobot::SetState(int setState)
{
    currentState = statePool.at(setState);
    currentState->Enter();
}
// ステート変更
void StateMachineRobot::ChangeState(int newState)
{
    // ステートの切り替え。
    // 現在のステートのExit関数を実行
    //新しいステートをセット、新しいステートのEnter関数を呼び出す。
    currentState->Exit();
    //SetState(newState);
    currentState = statePool.at(newState);
    currentState->Enter();

}
// ステート登録
void StateMachineRobot::RegisterState(State_Robot* state)
{
    // 親ステート登録
    statePool.emplace_back(state);
}
// ステート番号取得
int StateMachineRobot::GetStateIndex()
{
    int i = 0;
    for (State_Robot* state : statePool)
    {
        if (state == currentState)
        {
            // i番号目のステートをリターン
            return i;
        }
        ++i;
    }
    // ステートが見つからなかった時
    return -1;
}