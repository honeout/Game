#include "Player.h"
#include "Graphics/Graphics.h"
#include "PlayerManager.h"

//static Player* instance = nullptr;
//
////�C���X�^���X�擾
//Player& Player::Instance()
//{
//    return *instance;
//}

//�j��
void Player::Destroy()
{
    PlayerManager::Instance().Remove(this);
}

void Player::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //�Փ˔���p�̃f�o�b�O����`��
    //debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

    //�Փ˔���p�̃f�o�b�O�~����`��
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}

void Player::DrawDebugGUI()
{
}
