//#include "StateDerivedRobot.h"
//#include "Mathf.h"
//
//// 各種Enter関数の内容は各Transition○○State関数を参考に
//// 各種Execute関数の内容は各Update○○State関数を参考に
//
////待機ステートの入ったときのメソッド
//void IdleState_Robot::Enter()
//{
//    //アニメーション再生
//    owner->GetModel()->PlayAnimation(static_cast<int>(RobotAnimation::Idle), true);
//}
//
////待機ステートで実行するメソッド
//void IdleState_Robot::Execute(float elapsedTime)
//{
//    //歩き入力処理
//    if (owner->InputWalk(elapsedTime))
//    {
//        //歩きステートへ遷移
//        owner->GetStateMachine()->ChangeState(static_cast<int>(PlayerRobot::Robot_State::Robot_Walk));
//    }
//
//    //走り入力処理
//    if (owner->InputRun(elapsedTime))
//    {
//        //走りステートへ遷移
//        owner->GetStateMachine()->ChangeState(static_cast<int>(PlayerRobot::Robot_State::Robot_Run));
//    }
//
//    //ジャンプ入力処理
//    if (owner->InputJump())
//    {
//        //ジャンプステートへ遷移
//        owner->GetStateMachine()->ChangeState(static_cast<int>(PlayerRobot::Robot_State::Robot_Jump));
//    }
//
//    //攻撃入力処理
//    if (owner->InputAttack())
//    {
//        //攻撃ステートへ遷移
//        owner->GetStateMachine()->ChangeState(static_cast<int>(PlayerRobot::Robot_State::Robot_Attack));
//    }
//}
//
////待機ステートから出ていくときのメソッド
//void IdleState_Robot::Exit()
//{
//}
//
////歩きステートへ入ったときのメソッド
//void WalkState_Robot::Enter()
//{
//    //アニメーション再生
//    owner->GetModel()->PlayAnimation(static_cast<int>(RobotAnimation::Walking), true);
//}
//
////歩きステート実行中のメソッド
//void WalkState_Robot::Execute(float elapsedTime)
//{
//    //歩き入力処理がなければ待機ステートへ遷移
//    if (!owner->InputWalk(elapsedTime))
//    {
//        owner->GetStateMachine()->ChangeState(static_cast<int>(PlayerRobot::Robot_State::Robot_Idle));
//    }
//
//    //走り入力処理
//    if (owner->InputRun(elapsedTime))
//    {
//        //走りステートへ遷移
//        owner->GetStateMachine()->ChangeState(static_cast<int>(PlayerRobot::Robot_State::Robot_Run));
//    }
//
//    //ジャンプ入力処理
//    if (owner->InputJump())
//    {
//        //ジャンプステートへ遷移
//        owner->GetStateMachine()->ChangeState(static_cast<int>(PlayerRobot::Robot_State::Robot_Jump));
//    }
//
//    //攻撃入力処理
//    if (owner->InputAttack())
//    {
//        //攻撃ステートへ遷移
//        owner->GetStateMachine()->ChangeState(static_cast<int>(PlayerRobot::Robot_State::Robot_Attack));
//    }
//}
//
////歩きステートを出るときのメソッド
//void WalkState_Robot::Exit()
//{
//}
//
////走りステートに入るときのメソッド
//void RunState_Robot::Enter()
//{
//    //アニメーション再生
//    owner->GetModel()->PlayAnimation(static_cast<int>(RobotAnimation::Running), true);
//}
//
////走りステート実行中のメソッド
//void RunState_Robot::Execute(float elapsedTime)
//{
//    //歩き入力処理がなければ待機ステートへ遷移
//    if (!owner->InputRun(elapsedTime))
//    {
//        owner->GetStateMachine()->ChangeState(static_cast<int>(PlayerRobot::Robot_State::Robot_Idle));
//    }
//
//    //歩き入力処理
//    if (owner->InputWalk(elapsedTime))
//    {
//        //歩きステートへ遷移
//        owner->GetStateMachine()->ChangeState(static_cast<int>(PlayerRobot::Robot_State::Robot_Walk));
//    }
//
//    //ジャンプ入力処理
//    if (owner->InputJump())
//    {
//        //ジャンプステートへ遷移
//        owner->GetStateMachine()->ChangeState(static_cast<int>(PlayerRobot::Robot_State::Robot_Jump));
//    }
//
//    //攻撃入力処理
//    if (owner->InputAttack())
//    {
//        //攻撃ステートへ遷移
//        owner->GetStateMachine()->ChangeState(static_cast<int>(PlayerRobot::Robot_State::Robot_Attack));
//    }
//}
//
////走りステートを出るときのメソッド
//void RunState_Robot::Exit()
//{
//}
//
////ジャンプステートに入るときのメソッド
//void JumpState_Robot::Enter()
//{
//}
//
////ジャンプステート実行中のメソッド
//void JumpState_Robot::Execute(float elpasedTime)
//{
//}
//
////ジャンプステートを出るときのメソッド
//void JumpState_Robot::Exit()
//{
//}
//
////着地ステートに入るときのメソッド
//void LandState_Robot::Enter()
//{
//
//}
//
////着地ステート実行中のメソッド
//void LandState_Robot::Execute(float elapsedTime)
//{
//}
//
////着地ステートを出るときのメソッド
//void LandState_Robot::Exit()
//{
//}
//
////回避ステートに入るときのメソッド
//void DodgeState_Robot::Enter()
//{
//}
//
////回避ステート実行中のメソッド
//void DodgeState_Robot::Execute(float elapsedTime)
//{
//}
//
////回避ステートを出るときのメソッド
//void DodgeState_Robot::Exit()
//{
//}
//
////攻撃ステートに入るときのメソッド
//void AttackState_Robot::Enter()
//{
//}
//
////攻撃ステートを実行中のメソッド
//void AttackState_Robot::Execute(float elapsedTime)
//{
//}
//
////攻撃ステートを出るときのメソッド
//void AttackState_Robot::Exit()
//{
//}
//
////ダメージステートに入るときのメソッド
//void DamageState_Robot::Enter()
//{
//}
//
////ダメージステート実行中のメソッド
//void DamageState_Robot::Execute(float elapsedTime)
//{
//}
//
////ダメージステートを出るときのメソッド
//void DamageState_Robot::Exit()
//{
//}
//
////死亡ステートに入るときのメソッド
//void DeadState_Robot::Enter()
//{
//}
//
////死亡ステート実行中のメソッド
//void DeadState_Robot::Execute(float elapsedTime)
//{
//}
//
////死亡ステートを出るときのメソッド
//void DeadState_Robot::Exit()
//{
//}
