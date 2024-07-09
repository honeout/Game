#pragma once
#include "EnemySlime.h"


// �p�j�X�e�[�g�I�u�W�F�N�g
class WanderState_Slime : public State_Slime
{
public:
    // �R���X�g���N�^
    WanderState_Slime(EnemySlime* enemy) :State_Slime(enemy) {};
    // �f�X�g���N�^
    ~WanderState_Slime() {}
    // �X�e�[�g�ɓ��������̃��\�b�h
    void Enter()override;
    // �X�e�[�g�Ŏ��s���郁�\�b�h
    void Execute(float elapsedTime)override;
    // �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    void Exit()override;
};

// �ҋ@�X�e�[�g�I�u�W�F�N�g
class IdleState_Slime : public State_Slime
{
public:
    // �R���X�g���N�^
    IdleState_Slime(EnemySlime* enemy) :State_Slime(enemy) {};
    // �f�X�g���N�^
    ~IdleState_Slime() {}
    // �X�e�[�g�ɓ��������̃��\�b�h
    void Enter()override;
    // �X�e�[�g�Ŏ��s���郁�\�b�h
    void Execute(float elapsedTime)override;
    // �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    void Exit()override;
};

// �ǐՃX�e�[�g�I�u�W�F�N�g
class PursuitState_Slime : public State_Slime
{
public:
    // �R���X�g���N�^
    PursuitState_Slime(EnemySlime* enemy) :State_Slime(enemy) {};
    // �f�X�g���N�^
    ~PursuitState_Slime() {}
    // �X�e�[�g�ɓ��������̃��\�b�h
    void Enter()override;
    // �X�e�[�g�Ŏ��s���郁�\�b�h
    void Execute(float elapsedTime)override;
    // �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    void Exit()override;
};

// �U���X�e�[�g�I�u�W�F�N�g
class AttackState_Slime : public State_Slime
{
public:
    // �R���X�g���N�^
    AttackState_Slime(EnemySlime* enemy) :State_Slime(enemy) {};
    // �f�X�g���N�^
    ~AttackState_Slime() {}
    // �X�e�[�g�ɓ��������̃��\�b�h
    void Enter()override;
    // �X�e�[�g�Ŏ��s���郁�\�b�h
    void Execute(float elapsedTime)override;
    // �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    void Exit()override;
};