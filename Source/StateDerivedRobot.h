//#pragma once
//#include "PlayerRobot.h"
//
//
//// �ҋ@�X�e�[�g�I�u�W�F�N�g
//class IdleState_Robot : public State_Robot
//{
//public:
//    // �R���X�g���N�^
//    IdleState_Robot(PlayerRobot* player) :State_Robot(player) {};
//    // �f�X�g���N�^
//    ~IdleState_Robot() {}
//    // �X�e�[�g�ɓ��������̃��\�b�h
//    void Enter()override;
//    // �X�e�[�g�Ŏ��s���郁�\�b�h
//    void Execute(float elapsedTime)override;
//    // �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
//    void Exit()override;
//};
//
////�����X�e�[�g�I�u�W�F�N�g
//class WalkState_Robot :public State_Robot
//{
//public:
//    //�R���X�g���N�^
//    WalkState_Robot(PlayerRobot* player) :State_Robot(player) {};
//
//    //�f�X�g���N�^
//    ~WalkState_Robot() {};
//
//    //�X�e�[�g�ɓ������Ƃ��̃��\�b�h
//    void Enter() override;
//
//    //�X�e�[�g�Ŏ��s���郁�\�b�h
//    void Execute(float elapsedTime) override;
//
//    //�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
//    void Exit() override;
//};
//
////����X�e�[�g�I�u�W�F�N�g
//class RunState_Robot :public State_Robot
//{
//public:
//    //�R���X�g���N�^
//    RunState_Robot(PlayerRobot* player) :State_Robot(player) {};
//
//    //�f�X�g���N�^
//    ~RunState_Robot() {};
//
//    //�X�e�[�g�ɓ������Ƃ��̃��\�b�h
//    void Enter() override;
//
//    //�X�e�[�g�Ŏ��s���郁�\�b�h
//    void Execute(float elapsedTime) override;
//
//    //�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
//    void Exit() override;
//};
//
////����X�e�[�g�I�u�W�F�N�g
//class DodgeState_Robot :public State_Robot
//{
//public:
//    //�R���X�g���N�^
//    DodgeState_Robot(PlayerRobot* player) :State_Robot(player) {};
//
//    //�f�X�g���N�^
//    ~DodgeState_Robot() {};
//
//    //�X�e�[�g�ɓ������Ƃ��̃��\�b�h
//    void Enter() override;
//
//    //�X�e�[�g�Ŏ��s���郁�\�b�h
//    void Execute(float elapsedTime) override;
//
//    //�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
//    void Exit() override;
//};
//
////�W�����v�X�e�[�g�I�u�W�F�N�g
//class JumpState_Robot : public State_Robot
//{
//public:
//    //�R���X�g���N�^
//    JumpState_Robot(PlayerRobot* player) : State_Robot(player) {};
//
//    //�f�X�g���N�^
//    ~JumpState_Robot() {};
//
//    //�X�e�[�g�ɓ������Ƃ��̃��\�b�h
//    void Enter() override;
//
//    //�X�e�[�g�Ŏ��s���郁�\�b�h
//    void Execute(float elpasedTime)override;
//
//    //�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
//    void Exit()override;
//};
//
////���n�X�e�[�g�I�u�W�F�N�g
//class LandState_Robot : public State_Robot
//{
//public:
//    //�R���X�g���N�^
//    LandState_Robot(PlayerRobot* player) :State_Robot(player) {};
//
//    //�f�X�g���N�^
//    ~LandState_Robot() {};
//
//    //�X�e�[�g�ɓ������Ƃ��̃��\�b�h
//    void Enter()override;
//
//    //�X�e�[�g�Ŏ��s���郁�\�b�h
//    void Execute(float elapsedTime) override;
//
//    //�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
//    void Exit()override;
//};
////�U���X�e�[�g�I�u�W�F�N�g
//class AttackState_Robot : public State_Robot
//{
//public:
//    //�R���X�g���N�^
//    AttackState_Robot(PlayerRobot* player) : State_Robot(player) {};
//
//    //�f�X�g���N�^
//    ~AttackState_Robot() {}
//
//    //�X�e�[�g�ɓ������Ƃ��̃��\�b�h
//    void Enter() override;
//
//    //�X�e�[�g�ɓ������Ƃ��̃��\�b�h
//    void Execute(float elapsedTime)override;
//
//    //�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
//    void Exit() override;
//};
//
//// �_���[�W�X�e�[�g�I�u�W�F�N�g
//class DamageState_Robot : public State_Robot
//{
//public:
//    // �R���X�g���N�^
//    DamageState_Robot(PlayerRobot* player) :State_Robot(player) {};
//    // �f�X�g���N�^
//    ~DamageState_Robot() {}
//    // �X�e�[�g�ɓ��������̃��\�b�h
//    void Enter()override;
//    // �X�e�[�g�Ŏ��s���郁�\�b�h
//    void Execute(float elapsedTime)override;
//    // �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
//    void Exit()override;
//};
//
////���S�X�e�[�g�I�u�W�F�N�g
//class DeadState_Robot : public State_Robot
//{
//public:
//    //�R���X�g���N�^
//    DeadState_Robot(PlayerRobot* player) :State_Robot(player) {};
//
//    //�f�X�g���N�^
//    ~DeadState_Robot() {};
//
//    //�X�e�[�g�ɓ������Ƃ��̃��\�b�h
//    void Enter() override;
//
//    //�X�e�[�g�Ŏ��s���郁�\�b�h
//    void Execute(float elapsedTime) override;
//
//    //�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
//    void Exit()override;
//};
