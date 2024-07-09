//#pragma once
//#include "PlayerRobot.h"
//
//
//// 待機ステートオブジェクト
//class IdleState_Robot : public State_Robot
//{
//public:
//    // コンストラクタ
//    IdleState_Robot(PlayerRobot* player) :State_Robot(player) {};
//    // デストラクタ
//    ~IdleState_Robot() {}
//    // ステートに入った時のメソッド
//    void Enter()override;
//    // ステートで実行するメソッド
//    void Execute(float elapsedTime)override;
//    // ステートから出ていくときのメソッド
//    void Exit()override;
//};
//
////歩きステートオブジェクト
//class WalkState_Robot :public State_Robot
//{
//public:
//    //コンストラクタ
//    WalkState_Robot(PlayerRobot* player) :State_Robot(player) {};
//
//    //デストラクタ
//    ~WalkState_Robot() {};
//
//    //ステートに入ったときのメソッド
//    void Enter() override;
//
//    //ステートで実行するメソッド
//    void Execute(float elapsedTime) override;
//
//    //ステートから出ていくときのメソッド
//    void Exit() override;
//};
//
////走りステートオブジェクト
//class RunState_Robot :public State_Robot
//{
//public:
//    //コンストラクタ
//    RunState_Robot(PlayerRobot* player) :State_Robot(player) {};
//
//    //デストラクタ
//    ~RunState_Robot() {};
//
//    //ステートに入ったときのメソッド
//    void Enter() override;
//
//    //ステートで実行するメソッド
//    void Execute(float elapsedTime) override;
//
//    //ステートから出ていくときのメソッド
//    void Exit() override;
//};
//
////回避ステートオブジェクト
//class DodgeState_Robot :public State_Robot
//{
//public:
//    //コンストラクタ
//    DodgeState_Robot(PlayerRobot* player) :State_Robot(player) {};
//
//    //デストラクタ
//    ~DodgeState_Robot() {};
//
//    //ステートに入ったときのメソッド
//    void Enter() override;
//
//    //ステートで実行するメソッド
//    void Execute(float elapsedTime) override;
//
//    //ステートから出ていくときのメソッド
//    void Exit() override;
//};
//
////ジャンプステートオブジェクト
//class JumpState_Robot : public State_Robot
//{
//public:
//    //コンストラクタ
//    JumpState_Robot(PlayerRobot* player) : State_Robot(player) {};
//
//    //デストラクタ
//    ~JumpState_Robot() {};
//
//    //ステートに入ったときのメソッド
//    void Enter() override;
//
//    //ステートで実行するメソッド
//    void Execute(float elpasedTime)override;
//
//    //ステートから出ていくときのメソッド
//    void Exit()override;
//};
//
////着地ステートオブジェクト
//class LandState_Robot : public State_Robot
//{
//public:
//    //コンストラクタ
//    LandState_Robot(PlayerRobot* player) :State_Robot(player) {};
//
//    //デストラクタ
//    ~LandState_Robot() {};
//
//    //ステートに入ったときのメソッド
//    void Enter()override;
//
//    //ステートで実行するメソッド
//    void Execute(float elapsedTime) override;
//
//    //ステートから出ていくときのメソッド
//    void Exit()override;
//};
////攻撃ステートオブジェクト
//class AttackState_Robot : public State_Robot
//{
//public:
//    //コンストラクタ
//    AttackState_Robot(PlayerRobot* player) : State_Robot(player) {};
//
//    //デストラクタ
//    ~AttackState_Robot() {}
//
//    //ステートに入ったときのメソッド
//    void Enter() override;
//
//    //ステートに入ったときのメソッド
//    void Execute(float elapsedTime)override;
//
//    //ステートから出ていくときのメソッド
//    void Exit() override;
//};
//
//// ダメージステートオブジェクト
//class DamageState_Robot : public State_Robot
//{
//public:
//    // コンストラクタ
//    DamageState_Robot(PlayerRobot* player) :State_Robot(player) {};
//    // デストラクタ
//    ~DamageState_Robot() {}
//    // ステートに入った時のメソッド
//    void Enter()override;
//    // ステートで実行するメソッド
//    void Execute(float elapsedTime)override;
//    // ステートから出ていくときのメソッド
//    void Exit()override;
//};
//
////死亡ステートオブジェクト
//class DeadState_Robot : public State_Robot
//{
//public:
//    //コンストラクタ
//    DeadState_Robot(PlayerRobot* player) :State_Robot(player) {};
//
//    //デストラクタ
//    ~DeadState_Robot() {};
//
//    //ステートに入ったときのメソッド
//    void Enter() override;
//
//    //ステートで実行するメソッド
//    void Execute(float elapsedTime) override;
//
//    //ステートから出ていくときのメソッド
//    void Exit()override;
//};
