#pragma once

//unit07
#include "Graphics/Model.h"
#include "Projectile.h"

//���i�e��
class ProjectileStraite : public Projectile
{
public:
    ProjectileStraite(ProjectileManager* manager);
    ~ProjectileStraite() override;

    //�X�V����
    void Update(float elapsedTime) override;

    //�`�揈��
    void Render(RenderContext& rc, ModelShader* shader)override;

    //����
    void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);

private:
    Model* model = nullptr;
    //TODO �e�̑��x
    float speed = 300.0f;
    float lifeTimer = 0.8f;
};