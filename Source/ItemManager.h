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

    //�X�V����
    void Update(float elapsedTime);

    //�`�揈��
    void Render(RenderContext& rc, ModelShader* shader);

    //�o�^
    void Register(Item* item);

    //�폜
    void Remove(Item* item);

    //�A�C�e���S�폜
    void Clear();

    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

    //�A�C�e���̐��擾
    int GetItemCount() const { return static_cast<int>(items.size()); }

    //�ꊇ�o�^�p
    void RegisterBatch(const std::vector<Item*>& items);

    //�A�C�e���擾
    Item* GetItem(int index) { return items.at(index); }


public:
    std::vector<Item*> items;
    std::vector<Item*> removes;
};