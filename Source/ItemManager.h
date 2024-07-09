#pragma once
#include <vector>
#include "Item.h"
#include "ItemHealing.h"
#include "ItemBoost.h"
class ItemManager
{
public:
    ItemManager() {}
    ~ItemManager() {}

public:
    static ItemManager& Instance()
    {
        static ItemManager instance;
        return instance;
    }

    //更新処理
    void Update(float elapsedTime);

    //描画処理
    void Render(RenderContext& rc, ModelShader* shader);

    //登録
    void Register(Item* item);

    //削除
    void Remove(Item* item);

    //アイテム全削除
    void Clear();

    //デバッグプリミティブ描画
    void DrawDebugPrimitive();

    //アイテムの数取得
    int GetItemCount() const { return static_cast<int>(items.size()); }

    //一括登録用
    void RegisterBatch(const std::vector<Item*>& items);

    //アイテム取得
    Item* GetItem(int index) { return items.at(index); }


public:
    std::vector<Item*> items;
    std::vector<Item*> removes;
};