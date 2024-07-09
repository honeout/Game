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

    //���f���̃X�P�[�����O
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

    //TODO����2�s���Ȃ��ƃ��f���\������Ȃ��̂Œ���
    UpdateTransform();
    model->UpdateTransform(transform);
}

void ItemHealing::CollisionItemHealingVsPlayers(float elapsedTime)
{
    ItemManager& itemManager = ItemManager::Instance();
    PlayerManager& playerManager = PlayerManager::Instance();

    //�S�ẴA�C�e���ƏՓ˔���
    int itemCount = itemManager.GetItemCount();
    for (int i = 0; i < itemCount; ++i)
    {

        Item* item = itemManager.GetItem(i);

        //�Փˏ���
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectSphereVsCylinder(
            item->GetPosition(),
            item->GetRadius(),
            playerManager.GetPlayer(0)->GetPosition(),
            playerManager.GetPlayer(0)->GetRadius(),
            playerManager.GetPlayer(0)->GetHeight(),
            outPosition))
        {
            //TODO�@�����Ńv���C���[�̗̑͂��񕜂��鏈��
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