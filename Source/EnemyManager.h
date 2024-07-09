#pragma once

#include<vector>
#include "Enemy.h"

//エネミーマネージャー
class EnemyManager
{
private:
    EnemyManager() {}
    ~EnemyManager() {}

public:
    //唯一のインスタンス取得(シングルトン化)
    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }

    //更新処理
    void Update(float elapsedTime);

    //描画処理
    void Render(RenderContext& rc, ModelShader* shader);

    //エネミー登録
    void Register(Enemy* enemy);

    //エネミー削除
    void Remove(Enemy* enemy);

    //エネミー全削除
    void Clear();

    //デバッグプリミティブ描画
    void DrawDebugPrimitive();

    //デバッグ情報描画
    void DrawDebugGUI();

    //エネミー数取得
    int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

    //エネミー取得
    Enemy* GetEnemy(int index) { return enemies.at(index); }

    bool GetDie() { return die; }
    void SetDie(bool die) { this->die = die; }

private:
    //エネミー同士の衝突処理
    void CollisionEnemyVsEnemies();

private:
    //複数のエネミーの管理のためにエネミーポインタをstd::vectorで管理
    std::vector<Enemy*> enemies;
    std::vector<Enemy*> removes;

    bool die = false;

};