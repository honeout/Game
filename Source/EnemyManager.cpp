#include "EnemyManager.h"
#include "Graphics/Graphics.h"
#include "Collision.h"

//デバッグプリミティブ描画
void EnemyManager::DrawDebugPrimitive()
{
    for (Enemy* enemy : enemies)
    {
        enemy->DrawDebugPrimitive();
    }
}

void EnemyManager::DrawDebugGUI()
{
    for (Enemy* enemy : enemies)
    {
        enemy->DrawDebugGUI();
    }
}

//更新処理
void EnemyManager::Update(float elapsedTime)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Update(elapsedTime);
    }

    //破棄処理
    for (Enemy* enemy : removes)
    {
        //std::vector要素を削除する場合はイテレータで削除する
        std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);
        if (it != enemies.end())
        {
            enemies.erase(it);
        }

        //削除
        delete enemy;
    }
    //破棄リストをクリア
    removes.clear();

    //敵同士の衝突処理
    CollisionEnemyVsEnemies();
}

//描画処理
void EnemyManager::Render(RenderContext& context, ModelShader* shader)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Render(context, shader);
    }
}

//エネミー登録
void EnemyManager::Register(Enemy* enemy)
{
    enemies.emplace_back(enemy);
}

//エネミー削除
void EnemyManager::Remove(Enemy* enemy)
{
    //破棄リストに追加
    removes.emplace_back(enemy);
}

//エネミー全削除
void EnemyManager::Clear()
{
    for (Enemy* enemy : enemies)
    {
        delete enemy;
    }
    enemies.clear();
}

//エネミー同士の衝突処理
void EnemyManager::CollisionEnemyVsEnemies()
{
    size_t enemyCount = enemies.size();
    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemyA = enemies.at(i);
        for (int j = i + 1; j < enemyCount; ++j)
        {
            Enemy* enemyB = enemies.at(j);

            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectCylinderVsCylinder(
                enemyA->GetPosition(),
                enemyA->GetRadius(),
                enemyA->GetHeight(),
                enemyB->GetPosition(),
                enemyB->GetRadius(),
                enemyB->GetHeight(),
                outPosition))
            {
                enemyB->SetPosition(outPosition);
            }
        }
    }
}