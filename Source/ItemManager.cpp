#include "ItemManager.h"
#include "Graphics/Graphics.h"
#include "Collision.h"

//�f�o�b�O�`��
void ItemManager::DrawDebugPrimitive()
{
    for (Item* item_ : items)
    {
        item_->DrawDebugPrimitive();
    }
}

//�X�V����
void ItemManager::Update(float elapsedTime)
{
    for (Item* item_ : items)
    {
        item_->Update(elapsedTime);
    }

    //�j������
    for (Item* item_ : removes)
    {
        std::vector<Item*>::iterator it = std::find(items.begin(), items.end(), item_);

        if (it != items.end())
        {
            items.erase(it);
        }

        delete item_;
    }

    removes.clear();
}

//�`�揈��
void ItemManager::Render(RenderContext& rc, ModelShader* shader)
{
    for (Item* item_ : items)
    {
        item_->Render(rc, shader);
    }
}

//�o�^����
void ItemManager::Register(Item* item)
{
    items.emplace_back(item);
}

//�j������
void ItemManager::Remove(Item* item)
{
    removes.emplace_back(item);
}

//�S�폜
void ItemManager::Clear()
{
    for (Item* item_ : items)
    {
        delete item_;
    }
    items.clear();
}


void ItemManager::RegisterBatch(const std::vector<Item*>& items)
{
    //�n���ꂽ�I�u�W�F�N�g���}�l�[�W���[�ɓo�^���鏈��
    for (const auto& item_ : items)
    {
        //�}�l�[�W���[�ɓo�^���鏈��
        Register(item_);
    }
}
