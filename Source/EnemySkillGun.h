#pragma once

//unit07
#include "Graphics/Model.h"
#include "Projectile.h"
//unit10
#include "Effect.h"

//���i�e��
class EnemySkillGun : public Projectile
{
public:
    //�C���X�^���X�擾  
    static EnemySkillGun& Instance();

    EnemySkillGun(ProjectileManager* manager, const char* nodeName ,const char* name);
    ~EnemySkillGun() override;

    //�X�V����
    void Update(float elapsedTime) override;

    //�`�揈��
    void Render(RenderContext& rc, ModelShader* shader)override;

    //����
    void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& angle = { 0,0,0 }, const float& speed = 10.0f);

private:
    Model* model = nullptr;
    float speed = 10.0f;
    float lifeTimer = 5.0f;
    Effect* trangitionEffect = nullptr;
};