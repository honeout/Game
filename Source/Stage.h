#pragma once
#pragma once

#include "Graphics/Shader.h"
#include "Collision.h"
//ステージ
class Stage
{
public:
    Stage() {}
    virtual ~Stage() {}

    //更新処理
    virtual void Update(float elapsedTime) = 0;
    //描画処理
    //virtual void Render(ID3D11DeviceContext* dc, ModelShader* shader) = 0;
    virtual void Render(RenderContext& rc, ModelShader* shader) = 0;
    //レイキャスト
    virtual bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit) = 0;

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

    //スタート位置設定
    void SetStartPoint(const DirectX::XMFLOAT3& start) { this->start = start; }

    //ゴール位置設定
    void SetGoalPoint(const DirectX::XMFLOAT3& goal) { this->goal = goal; }

    //トルク設定
    void SetTorque(const DirectX::XMFLOAT3& torque) { this->torque = torque; }

    //移動速度設定
    void SetMoveSpeed(float speed) { moveSpeed = speed; }

    //移動速度取得
    float GetMoveSpeed() { return moveSpeed; }

    //移動頻度設定
    void SetMoveRate(float rate) { moveRate = rate; }

    //行列更新処理
    void UpdateTransform();

protected:
    DirectX::XMFLOAT3 position = { 0,0,0 };
    DirectX::XMFLOAT3 angle = { 0,0,0 };
    DirectX::XMFLOAT3 scale = { 1,1,1 };
    DirectX::XMFLOAT4X4 transform = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };

    DirectX::XMFLOAT3 torque = { 0,0,0 };
    DirectX::XMFLOAT3 start = { 0,0,0 };
    DirectX::XMFLOAT3 goal = { 0,0,0 };

    float moveSpeed = 0.0f;
    float moveRate = 0.0f;
    DirectX::XMFLOAT4X4 oldTransform = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
    DirectX::XMFLOAT3 oldAngle = { 0,0,0 };
};