#include "EnemySkillGunArea.h"

//unit20

static EnemySkillGunArea* instance = nullptr;

//�C���X�^���X�擾
EnemySkillGunArea& EnemySkillGunArea::Instance()
{
    return *instance;

}

//�R���X�g���N�^
EnemySkillGunArea::EnemySkillGunArea(ProjectileManager* manager,const char* nodeName, const char* name) : Projectile(manager)
{
    //�C���X�^���X�|�C���^�ݒ�
    instance = this;

    //�����f��

    model = new Model(name);

    //�\���T�C�Y�𒲐�
    scale.x = scale.y = scale.z = 0.1f;

    trangitionEffect = new Effect(nodeName);
    trangitionEffect->Play(position);
}

//�f�X�g���N�^
EnemySkillGunArea::~EnemySkillGunArea()
{
    delete model;
    delete trangitionEffect;
}

//�X�V����
void EnemySkillGunArea::Update(float elapsedTime)
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
    //position.x += direction.x * speed;
    position.y += direction.y * speed;
    //position.z += direction.z * speed;

    //TODO �X�L�����̕������Q�l�ɂ���
    //�G�t�F�N�g�𓮂����̂ɕK�v
    //trangitionEffect->SetPostion(trangitionEffect->GetHandle(), position);
    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //���f���s��X�V
    model->UpdateTransform(transform);
}

void EnemySkillGunArea::Render(RenderContext& rc, ModelShader* shader)
{
    shader->Draw(rc, model);
}

void EnemySkillGunArea::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, const float& speed)
{
    this->direction = direction;
    this->position = position;

    this->speed = speed;
}