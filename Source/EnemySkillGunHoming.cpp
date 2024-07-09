#include "EnemySkillGunHoming.h"
#include "EnemyManager.h"

EnemySkillGunHoming::EnemySkillGunHoming(ProjectileManager* manager, const char* nodeName, const char* name) : Projectile(manager)
{
    //model = new Model("Data/Model/Sword/Sword.mdl");

    //�u���b�N�z�[���i�ǔ��e�j
    //model = new Model("Data/Model/Sword/Blackhole.mdl");
    model = new Model(name);

    trangitionEffect = new Effect(nodeName);
    trangitionEffect->Play(position);

    scale.x = scale.y = scale.z = 0.1f;
}


EnemySkillGunHoming::~EnemySkillGunHoming()
{
    delete model;
    delete trangitionEffect;
}

void EnemySkillGunHoming::Update(float elaspedTime)
{
    //trangitionEffect->SetPostion(trangitionEffect->GetHandle(), position);
    //trangitionEffect->SetRotation(trangitionEffect->GetHandle(), angle);

    //��������
    lifetimer -= elaspedTime;

    if (lifetimer <= 0.0f)
    {
        Destroy();
    }

    //�ړ�
    {
        float moveSpeed = this->moveSpeed * elaspedTime;
        position.x += direction.x * moveSpeed;
        position.y += direction.y * moveSpeed;
        position.z += direction.z * moveSpeed;

    }




    //��]
    {
        float turnSpeed = this->turnSpeed * elaspedTime;
        //�^�[�Q�b�g�܂ł̃x�N�g�����Z�o
        DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);
        DirectX::XMVECTOR Target = DirectX::XMLoadFloat3(&target);
        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Target, Position);

        //�[���x�N�g�����łȂ��Ȃ��]����
        DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
        float lengthSq;
        DirectX::XMStoreFloat(&lengthSq, LengthSq);
        // �z�[�~���O��ԂȂ�ǂ��I������������B
        //if (lengthSq > 0.00001f)
        if (!homingout)
        if (lengthSq > 0.1f)
        {
            //�^�[�Q�b�g�܂ł̃x�N�g����P�ʃx�N�g����
            Vec = DirectX::XMVector3Normalize(Vec);
            //�����Ă�������x�N�g�����Z�o
            DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);
            //�O�������x�N�g���ƃ^�[�Q�b�g�܂ł̃x�N�g���̓��ρi�p�x�j���Z�o
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Direction, Vec);

            float dot;
            DirectX::XMStoreFloat(&dot, Dot);

            float dotmax = 1.0f;
            //2�̒P�ʃx�N�g���̊p�x���������ق�
            //1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
            float rot = dotmax - dot;
            if (rot > turnSpeed) rot = turnSpeed;

            //��]�p�x������Ȃ��]����������
            if (fabsf(rot) > 0.0001f)
            {
                //��]�����Z�o
                DirectX::XMVECTOR Axis = DirectX::XMVector3Cross(Direction, Vec);
                Axis = DirectX::XMVector3Normalize(Axis);
                //��]���Ɖ�]�ʂ����]�s����Z�o
                DirectX::XMMATRIX Rotation = DirectX::XMMatrixRotationAxis(Axis, rot);
                //���݂̍s�����]������(1)
                DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);
                Transform = DirectX::XMMatrixMultiply(Transform, Rotation);
                //��]��̑O�����������o���A�P�ʃx�N�g��������
                Direction = DirectX::XMVector3Normalize(Transform.r[2]);
                DirectX::XMStoreFloat3(&direction, Direction);
                //���݂̍s�����]������(2)
            //Direction = DirectX::XMVector3Transform(Direction, Rotation);
            //Direction = DirectX::XMVector3Normalize(Direction);
            //DirectX::XMStoreFloat3(&direction, Direction);
            }


        }
        else
        {
            homingout = true;
        }
    }



    UpdateTransform();
    model->UpdateTransform(transform);

}

void EnemySkillGunHoming::Render(RenderContext& rc, ModelShader* shader)
{
    shader->Draw(rc, model);
}

void EnemySkillGunHoming::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& target , const DirectX::XMFLOAT3& angle)
{
    this->direction = direction;
    this->position = position;
    this->target = target;
    this->angle = angle;
    UpdateTransform();
}
