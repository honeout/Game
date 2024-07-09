#pragma once

#include "Graphics/Model.h"
#include "Item.h"
#include "Audio/Audio.h"

//アイテム
class ItemBoost : public Item
{
public:
    ItemBoost();
    ~ItemBoost();

    //更新処理
    void Update(float elapsedTime);

    //描画処理
    void Render(RenderContext& rc, ModelShader* shader);

    //デバッグプリミティブ描画
    void DrawDebugPrimitive();

    //破棄
    void Destroy();

private:

    //ノードとプレイヤーの衝突処理
   // void CollisionNodeVsPlayer(const char* nodeName, float nodeRadius);
    void CollisionItemBoostVsPlayers(float elapsedTime);

private:
    Model* model = nullptr;

    float stateTimer = 0.0f;
    float speedBoostTimer = 0.0f;

    DirectX::XMFLOAT4X4 oldTransform = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
    DirectX::XMFLOAT3 oldAngle = { 0,0,0 };
};