#include "StateDerivedSlime.h"
#include "PlayerRobot.h"
#include "Mathf.h"

// State�����N���X�Ƃ��Ċe��State�N���X��p�ӂ���B
// �e��Enter�֐��̓��e�͊eTransition����State�֐����Q�l��
// �e��Execute�֐��̓��e�͊eUpdate����State�֐����Q�l��

// �p�j�X�e�[�g�ɓ��������̃��\�b�h
void WanderState_Slime::Enter()
{

    owner->SetRandomTargetPosition();
    owner->GetModel()->PlayAnimation(static_cast<int>(SlimeAnimation::RunFWD), true);
}

// �p�j�X�e�[�g�Ŏ��s���郁�\�b�h
void WanderState_Slime::Execute(float elapsedTime)
{
    // �ړI�n�_�܂ł�XZ���ʂł̋�������
    DirectX::XMFLOAT3 position = owner->GetPosition();
    DirectX::XMFLOAT3 targetPosition = owner->GetTargetPosition();
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float distSq = vx * vx + vz * vz;

    // �ړI�n�֒�����
    float radius = owner->GetRadius();
    if (distSq < radius * radius)
    {
        // �ҋ@�X�e�[�g�֑J��
        // ChangeState�N���X��State��؂�ւ���
        owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::Slime_State::Slime_Idle));
    }
    // �ړI�n�_�ֈړ�
    owner->MoveToTarget(elapsedTime, 0.5f);
    // �v���C���[���G
    if (owner->SearchPlayer())
    {
        // ����������ǐՃX�e�[�g�֑J��
        // ChangeState�N���X��State��؂�ւ���
        owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::Slime_State::Slime_Pursuit));
    }

}

// �p�j�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void WanderState_Slime::Exit()
{

}

void IdleState_Slime::Enter()
{

}

void IdleState_Slime::Execute(float elapsedTime)
{
}

void IdleState_Slime::Exit()
{
}

void PursuitState_Slime::Enter()
{
}

void PursuitState_Slime::Execute(float elapsedTime)
{
}

void AttackState_Slime::Enter()
{
}

void AttackState_Slime::Execute(float elapsedTime)
{
}

void AttackState_Slime::Exit()
{
}
