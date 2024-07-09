#pragma once

//unit07
#include "Graphics/Model.h"
#include "Projectile.h"

//unit10
#include "Effect.h"

//直進弾丸
class EnemySkillGunCharge : public Projectile
{
public:
    //インスタンス取得  
    static EnemySkillGunCharge& Instance();

    EnemySkillGunCharge(ProjectileManager* manager, const char* nodeName, const char* name);
    ~EnemySkillGunCharge() override;

    //更新処理
    void Update(float elapsedTime) override;

    //描画処理
    void Render(RenderContext& rc, ModelShader* shader)override;

    //発射
    void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& angle = { 0,0,0 }, const float& speed = 10.0f);

public:
    DirectX::XMFLOAT3 angledis = { 0,0,0 };

private:
    Model* model = nullptr;
    float speed = 20.0f;
    float lifeTimer = 10.0f;

    Effect* hitEffect = nullptr;
};
