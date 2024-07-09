#pragma once
#pragma once
#include <vector>
#include "StateBaseSlime.h"

class StateMachineSlime
{
public:
    // �R���X�g���N�^
    StateMachineSlime() {}
    // �f�X�g���N�^
    ~StateMachineSlime();
    // �X�V����
    void Update(float elapsedTime);
    // �X�e�[�g�Z�b�g
    void SetState(int setstate);
    // �X�e�[�g�ύX
    void ChangeState(int newState);
    // �X�e�[�g�o�^
    void RegisterState(State_Slime* state);
    // ���݂̃X�e�[�g�ԍ��擾
    int GetStateIndex();
private:
    // ���݂̃X�e�[�g
    State_Slime* currentState = nullptr;
    // �e�X�e�[�g��ێ�����z��
    std::vector<State_Slime*> statePool;
};