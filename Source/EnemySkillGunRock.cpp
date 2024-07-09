#include "EnemySkillGunRock.h"

//unit20

static EnemySkillGunRock* instance = nullptr;

//インスタンス取得
EnemySkillGunRock& EnemySkillGunRock::Instance()
{
    return *instance;

}


//コンストラクタ
EnemySkillGunRock::EnemySkillGunRock(ProjectileManager* manager, const char* nodeName, const char* name) : Projectile(manager)
{
    //インスタンスポインタ設定
    instance = this;

    //球モデル
    //model = new Model("Data/Model/Sword/Sword.mdl");
    model = new Model(name);

    //表示サイズを調整
    scale.x = scale.y = scale.z = 0.3f;
    //trangitionEffect = new Effect("Data/Effect/fire.efk");
    //trangitionEffect = new Effect("Data/Effect/lightning strick boss.efk");
    trangitionEffect = new Effect(nodeName);
    trangitionEffect->Play(position);
}

//デストラクタ
EnemySkillGunRock::~EnemySkillGunRock()
{
    delete model;
    delete trangitionEffect;
}

//更新処理
void EnemySkillGunRock::Update(float elapsedTime)
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
    //position.y += direction.y * speed;
    //position.z += direction.z * speed;
    //trangitionEffect->SetPostion(trangitionEffect->GetHandle(), position);
    //オブジェクト行列を更新
    UpdateTransform();

    //モデル行列更新
    model->UpdateTransform(transform);
}

void EnemySkillGunRock::Render(RenderContext& rc, ModelShader* shader)
{
    shader->Draw(rc, model);
}

void EnemySkillGunRock::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
    this->direction = direction;
    this->position = position;
}