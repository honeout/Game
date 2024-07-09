#pragma once
#pragma once

#include "Graphics/Shader.h"
#include "Collision.h"
//�X�e�[�W
class Stage
{
public:
    Stage() {}
    virtual ~Stage() {}

    //�X�V����
    virtual void Update(float elapsedTime) = 0;
    //�`�揈��
    //virtual void Render(ID3D11DeviceContext* dc, ModelShader* shader) = 0;
    virtual void Render(RenderContext& rc, ModelShader* shader) = 0;
    //���C�L���X�g
    virtual bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit) = 0;

    //�ʒu�擾
    const DirectX::XMFLOAT3& GetPosition() const { return position; }

    //�ʒu�ݒ�
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

    //��]�擾
    const DirectX::XMFLOAT3& GetAngle() const { return angle; }

    //��]�ݒ�
    void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

    //�X�P�[���擾
    const DirectX::XMFLOAT3& GetScale() const { return scale; }

    //�X�P�[���ݒ�
    void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

    //�X�^�[�g�ʒu�ݒ�
    void SetStartPoint(const DirectX::XMFLOAT3& start) { this->start = start; }

    //�S�[���ʒu�ݒ�
    void SetGoalPoint(const DirectX::XMFLOAT3& goal) { this->goal = goal; }

    //�g���N�ݒ�
    void SetTorque(const DirectX::XMFLOAT3& torque) { this->torque = torque; }

    //�ړ����x�ݒ�
    void SetMoveSpeed(float speed) { moveSpeed = speed; }

    //�ړ����x�擾
    float GetMoveSpeed() { return moveSpeed; }

    //�ړ��p�x�ݒ�
    void SetMoveRate(float rate) { moveRate = rate; }

    //�s��X�V����
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