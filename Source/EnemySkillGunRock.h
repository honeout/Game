#pragma once

//unit07
#include "Graphics/Model.h"
#include "Projectile.h"
//unit10
#include "Effect.h"

//直進弾丸
class EnemySkillGunRock : public Projectile
{
public:
    //インスタンス取得  
    static EnemySkillGunRock& Instance();

    EnemySkillGunRock(ProjectileManager* manager, const char* nodeName, const char* name);
    ~EnemySkillGunRock() override;

    //更新処理
    void Update(float elapsedTime) override;

    //描画処理
    void Render(RenderContext& rc, ModelShader* shader)override;

    //発射
    void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);

private:
    Model* model = nullptr;
    float speed = 10.0f;
    float lifeTimer = 1.5f;

    Effect* trangitionEffect = nullptr;
};