#pragma once

#include "Graphics/Shader.h"

//�A�C�e��
class Item
{
public:
    Item() {};
    virtual ~Item() {};

    //�s��X�V����
    void UpdateTransform();

    //�`�揈��
    virtual void Render(RenderContext& rc, ModelShader* shader) = 0;

    //�X�V����
    virtual void Update(float elapsedTime) = 0;

    //�j������
    void Destroy();

    //�f�o�b�O�v���~�e�B�u�`��
    virtual void DrawDebugPrimitive();

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

    //���a�擾
    float GetRadius() const { return radius; }

    //�n�ʂɐڒn���Ă��邩
    bool IsGround() const { return isGround; }

    //�����擾
    float GetHeight() const { return height; }

    //eraseindex�擾�ݒ�
    void SetEraseIndex(int index) { erase_index = index; }
    int GetEraseIndex() { return erase_index; }

protected:
    DirectX::XMFLOAT3       position = { 0,0,0 };       //�ʒu
    DirectX::XMFLOAT3       angle = { 0,0,0 };          //��]
    DirectX::XMFLOAT3       scale = { 1,1,1 };          //�X�P�[��
    DirectX::XMFLOAT4X4     transform = {               //��̃p�����[�^�����ƂɌv�Z����s��
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
    float radius = 0.5f;                                //���a
    float gravity = -1.0f;                              //�d��
    DirectX::XMFLOAT3 velocity = { 0,0,0 };             //����
    bool isGround = false;
    float height = 2.0f;                                //����
    int health = 5;                                     //�̗�
    float stepOffset = 1.0f;                            //���C�L���X�g�����p
    float slopeRate = 1.0f;                             //�X�Η�
    int erase_index = 0;
};