#include "StateMachineSlime.h"

StateMachineSlime::~StateMachineSlime()
{
    for (State_Slime* state : statePool)
    {
        delete state;
    }
    statePool.clear();
}
// �X�V����
void StateMachineSlime::Update(float elapsedTime)
{
    currentState->Execute(elapsedTime);
}
// �X�e�[�g�Z�b�g
void StateMachineSlime::SetState(int setState)
{
    currentState = statePool.at(setState);
    currentState->Enter();
}
// �X�e�[�g�ύX
void StateMachineSlime::ChangeState(int newState)
{
    // �X�e�[�g�̐؂�ւ��B
    // ���݂̃X�e�[�g��Exit�֐������s
    //�V�����X�e�[�g���Z�b�g�A�V�����X�e�[�g��Enter�֐����Ăяo���B
    currentState->Exit();
    //SetState(newState);
    currentState = statePool.at(newState);
    currentState->Enter();

}
// �X�e�[�g�o�^
void StateMachineSlime::RegisterState(State_Slime* state)
{
    // �e�X�e�[�g�o�^
    statePool.emplace_back(state);
}
// �X�e�[�g�ԍ��擾
int StateMachineSlime::GetStateIndex()
{
    int i = 0;
    for (State_Slime* state : statePool)
    {
        if (state == currentState)
        {
            // i�ԍ��ڂ̃X�e�[�g�����^�[��
            return i;
        }
        ++i;
    }
    // �X�e�[�g��������Ȃ�������
    return -1;
}