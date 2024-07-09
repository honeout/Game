//#include "StateDerivedRobot.h"
//#include "Mathf.h"
//
//// �e��Enter�֐��̓��e�͊eTransition����State�֐����Q�l��
//// �e��Execute�֐��̓��e�͊eUpdate����State�֐����Q�l��
//
////�ҋ@�X�e�[�g�̓������Ƃ��̃��\�b�h
//void IdleState_Robot::Enter()
//{
//    //�A�j���[�V�����Đ�
//    owner->GetModel()->PlayAnimation(static_cast<int>(RobotAnimation::Idle), true);
//}
//
////�ҋ@�X�e�[�g�Ŏ��s���郁�\�b�h
//void IdleState_Robot::Execute(float elapsedTime)
//{
//    //�������͏���
//    if (owner->InputWalk(elapsedTime))
//    {
//        //�����X�e�[�g�֑J��
//        owner->GetStateMachine()->ChangeState(static_cast<int>(PlayerRobot::Robot_State::Robot_Walk));
//    }
//
//    //������͏���
//    if (owner->InputRun(elapsedTime))
//    {
//        //����X�e�[�g�֑J��
//        owner->GetStateMachine()->ChangeState(static_cast<int>(PlayerRobot::Robot_State::Robot_Run));
//    }
//
//    //�W�����v���͏���
//    if (owner->InputJump())
//    {
//        //�W�����v�X�e�[�g�֑J��
//        owner->GetStateMachine()->ChangeState(static_cast<int>(PlayerRobot::Robot_State::Robot_Jump));
//    }
//
//    //�U�����͏���
//    if (owner->InputAttack())
//    {
//        //�U���X�e�[�g�֑J��
//        owner->GetStateMachine()->ChangeState(static_cast<int>(PlayerRobot::Robot_State::Robot_Attack));
//    }
//}
//
////�ҋ@�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
//void IdleState_Robot::Exit()
//{
//}
//
////�����X�e�[�g�֓������Ƃ��̃��\�b�h
//void WalkState_Robot::Enter()
//{
//    //�A�j���[�V�����Đ�
//    owner->GetModel()->PlayAnimation(static_cast<int>(RobotAnimation::Walking), true);
//}
//
////�����X�e�[�g���s���̃��\�b�h
//void WalkState_Robot::Execute(float elapsedTime)
//{
//    //�������͏������Ȃ���Αҋ@�X�e�[�g�֑J��
//    if (!owner->InputWalk(elapsedTime))
//    {
//        owner->GetStateMachine()->ChangeState(static_cast<int>(PlayerRobot::Robot_State::Robot_Idle));
//    }
//
//    //������͏���
//    if (owner->InputRun(elapsedTime))
//    {
//        //����X�e�[�g�֑J��
//        owner->GetStateMachine()->ChangeState(static_cast<int>(PlayerRobot::Robot_State::Robot_Run));
//    }
//
//    //�W�����v���͏���
//    if (owner->InputJump())
//    {
//        //�W�����v�X�e�[�g�֑J��
//        owner->GetStateMachine()->ChangeState(static_cast<int>(PlayerRobot::Robot_State::Robot_Jump));
//    }
//
//    //�U�����͏���
//    if (owner->InputAttack())
//    {
//        //�U���X�e�[�g�֑J��
//        owner->GetStateMachine()->ChangeState(static_cast<int>(PlayerRobot::Robot_State::Robot_Attack));
//    }
//}
//
////�����X�e�[�g���o��Ƃ��̃��\�b�h
//void WalkState_Robot::Exit()
//{
//}
//
////����X�e�[�g�ɓ���Ƃ��̃��\�b�h
//void RunState_Robot::Enter()
//{
//    //�A�j���[�V�����Đ�
//    owner->GetModel()->PlayAnimation(static_cast<int>(RobotAnimation::Running), true);
//}
//
////����X�e�[�g���s���̃��\�b�h
//void RunState_Robot::Execute(float elapsedTime)
//{
//    //�������͏������Ȃ���Αҋ@�X�e�[�g�֑J��
//    if (!owner->InputRun(elapsedTime))
//    {
//        owner->GetStateMachine()->ChangeState(static_cast<int>(PlayerRobot::Robot_State::Robot_Idle));
//    }
//
//    //�������͏���
//    if (owner->InputWalk(elapsedTime))
//    {
//        //�����X�e�[�g�֑J��
//        owner->GetStateMachine()->ChangeState(static_cast<int>(PlayerRobot::Robot_State::Robot_Walk));
//    }
//
//    //�W�����v���͏���
//    if (owner->InputJump())
//    {
//        //�W�����v�X�e�[�g�֑J��
//        owner->GetStateMachine()->ChangeState(static_cast<int>(PlayerRobot::Robot_State::Robot_Jump));
//    }
//
//    //�U�����͏���
//    if (owner->InputAttack())
//    {
//        //�U���X�e�[�g�֑J��
//        owner->GetStateMachine()->ChangeState(static_cast<int>(PlayerRobot::Robot_State::Robot_Attack));
//    }
//}
//
////����X�e�[�g���o��Ƃ��̃��\�b�h
//void RunState_Robot::Exit()
//{
//}
//
////�W�����v�X�e�[�g�ɓ���Ƃ��̃��\�b�h
//void JumpState_Robot::Enter()
//{
//}
//
////�W�����v�X�e�[�g���s���̃��\�b�h
//void JumpState_Robot::Execute(float elpasedTime)
//{
//}
//
////�W�����v�X�e�[�g���o��Ƃ��̃��\�b�h
//void JumpState_Robot::Exit()
//{
//}
//
////���n�X�e�[�g�ɓ���Ƃ��̃��\�b�h
//void LandState_Robot::Enter()
//{
//
//}
//
////���n�X�e�[�g���s���̃��\�b�h
//void LandState_Robot::Execute(float elapsedTime)
//{
//}
//
////���n�X�e�[�g���o��Ƃ��̃��\�b�h
//void LandState_Robot::Exit()
//{
//}
//
////����X�e�[�g�ɓ���Ƃ��̃��\�b�h
//void DodgeState_Robot::Enter()
//{
//}
//
////����X�e�[�g���s���̃��\�b�h
//void DodgeState_Robot::Execute(float elapsedTime)
//{
//}
//
////����X�e�[�g���o��Ƃ��̃��\�b�h
//void DodgeState_Robot::Exit()
//{
//}
//
////�U���X�e�[�g�ɓ���Ƃ��̃��\�b�h
//void AttackState_Robot::Enter()
//{
//}
//
////�U���X�e�[�g�����s���̃��\�b�h
//void AttackState_Robot::Execute(float elapsedTime)
//{
//}
//
////�U���X�e�[�g���o��Ƃ��̃��\�b�h
//void AttackState_Robot::Exit()
//{
//}
//
////�_���[�W�X�e�[�g�ɓ���Ƃ��̃��\�b�h
//void DamageState_Robot::Enter()
//{
//}
//
////�_���[�W�X�e�[�g���s���̃��\�b�h
//void DamageState_Robot::Execute(float elapsedTime)
//{
//}
//
////�_���[�W�X�e�[�g���o��Ƃ��̃��\�b�h
//void DamageState_Robot::Exit()
//{
//}
//
////���S�X�e�[�g�ɓ���Ƃ��̃��\�b�h
//void DeadState_Robot::Enter()
//{
//}
//
////���S�X�e�[�g���s���̃��\�b�h
//void DeadState_Robot::Execute(float elapsedTime)
//{
//}
//
////���S�X�e�[�g���o��Ƃ��̃��\�b�h
//void DeadState_Robot::Exit()
//{
//}
