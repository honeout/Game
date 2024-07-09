#pragma once

//unit07
#include "Graphics/Model.h"
#include "Projectile.h"
//unit10
#include "Effect.h"

//���i�e��
class EnemySkillGunRock : public Projectile
{
public:
    //�C���X�^���X�擾  
    static EnemySkillGunRock& Instance();

    EnemySkillGunRock(ProjectileManager* manager, const char* nodeName, const char* name);
    ~EnemySkillGunRock() override;

    //�X�V����
    void Update(float elapsedTime) override;

    //�`�揈��
    void Render(RenderContext& rc, ModelShader* shader)override;

    //����
    void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);

private:
    Model* model = nullptr;
    float speed = 10.0f;
    float lifeTimer = 1.5f;

    Effect* trangitionEffect = nullptr;
};