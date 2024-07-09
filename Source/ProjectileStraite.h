#pragma once

//unit07
#include "Graphics/Model.h"
#include "Projectile.h"

//’¼i’eŠÛ
class ProjectileStraite : public Projectile
{
public:
    ProjectileStraite(ProjectileManager* manager);
    ~ProjectileStraite() override;

    //XVˆ—
    void Update(float elapsedTime) override;

    //•`‰æˆ—
    void Render(RenderContext& rc, ModelShader* shader)override;

    //”­Ë
    void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);

private:
    Model* model = nullptr;
    //TODO ’e‚Ì‘¬“x
    float speed = 300.0f;
    float lifeTimer = 0.8f;
};