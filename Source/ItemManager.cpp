#include "ItemManager.h"
#include "Graphics/Graphics.h"
#include "Collision.h"

//デバッグ描画
void ItemManager::DrawDebugPrimitive()
{
    for (Item* item_ : items)
    {
        item_->DrawDebugPrimitive();
    }
}

//更新処理
void ItemManager::Update(float elapsedTime)
{
    for (Item* item_ : items)
    {
        item_->Update(elapsedTime);
    }

    //破棄処理
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

//描画処理
void ItemManager::Render(RenderContext& rc, ModelShader* shader)
{
    for (Item* item_ : items)
    {
        item_->Render(rc, shader);
    }
}

//登録処理
void ItemManager::Register(Item* item)
{
    items.emplace_back(item);
}

//破棄処理
void ItemManager::Remove(Item* item)
{
    removes.emplace_back(item);
}

//全削除
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
    //渡されたオブジェクトをマネージャーに登録する処理
    for (const auto& item_ : items)
    {
        //マネージャーに登録する処理
        Register(item_);
    }
}
