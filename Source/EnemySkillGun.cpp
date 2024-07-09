#include "EnemySkillGun.h"

//unit20

static EnemySkillGun* instance = nullptr;

//�C���X�^���X�擾
EnemySkillGun& EnemySkillGun::Instance()
{
    return *instance;
}

//�R���X�g���N�^
EnemySkillGun::EnemySkillGun(ProjectileManager* manager, const char* nodeName, const char* name) : Projectile(manager)
{
    //�C���X�^���X�|�C���^�ݒ�
    instance = this;

    //�����f��
    model = new Model(name);

    radius = 1.5f;

    //�\���T�C�Y�𒲐�
    scale.x = 0.07f;
    scale.y = 0.07f;
    scale.z = 0.04f;
    //trangitionEffect = new Effect("Data/Effect/fire.efk");
    //trangitionEffect = new Effect("Data/Effect/lightning attack.efk");
    //trangitionEffect = new Effect(nodeName);
    //trangitionEffect->Play(position);
}

//�f�X�g���N�^
EnemySkillGun::~EnemySkillGun()
{
    //trangitionEffect->Stop(trangitionEffect->GetHandle());
   // delete trangitionEffect;
    delete model;
}

//�X�V����
void EnemySkillGun::Update(float elapsedTime)
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

    // 
    //trangitionEffect->SetPostion(trangitionEffect->GetHandle(), this->position);
    //trangitionEffect->SetRotation(trangitionEffect->GetHandle(), this->angledis);
    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //���f���s��X�V
    model->UpdateTransform(transform);
}

void EnemySkillGun::Render(RenderContext& rc, ModelShader* shader)
{
    shader->Draw(rc, model);
}

void EnemySkillGun::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position,const DirectX::XMFLOAT3& angle,const float& speed)
{
    this->direction = direction;
    this->position = position;
    this->angledis = angle;

    // ���x�̂��ꂼ��̒���
    this->speed = speed;
}

