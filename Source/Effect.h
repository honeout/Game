#pragma once

#include <DirectXMath.h>
#include <Effekseer.h>

//�G�t�F�N�g
class Effect
{
public:
    Effect(const char* filename);
    ~Effect() {};

    //�Đ�
    Effekseer::Handle Play(const DirectX::XMFLOAT3& position, float scale = 1.0f);

    //��~
    void stop(Effekseer::Handle handle);

    //���W�ݒ�
    void SetPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position);

    //�X�P�[���ݒ�
    void SetScale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale);

    //��]�ݒ�
    void SetRotation(Effekseer::Handle handle, const DirectX::XMFLOAT3& angle);

    // �G�t�F�N�g�n���h���Q�b�g
    Effekseer::Handle GetHandle() { return this->handle; }

private:
    Effekseer::EffectRef effekseerEffect;
    Effekseer::Handle handle;
};
