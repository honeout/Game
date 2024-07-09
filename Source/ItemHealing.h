#pragma once

#include "Graphics/Model.h"
#include "Item.h"
#include "Audio/Audio.h"

//�A�C�e��
class ItemHealing : public Item
{
public:
    ItemHealing();
    ~ItemHealing();

    //�X�V����
    void Update(float elapsedTime);

    //�`�揈��
    void Render(RenderContext& rc, ModelShader* shader);

    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

    //�j��
    void Destroy();

private:

    //�m�[�h�ƃv���C���[�̏Փˏ���
   // void CollisionNodeVsPlayer(const char* nodeName, float nodeRadius);
    void CollisionItemHealingVsPlayers(float elapsedTime);

private:
    Model* model = nullptr;

    float stateTimer = 0.0f;
    float speedBoostTimer = 0.0f;

    DirectX::XMFLOAT4X4 oldTransform = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
    DirectX::XMFLOAT3 oldAngle = { 0,0,0 };
};