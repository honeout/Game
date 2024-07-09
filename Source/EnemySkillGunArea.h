#pragma once

//unit07
#include "Graphics/Model.h"
#include "Projectile.h"
//unit10
#include "Effect.h"

//���i�e��
class EnemySkillGunArea : public Projectile
{
public:
    //�C���X�^���X�擾  
    static EnemySkillGunArea& Instance();

    EnemySkillGunArea(ProjectileManager* manager, const char* nodeName, const char* name);
    ~EnemySkillGunArea() override;

    //�X�V����
    void Update(float elapsedTime) override;

    //�`�揈��
    void Render(RenderContext& rc, ModelShader* shader)override;

    //����
    void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, const float& speed = 10.0f);

private:
    Model* model = nullptr;
    float speed = 10.0f;
    float lifeTimer = 5.5f;

    Effect* trangitionEffect = nullptr;
};