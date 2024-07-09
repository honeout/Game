#pragma once

//unit07
#include "Graphics/Model.h"
#include "Projectile.h"
//unit10
#include "Effect.h"

//直進弾丸
class EnemySkillGunArea : public Projectile
{
public:
    //インスタンス取得  
    static EnemySkillGunArea& Instance();

    EnemySkillGunArea(ProjectileManager* manager, const char* nodeName, const char* name);
    ~EnemySkillGunArea() override;

    //更新処理
    void Update(float elapsedTime) override;

    //描画処理
    void Render(RenderContext& rc, ModelShader* shader)override;

    //発射
    void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, const float& speed = 10.0f);

private:
    Model* model = nullptr;
    float speed = 10.0f;
    float lifeTimer = 5.5f;

    Effect* trangitionEffect = nullptr;
};