#pragma once
#include <vector>
class PlayerRobot;

class State_Robot
{
public:
    // �R���X�g���N�^
    State_Robot(PlayerRobot* player) :owner(player) {}
    virtual ~State_Robot() {}
    // �S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
    // �X�e�[�g�ɓ��������̃��\�b�h
    virtual void Enter() = 0;
    // �X�e�[�g�Ŏ��s���郁�\�b�h
    virtual void Execute(float elapsedTime) = 0;
    // �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
    virtual void Exit() = 0;
protected:
    PlayerRobot* owner;
};