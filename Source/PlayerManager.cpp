#include "PlayerManager.h"
#include "Graphics/Graphics.h"
#include "Collision.h"

//�X�V����
void PlayerManager::Update(float elapsedTime)
{
    for (Player* player : players)
    {
        player->Update(elapsedTime);
    }

    //�j������
    for (Player* player : removes)
    {
        //std::vector�v�f���폜����ꍇ�̓C�e���[�^�ō폜����
        std::vector<Player*>::iterator it = std::find(players.begin(), players.end(), player);

        if (it != players.end())
        {
            players.erase(it);
        }

        //�폜
        delete player;
    }
    //�j�����X�g���N���A
    removes.clear();
}

//�`�揈��
void PlayerManager::Render(RenderContext& context, ModelShader* shader)
{
    for (Player* player : players)
    {
        player->Render(context, shader);
    }
}

//�v���C���[�o�^
void PlayerManager::Register(Player* player)
{
    players.emplace_back(player);
}

//�v���C���[�폜
void PlayerManager::Remove(Player* player)
{
    //�j�����X�g�ɒǉ�
    removes.emplace_back(player);
}

//�v���C���[�S�폜
void PlayerManager::Clear()
{
    for (Player* player : players)
    {
        delete player;
    }
    players.clear();
}

void PlayerManager::DrawDebugPrimitive()
{
    //TODO �������O�̈׃R�����g�A�E�g
    //for (Player* player : players)
    //{
    //    player->DrawDebugPrimitive();
    //}
}

void PlayerManager::DrawDebugGUI()
{
    //TODO �������O�̈׃R�����g�A�E�g
    //for (Player* player : players)
    //{
    //    player->DrawDebugGUI();
    //}
}
