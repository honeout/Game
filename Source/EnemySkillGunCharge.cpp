#include "EnemySkillGunCharge.h"

//unit20

static EnemySkillGunCharge* instance = nullptr;

//�C���X�^���X�擾
EnemySkillGunCharge& EnemySkillGunCharge::Instance()
{
    return *instance;
}

//�R���X�g���N�^
EnemySkillGunCharge::EnemySkillGunCharge(ProjectileManager* manager, const char* nodeName, const char* name) : Projectile(manager)
{
    //�C���X�^���X�|�C���^�ݒ�
    instance = this;

    //�����f��
    model = new Model(name);

    radius = 2.0f;

    //�\���T�C�Y�𒲐�
    scale.x = 0.1f;
    scale.y = 0.1f;
    scale.z = 0.08f;

    hitEffect = new Effect(nodeName);
    hitEffect->Play(position);
}

//�f�X�g���N�^
EnemySkillGunCharge::~EnemySkillGunCharge()
{
    hitEffect->stop(hitEffect->GetHandle());
    delete hitEffect;
    delete model;
}

//�X�V����
void EnemySkillGunCharge::Update(float elapsedTime)
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


    hitEffect->SetPosition(hitEffect->GetHandle(), this->position);
    hitEffect->SetRotation(hitEffect->GetHandle(), this->angledis);

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //���f���s��X�V
    model->UpdateTransform(transform);
}

void EnemySkillGunCharge::Render(RenderContext& rc, ModelShader* shader)
{
    shader->Draw(rc, model);
}

void EnemySkillGunCharge::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& angle, const float& speed)
{
    this->direction = direction;
    this->position = position;
    this->angledis = angle;

    this->speed = speed;
}