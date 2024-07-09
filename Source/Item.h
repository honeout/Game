#pragma once

#include "Graphics/Shader.h"

//アイテム
class Item
{
public:
    Item() {};
    virtual ~Item() {};

    //行列更新処理
    void UpdateTransform();

    //描画処理
    virtual void Render(RenderContext& rc, ModelShader* shader) = 0;

    //更新処理
    virtual void Update(float elapsedTime) = 0;

    //破棄処理
    void Destroy();

    //デバッグプリミティブ描画
    virtual void DrawDebugPrimitive();

    //位置取得
    const DirectX::XMFLOAT3& GetPosition() const { return position; }

    //位置設定
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

    //回転取得
    const DirectX::XMFLOAT3& GetAngle() const { return angle; }

    //回転設定
    void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

    //スケール取得
    const DirectX::XMFLOAT3& GetScale() const { return scale; }

    //スケール設定
    void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

    //半径取得
    float GetRadius() const { return radius; }

    //地面に接地しているか
    bool IsGround() const { return isGround; }

    //高さ取得
    float GetHeight() const { return height; }

    //eraseindex取得設定
    void SetEraseIndex(int index) { erase_index = index; }
    int GetEraseIndex() { return erase_index; }

protected:
    DirectX::XMFLOAT3       position = { 0,0,0 };       //位置
    DirectX::XMFLOAT3       angle = { 0,0,0 };          //回転
    DirectX::XMFLOAT3       scale = { 1,1,1 };          //スケール
    DirectX::XMFLOAT4X4     transform = {               //上のパラメータをもとに計算する行列
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
    float radius = 0.5f;                                //半径
    float gravity = -1.0f;                              //重力
    DirectX::XMFLOAT3 velocity = { 0,0,0 };             //速力
    bool isGround = false;
    float height = 2.0f;                                //高さ
    int health = 5;                                     //体力
    float stepOffset = 1.0f;                            //レイキャスト処理用
    float slopeRate = 1.0f;                             //傾斜率
    int erase_index = 0;
};