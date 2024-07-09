#pragma once

#include <vector>
#include "Player.h"

//プレイヤーマネージャー
class PlayerManager
{
private:
    PlayerManager() {}
    ~PlayerManager() {}

public:
    //唯一のインスタンス取得
    static PlayerManager& Instance()
    {
        static PlayerManager instance;
        return instance;
    }

    //更新処理
    void Update(float elapsedTime);

    //描画処理
    void Render(RenderContext& rc, ModelShader* shader);

    //プレイヤー登録
    void Register(Player* player);

    //プレイヤー削除
    void Remove(Player* player);

    //プレイヤー全削除
    void Clear();

    //デバッグプリミティブ描画
    void DrawDebugPrimitive();

    //デバッグ情報描画
    void DrawDebugGUI();

    //プレイヤー数取得
    int GetPlayerCount() const { return static_cast<int>(players.size()); }

    //プレイヤー取得
    Player* GetPlayer(int index) { return players.at(index); }

    //現在の武器番号の設定
    void SetWeapon(int index) { weapon_index = index; }

    //現在の武器番号の取得
    int GetWeapon() const { return weapon_index; }

    //前の武器番号の設定
    void SetBeforeWeapon(int index) { weapon_before_index = index; }

    //前の武器番号の設定
    int GetBeforeWeapon() const { return weapon_before_index; }

    //武器変更フラグ処理
    void SetWeaponFlag(bool flag) { weaponchangeflag = flag; }
    bool GetWeaponFlag() const { return weaponchangeflag; }
    
    void SetStartAnimation(bool startanimation) { this->startanimation = startanimation; }
    bool GetStartAnimation() { return this->startanimation; }

    bool GetGuardMode() {return inputguard;}
    void SetGuardMode(bool intpuguard) {this->inputguard = intpuguard;}
public:
    int weapon_index = 0;
    // 前の武器(モデル番号)
    int weapon_before_index = 0;
    bool weaponchangeflag = false;

    bool startanimation = false;

private:
    //複数のプレイヤーの管理のためにプレイヤーポインタをstd::vectorで管理
    std::vector<Player*> players;
    std::vector<Player*> removes;

    bool inputguard = false;


};