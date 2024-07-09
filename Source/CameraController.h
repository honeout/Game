#pragma once

#include <DirectXMath.h>

// �J�����R���g���[���[
class CameraController
{
public:
    CameraController();
    ~CameraController() {}

    // �X�V����
    void Update(float elapsedTime);

    //�^�[�Q�b�g�ʒu�ݒ�
    void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

    // �J�����f���p
    void UpdateDemo(float elapsedTime);

    void DrawGUI();

    void AngleX(float angleX) { angle.x = angleX; }

    void CameraShake(float maxX, float minX, float maxY, float minY, float increase, float randMaxX = 0, float randMinX = 0, float randMaxY = 0, float randMinY = 0);

    void SetShakeCheck(bool shackchack) {this->shackchack = shackchack; }

    void camerafall(float elapsedTime);
    void CameraDown(float elapsedTime);

    void SetRange(float range) { this->range = range; }

    void SetAngleY(float angle) { this->angle.y = angle; }


    void CameraShackmouse(float maxX, float minX, float maxY, float minY, float increase, float randMaxX = 0, float randMinX = 0, float randMaxY = 0, float randMinY = 0);

    void SetCamera_Operate(float camera_operate) { this->camera_operate = camera_operate; }
    float GetCamera_Operate() { return camera_operate; }

    DirectX::XMFLOAT3 GetAngle() { return angle; }

    //void rangeincrece(float increce) {range += increce};
private:
    float					rotateX = DirectX::XMConvertToRadians(45);
    float					rotateY = 0.0f;
    float					distance = 10.0f;
    DirectX::XMFLOAT3 camera_position{ 0.0f, 0.0f, -10.0f };
    DirectX::XMFLOAT3 camera_focus{ 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT3 target = { 0,0,0 };
    DirectX::XMFLOAT3 angle = { 0,0,0 };
    float rollSpeed = DirectX::XMConvertToRadians(90);
    float range = 20.0f;

    //�ő��]�l�ƍŏ���]�l�����
    float maxAngleX = DirectX::XMConvertToRadians(45);
    float minAngleX = DirectX::XMConvertToRadians(0);


    float minAngleDemoX = DirectX::XMConvertToRadians(-85);

    float camera_operate = 0;
    float cameratime = 0.0f;

    // �f�����[�r�[�p�J�����𓮂���
    DirectX::XMFLOAT2 camerapower =  { 0.01f ,0.01f };

    // �V�F�C�N�̌���
    bool shakeX = true;
    bool shakeY = true;

    float shakespeed = 1;
    float ahakespeedMax = 3;
    bool shackchack = false;

    float moveX;
    float moveY;

    bool shakeMouseX = true;
    bool shakeMouseY = true;
};
