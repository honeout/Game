#include "PlayerManager.h"
#include "Graphics/Graphics.h"
#include "Collision.h"

//更新処理
void PlayerManager::Update(float elapsedTime)
{
    for (Player* player : players)
    {
        player->Update(elapsedTime);
    }

    //破棄処理
    for (Player* player : removes)
    {
        //std::vector要素を削除する場合はイテレータで削除する
        std::vector<Player*>::iterator it = std::find(players.begin(), players.end(), player);

        if (it != players.end())
        {
            players.erase(it);
        }

        //削除
        delete player;
    }
    //破棄リストをクリア
    removes.clear();
}

//描画処理
void PlayerManager::Render(RenderContext& context, ModelShader* shader)
{
    for (Player* player : players)
    {
        player->Render(context, shader);
    }
}

//プレイヤー登録
void PlayerManager::Register(Player* player)
{
    players.emplace_back(player);
}

//プレイヤー削除
void PlayerManager::Remove(Player* player)
{
    //破棄リストに追加
    removes.emplace_back(player);
}

//プレイヤー全削除
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
    //TODO ここも念の為コメントアウト
    //for (Player* player : players)
    //{
    //    player->DrawDebugPrimitive();
    //}
}

void PlayerManager::DrawDebugGUI()
{
    //TODO ここも念の為コメントアウト
    //for (Player* player : players)
    //{
    //    player->DrawDebugGUI();
    //}
}
