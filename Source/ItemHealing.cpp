#include "ItemHealing.h"
#include "Mathf.h"
#include "ItemManager.h"
#include "Graphics/Graphics.h"
#include "Collision.h"
#include "Player.h"
#include "PlayerManager.h"



ItemHealing::ItemHealing()
{
    model = new Model("Data/Model/Item/Silvercar.mdl");

    //モデルのスケーリング
    scale.x = scale.y = scale.z = 0.2f;

    radius = 1.5f;
}

ItemHealing::~ItemHealing()
{
    delete model;
}

void ItemHealing::Update(float elapsedTime)
{
    angle.y += 0.1f;

    CollisionItemHealingVsPlayers(elapsedTime);

    //TODOこの2行がないとモデル表示されないので注意
    UpdateTransform();
    model->UpdateTransform(transform);
}

void ItemHealing::CollisionItemHealingVsPlayers(float elapsedTime)
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
            //TODO　ここでプレイヤーの体力を回復する処理
        }
    }
}

void ItemHealing::Render(RenderContext& rc, ModelShader* shader)
{
    shader->Draw(rc, model);
}

void ItemHealing::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}


void ItemHealing::Destroy()
{
    ItemManager::Instance().Remove(this);
}