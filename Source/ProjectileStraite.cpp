#include "ProjectileStraite.h"

//コンストラクタ
ProjectileStraite::ProjectileStraite(ProjectileManager* manager) : Projectile(manager)
{
    //球モデル
    //model = new Model("Data/Model/SpikeBall/SpikeBall.mdl");
    model = new Model("Data/Model/Bullet/Bullet_02.mdl");

    //剣モデル
    //model = new Model("Data/Model/Sword/Sword.mdl");

    //表示サイズを調整
    scale.x = scale.y = scale.z = 0.05f;     //球
    //scale.x = scale.y = scale.z = 3.0f;   //剣
}

//デストラクタ
ProjectileStraite::~ProjectileStraite()
{
    delete model;
}

//更新処理
void ProjectileStraite::Update(float elapsedTime)
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

    //オブジェクト行列を更新
    UpdateTransform();

    //モデル行列更新
    model->UpdateTransform(transform);
}

void ProjectileStraite::Render(RenderContext& rc, ModelShader* shader)
{
    shader->Draw(rc, model);
}

void ProjectileStraite::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
    this->direction = direction;
    this->position = position;
}