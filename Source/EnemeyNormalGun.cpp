#include "EnemeyNormalGun.h"

//unit20

static EnemyNormalGun* instance = nullptr;

//�C���X�^���X�擾
EnemyNormalGun& EnemyNormalGun::Instance()
{
    return *instance;
}

//�R���X�g���N�^
EnemyNormalGun::EnemyNormalGun(ProjectileManager* manager, const char* nodeName, const char* name) : Projectile(manager)
{
    //�C���X�^���X�|�C���^�ݒ�
    instance = this;

    //�����f��
    model = new Model(name);
  //model = new Model("Data/Model/SpikeBall/SpikeBall.mdl");

    //�\���T�C�Y�𒲐�
    scale.x = scale.y = scale.z = 1.0f;
    //trangitionEffect = new Effect("Data/Effect/fire.efk");
    //trangitionEffect = new Effect("Data/Effect/lightning attack.efk");
    //trangitionEffect = new Effect(nodeName);
    //trangitionEffect->Play(position);
}

//�f�X�g���N�^
EnemyNormalGun::~EnemyNormalGun()
{
    //trangitionEffect->Stop(trangitionEffect->GetHandle());
    delete trangitionEffect;
    delete model;
}

//�X�V����
void EnemyNormalGun::Update(float elapsedTime)
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

    //trangitionEffect->SetPostion(trangitionEffect->GetHandle(), this->position);
    //trangitionEffect->SetRotation(trangitionEffect->GetHandle(), this->angledis);
    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //���f���s��X�V
    model->UpdateTransform(transform);
}

void EnemyNormalGun::Render(RenderContext& rc, ModelShader* shader)
{
    shader->Draw(rc, model);
}



void EnemyNormalGun::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& angle, const float& speed)
{
    this->direction = direction;
    this->position = position;
    this->angledis = angle;

    // ���x�̂��ꂼ��̒���
    this->speed = speed;
}

