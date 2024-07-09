#pragma once
#include "Graphics/Model.h"
#include "Player.h"
#include "Effekseer.h"

//武器
class Weapon
{
public:

    Weapon() ;
    ~Weapon() ;

    //// 初期化
    //void initialize();

    //// 解放処理
    //void Clear();
    
    //行列更新処理
    void UpdateTransform();

    //更新処理
    void Update(float elapsedTime);

    //描画処理
    void Render(RenderContext& rc, ModelShader* shader);
private:

    // 双剣ステートへ移行
    void TransitionDual();

    // 双剣から変形更新処理
    void UpdateDual(float elapsedTime);

    // 大剣ステートへ移行
    void TransitionGread();

    // 大剣から変形更新処理
    void UpdateGread(float elapsedTime);

    // 双銃ステートへ移行
    void TransitionGun();

    // 双銃から変形更新処理
    void UpdateGun(float elapsedTime);
private:
    //アニメーション
    //enum Animation
    //{
    //    Anim_Dual_to_Gun,
    //    Anim_Dual_to_Great,
    //};

    //武器のアニメーション
    enum  Animation
    {
        // 武器変形
        Anim_Dual_to_Gread, // 双剣から大剣
        Anim_Gread_to_Gun,  // 大剣から銃
        Anim_Gun_to_Dual,   // 双剣から銃
    };

    //ステート家訓イン
    enum class State
    {
        Dual,
        Gread,
        Gun,
    };



private:
    Model* model = nullptr;    
    DirectX::XMFLOAT3       position = { 0,0,0 };       //位置
    DirectX::XMFLOAT3       angle = { 0,0,0 };          //回転
    DirectX::XMFLOAT3       scale = { 1,1,1 };          //スケール
    DirectX::XMFLOAT4X4     transform = {               //上のパラメータをもとに計算する行列
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
    DirectX::XMFLOAT3 playerPositionDual = { 0,0,0 };
    DirectX::XMFLOAT3 playerPositionGreat = { 0,0,0 };
    DirectX::XMFLOAT3 playerPositionGun = { 0,0,0 };

    // switch分用
    State state = State::Dual;

    std::unique_ptr<Effect> change;


    float sizeefk = 0.5f;


};
