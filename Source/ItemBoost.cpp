#include "ItemBoost.h"
#include "Mathf.h"
#include "ItemManager.h"
#include "Graphics/Graphics.h"
#include "Collision.h"
#include "Player.h"
#include "PlayerManager.h"



ItemBoost::ItemBoost()
{
    model = new Model("Data/Model/Item/Silvercar.mdl");

    //モデルのスケーリング
    scale.x = scale.y = scale.z = 0.2f;

    radius = 1.5f;
}

ItemBoost::~ItemBoost()
{
    delete model;
}

void ItemBoost::Update(float elapsedTime)
{
    angle.y += 0.1f;

    CollisionItemBoostVsPlayers(elapsedTime);

    //TODOこの2行がないとモデル表示されないので注意
    UpdateTransform();
    model->UpdateTransform(transform);
}

void ItemBoost::CollisionItemBoostVsPlayers(float elapsedTime)
{
    ItemManager& itemManager = ItemManager::Instance();
    PlayerManager& playerManager = PlayerManager::Instance();

    //全てのアイテムと衝突判定
    int itemCount = itemManager.GetItemCount();
    for (int i = 0; i < itemCount; ++i)
    {

        Item* item = itemManager.GetItem(i);

        //衝突処理
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectSphereVsCylinder(
            item->GetPosition(),
            item->GetRadius(),
            playerManager.GetPlayer(0)->GetPosition(),
            playerManager.GetPlayer(0)->GetRadius(),
            playerManager.GetPlayer(0)->GetHeight(),
            outPosition))
        {
            //TODO　ここでプレイヤーの最大速度を上げて一定時間動きを早くする
        }
    }
}

void ItemBoost::Render(RenderContext& rc, ModelShader* shader)
{
    shader->Draw(rc, model);
}

void ItemBoost::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}


void ItemBoost::Destroy()
{
    ItemManager::Instance().Remove(this);
}