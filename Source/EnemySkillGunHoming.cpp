#include "EnemySkillGunHoming.h"
#include "EnemyManager.h"

EnemySkillGunHoming::EnemySkillGunHoming(ProjectileManager* manager, const char* nodeName, const char* name) : Projectile(manager)
{
    //model = new Model("Data/Model/Sword/Sword.mdl");

    //ブラックホール（追尾弾）
    //model = new Model("Data/Model/Sword/Blackhole.mdl");
    model = new Model(name);

    trangitionEffect = new Effect(nodeName);
    trangitionEffect->Play(position);

    scale.x = scale.y = scale.z = 0.1f;
}


EnemySkillGunHoming::~EnemySkillGunHoming()
{
    delete model;
    delete trangitionEffect;
}

void EnemySkillGunHoming::Update(float elaspedTime)
{
    //trangitionEffect->SetPostion(trangitionEffect->GetHandle(), position);
    //trangitionEffect->SetRotation(trangitionEffect->GetHandle(), angle);

    //寿命処理
    lifetimer -= elaspedTime;

    if (lifetimer <= 0.0f)
    {
        Destroy();
    }

    //移動
    {
        float moveSpeed = this->moveSpeed * elaspedTime;
        position.x += direction.x * moveSpeed;
        position.y += direction.y * moveSpeed;
        position.z += direction.z * moveSpeed;

    }




    //回転
    {
        float turnSpeed = this->turnSpeed * elaspedTime;
        //ターゲットまでのベクトルを算出
        DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);
        DirectX::XMVECTOR Target = DirectX::XMLoadFloat3(&target);
        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Target, Position);

        //ゼロベクトルをでないなら回転処理
        DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
        float lengthSq;
        DirectX::XMStoreFloat(&lengthSq, LengthSq);
        // ホーミング状態なら良い終わったら消える。
        //if (lengthSq > 0.00001f)
        if (!homingout)
        if (lengthSq > 0.1f)
        {
            //ターゲットまでのベクトルを単位ベクトル化
            Vec = DirectX::XMVector3Normalize(Vec);
            //向いている方向ベクトルを算出
            DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);
            //前方方向ベクトルとターゲットまでのベクトルの内積（角度）を算出
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Direction, Vec);

            float dot;
            DirectX::XMStoreFloat(&dot, Dot);

            float dotmax = 1.0f;
            //2つの単位ベクトルの角度が小さいほど
            //1.0に近づくという性質を利用して回転速度を調整する
            float rot = dotmax - dot;
            if (rot > turnSpeed) rot = turnSpeed;

            //回転角度があるなら回転処理をする
            if (fabsf(rot) > 0.0001f)
            {
                //回転軸を算出
                DirectX::XMVECTOR Axis = DirectX::XMVector3Cross(Direction, Vec);
                Axis = DirectX::XMVector3Normalize(Axis);
                //回転軸と回転量から回転行列を算出
                DirectX::XMMATRIX Rotation = DirectX::XMMatrixRotationAxis(Axis, rot);
                //現在の行列を回転させる(1)
                DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);
                Transform = DirectX::XMMatrixMultiply(Transform, Rotation);
                //回転後の前方方向を取り出し、単位ベクトル化する
                Direction = DirectX::XMVector3Normalize(Transform.r[2]);
                DirectX::XMStoreFloat3(&direction, Direction);
                //現在の行列を回転させる(2)
            //Direction = DirectX::XMVector3Transform(Direction, Rotation);
            //Direction = DirectX::XMVector3Normalize(Direction);
            //DirectX::XMStoreFloat3(&direction, Direction);
            }


        }
        else
        {
            homingout = true;
        }
    }



    UpdateTransform();
    model->UpdateTransform(transform);

}

void EnemySkillGunHoming::Render(RenderContext& rc, ModelShader* shader)
{
    shader->Draw(rc, model);
}

void EnemySkillGunHoming::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& target , const DirectX::XMFLOAT3& angle)
{
    this->direction = direction;
    this->position = position;
    this->target = target;
    this->angle = angle;
    UpdateTransform();
}
