#include "EnemySkillGunArea.h"

//unit20

static EnemySkillGunArea* instance = nullptr;

//インスタンス取得
EnemySkillGunArea& EnemySkillGunArea::Instance()
{
    return *instance;

}

//コンストラクタ
EnemySkillGunArea::EnemySkillGunArea(ProjectileManager* manager,const char* nodeName, const char* name) : Projectile(manager)
{
    //インスタンスポインタ設定
    instance = this;

    //球モデル

    model = new Model(name);

    //表示サイズを調整
    scale.x = scale.y = scale.z = 0.1f;

    trangitionEffect = new Effect(nodeName);
    trangitionEffect->Play(position);
}

//デストラクタ
EnemySkillGunArea::~EnemySkillGunArea()
{
    delete model;
    delete trangitionEffect;
}

//更新処理
void EnemySkillGunArea::Update(float elapsedTime)
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
    //position.x += direction.x * speed;
    position.y += direction.y * speed;
    //position.z += direction.z * speed;

    //TODO スキルこの部分を参考にする
    //エフェクトを動かすのに必要
    //trangitionEffect->SetPostion(trangitionEffect->GetHandle(), position);
    //オブジェクト行列を更新
    UpdateTransform();

    //モデル行列更新
    model->UpdateTransform(transform);
}

void EnemySkillGunArea::Render(RenderContext& rc, ModelShader* shader)
{
    shader->Draw(rc, model);
}

void EnemySkillGunArea::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, const float& speed)
{
    this->direction = direction;
    this->position = position;

    this->speed = speed;
}