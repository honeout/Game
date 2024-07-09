#pragma once
#include <vector>
#include "StateBaseRobot.h"

class StateMachineRobot
{
public:
    // �R���X�g���N�^
    StateMachineRobot() {}
    // �f�X�g���N�^
    ~StateMachineRobot();
    // �X�V����
    void Update(float elapsedTime);
    // �X�e�[�g�Z�b�g
    void SetState(int setstate);
    // �X�e�[�g�ύX
    void ChangeState(int newState);
    // �X�e�[�g�o�^
    void RegisterState(State_Robot* state);
    // ���݂̃X�e�[�g�ԍ��擾
    int GetStateIndex();
private:
    // ���݂̃X�e�[�g
    State_Robot* currentState = nullptr;
    // �e�X�e�[�g��ێ�����z��
    std::vector<State_Robot*> statePool;
};