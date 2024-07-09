#include "EnemySkillGunRock.h"

//unit20

static EnemySkillGunRock* instance = nullptr;

//�C���X�^���X�擾
EnemySkillGunRock& EnemySkillGunRock::Instance()
{
    return *instance;

}


//�R���X�g���N�^
EnemySkillGunRock::EnemySkillGunRock(ProjectileManager* manager, const char* nodeName, const char* name) : Projectile(manager)
{
    //�C���X�^���X�|�C���^�ݒ�
    instance = this;

    //�����f��
    //model = new Model("Data/Model/Sword/Sword.mdl");
    model = new Model(name);

    //�\���T�C�Y�𒲐�
    scale.x = scale.y = scale.z = 0.3f;
    //trangitionEffect = new Effect("Data/Effect/fire.efk");
    //trangitionEffect = new Effect("Data/Effect/lightning strick boss.efk");
    trangitionEffect = new Effect(nodeName);
    trangitionEffect->Play(position);
}

//�f�X�g���N�^
EnemySkillGunRock::~EnemySkillGunRock()
{
    delete model;
    delete trangitionEffect;
}

//�X�V����
void EnemySkillGunRock::Update(float elapsedTime)
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
    //position.y += direction.y * speed;
    //position.z += direction.z * speed;
    //trangitionEffect->SetPostion(trangitionEffect->GetHandle(), position);
    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //���f���s��X�V
    model->UpdateTransform(transform);
}

void EnemySkillGunRock::Render(RenderContext& rc, ModelShader* shader)
{
    shader->Draw(rc, model);
}

void EnemySkillGunRock::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
    this->direction = direction;
    this->position = position;
}