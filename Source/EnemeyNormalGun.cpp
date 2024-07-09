#include "EnemeyNormalGun.h"

//unit20

static EnemyNormalGun* instance = nullptr;

//インスタンス取得
EnemyNormalGun& EnemyNormalGun::Instance()
{
    return *instance;
}

//コンストラクタ
EnemyNormalGun::EnemyNormalGun(ProjectileManager* manager, const char* nodeName, const char* name) : Projectile(manager)
{
    //インスタンスポインタ設定
    instance = this;

    //球モデル
    model = new Model(name);
  //model = new Model("Data/Model/SpikeBall/SpikeBall.mdl");

    //表示サイズを調整
    scale.x = scale.y = scale.z = 1.0f;
    //trangitionEffect = new Effect("Data/Effect/fire.efk");
    //trangitionEffect = new Effect("Data/Effect/lightning attack.efk");
    //trangitionEffect = new Effect(nodeName);
    //trangitionEffect->Play(position);
}

//デストラクタ
EnemyNormalGun::~EnemyNormalGun()
{
    //trangitionEffect->Stop(trangitionEffect->GetHandle());
    delete trangitionEffect;
    delete model;
}

//更新処理
void EnemyNormalGun::Update(float elapsedTime)
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

    //trangitionEffect->SetPostion(trangitionEffect->GetHandle(), this->position);
    //trangitionEffect->SetRotation(trangitionEffect->GetHandle(), this->angledis);
    //オブジェクト行列を更新
    UpdateTransform();

    //モデル行列更新
    model->UpdateTransform(transform);
}

void EnemyNormalGun::Render(RenderContext& rc, ModelShader* shader)
{
    shader->Draw(rc, model);
}



void EnemyNormalGun::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& angle, const float& speed)
{
    this->direction = direction;
    this->position = position;
    this->angledis = angle;

    // 速度のそれぞれの調整
    this->speed = speed;
}

