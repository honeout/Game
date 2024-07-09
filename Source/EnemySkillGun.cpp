#include "EnemySkillGun.h"

//unit20

static EnemySkillGun* instance = nullptr;

//インスタンス取得
EnemySkillGun& EnemySkillGun::Instance()
{
    return *instance;
}

//コンストラクタ
EnemySkillGun::EnemySkillGun(ProjectileManager* manager, const char* nodeName, const char* name) : Projectile(manager)
{
    //インスタンスポインタ設定
    instance = this;

    //球モデル
    model = new Model(name);

    radius = 1.5f;

    //表示サイズを調整
    scale.x = 0.07f;
    scale.y = 0.07f;
    scale.z = 0.04f;
    //trangitionEffect = new Effect("Data/Effect/fire.efk");
    //trangitionEffect = new Effect("Data/Effect/lightning attack.efk");
    //trangitionEffect = new Effect(nodeName);
    //trangitionEffect->Play(position);
}

//デストラクタ
EnemySkillGun::~EnemySkillGun()
{
    //trangitionEffect->Stop(trangitionEffect->GetHandle());
   // delete trangitionEffect;
    delete model;
}

//更新処理
void EnemySkillGun::Update(float elapsedTime)
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

    // 
    //trangitionEffect->SetPostion(trangitionEffect->GetHandle(), this->position);
    //trangitionEffect->SetRotation(trangitionEffect->GetHandle(), this->angledis);
    //オブジェクト行列を更新
    UpdateTransform();

    //モデル行列更新
    model->UpdateTransform(transform);
}

void EnemySkillGun::Render(RenderContext& rc, ModelShader* shader)
{
    shader->Draw(rc, model);
}

void EnemySkillGun::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position,const DirectX::XMFLOAT3& angle,const float& speed)
{
    this->direction = direction;
    this->position = position;
    this->angledis = angle;

    // 速度のそれぞれの調整
    this->speed = speed;
}

