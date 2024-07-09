#include "Player.h"
#include "Graphics/Graphics.h"
#include "PlayerManager.h"

//static Player* instance = nullptr;
//
////インスタンス取得
//Player& Player::Instance()
//{
//    return *instance;
//}

//破棄
void Player::Destroy()
{
    PlayerManager::Instance().Remove(this);
}

void Player::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //衝突判定用のデバッグ球を描画
    //debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

    //衝突判定用のデバッグ円柱を描画
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}

void Player::DrawDebugGUI()
{
}
