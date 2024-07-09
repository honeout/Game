#pragma once

#include "Graphics/Model.h"
#include "Projectile.h"
//unit10
#include "Effect.h"

class EnemySkillGunHoming : public Projectile
{
public:
    EnemySkillGunHoming(ProjectileManager* manager, const char* nodeName, const char* name);
    ~EnemySkillGunHoming() override;

    void Update(float elaspedTime) override;
    void Render(RenderContext& rc, ModelShader* shader) override;

    void Launch(const DirectX::XMFLOAT3& direction,
        const DirectX::XMFLOAT3& position,
        const DirectX::XMFLOAT3& target,
        const DirectX::XMFLOAT3& angle);

private:

    Model* model = nullptr;
    DirectX::XMFLOAT3 target = { 0,0,0 };
    float moveSpeed = 6.0f;
    float turnSpeed = DirectX::XMConvertToRadians(180);
    float lifetimer = 3.0f;

    Effect* trangitionEffect = nullptr;


    bool homingout = false;
};
