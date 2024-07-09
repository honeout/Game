#include "ProjectileStraite.h"

//�R���X�g���N�^
ProjectileStraite::ProjectileStraite(ProjectileManager* manager) : Projectile(manager)
{
    //�����f��
    //model = new Model("Data/Model/SpikeBall/SpikeBall.mdl");
    model = new Model("Data/Model/Bullet/Bullet_02.mdl");

    //�����f��
    //model = new Model("Data/Model/Sword/Sword.mdl");

    //�\���T�C�Y�𒲐�
    scale.x = scale.y = scale.z = 0.05f;     //��
    //scale.x = scale.y = scale.z = 3.0f;   //��
}

//�f�X�g���N�^
ProjectileStraite::~ProjectileStraite()
{
    delete model;
}

//�X�V����
void ProjectileStraite::Update(float elapsedTime)
{
    //��������
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0.0f)
    {
        //�������폜
        Destroy();
    }

    //�ړ�
    float speed = this->speed * elapsedTime;
    position.x += direction.x * speed;
    position.y += direction.y * speed;
    position.z += direction.z * speed;

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //���f���s��X�V
    model->UpdateTransform(transform);
}

void ProjectileStraite::Render(RenderContext& rc, ModelShader* shader)
{
    shader->Draw(rc, model);
}

void ProjectileStraite::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
    this->direction = direction;
    this->position = position;
}