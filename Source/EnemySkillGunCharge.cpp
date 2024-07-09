#include "EnemySkillGunCharge.h"

//unit20

static EnemySkillGunCharge* instance = nullptr;

//インスタンス取得
EnemySkillGunCharge& EnemySkillGunCharge::Instance()
{
    return *instance;
}

//コンストラクタ
EnemySkillGunCharge::EnemySkillGunCharge(ProjectileManager* manager, const char* nodeName, const char* name) : Projectile(manager)
{
    //インスタンスポインタ設定
    instance = this;

    //球モデル
    model = new Model(name);

    radius = 2.0f;

    //表示サイズを調整
    scale.x = 0.1f;
    scale.y = 0.1f;
    scale.z = 0.08f;

    hitEffect = new Effect(nodeName);
    hitEffect->Play(position);
}

//デストラクタ
EnemySkillGunCharge::~EnemySkillGunCharge()
{
    hitEffect->stop(hitEffect->GetHandle());
    delete hitEffect;
    delete model;
}

//更新処理
void EnemySkillGunCharge::Update(float elapsedTime)
{
    //寿命処理
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0.0f)
    {
        //自分を削除
        Destroy();
    }

    //移動
    float speed = this->speed * elapsedTime;
    position.x += direction.x * speed;
    position.y += direction.y * speed;
    position.z += direction.z * speed;


    hitEffect->SetPosition(hitEffect->GetHandle(), this->position);
    hitEffect->SetRotation(hitEffect->GetHandle(), this->angledis);

    //オブジェクト行列を更新
    UpdateTransform();

    //モデル行列更新
    model->UpdateTransform(transform);
}

void EnemySkillGunCharge::Render(RenderContext& rc, ModelShader* shader)
{
    shader->Draw(rc, model);
}

void EnemySkillGunCharge::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& angle, const float& speed)
{
    this->direction = direction;
    this->position = position;
    this->angledis = angle;

    this->speed = speed;
}