#pragma once
#include "Graphics/Model.h"
#include "Player.h"
#include "Effekseer.h"

//����
class Weapon
{
public:

    Weapon() ;
    ~Weapon() ;

    //// ������
    //void initialize();

    //// �������
    //void Clear();
    
    //�s��X�V����
    void UpdateTransform();

    //�X�V����
    void Update(float elapsedTime);

    //�`�揈��
    void Render(RenderContext& rc, ModelShader* shader);
private:

    // �o���X�e�[�g�ֈڍs
    void TransitionDual();

    // �o������ό`�X�V����
    void UpdateDual(float elapsedTime);

    // �匕�X�e�[�g�ֈڍs
    void TransitionGread();

    // �匕����ό`�X�V����
    void UpdateGread(float elapsedTime);

    // �o�e�X�e�[�g�ֈڍs
    void TransitionGun();

    // �o�e����ό`�X�V����
    void UpdateGun(float elapsedTime);
private:
    //�A�j���[�V����
    //enum Animation
    //{
    //    Anim_Dual_to_Gun,
    //    Anim_Dual_to_Great,
    //};

    //����̃A�j���[�V����
    enum  Animation
    {
        // ����ό`
        Anim_Dual_to_Gread, // �o������匕
        Anim_Gread_to_Gun,  // �匕����e
        Anim_Gun_to_Dual,   // �o������e
    };

    //�X�e�[�g�ƌP�C��
    enum class State
    {
        Dual,
        Gread,
        Gun,
    };



private:
    Model* model = nullptr;    
    DirectX::XMFLOAT3       position = { 0,0,0 };       //�ʒu
    DirectX::XMFLOAT3       angle = { 0,0,0 };          //��]
    DirectX::XMFLOAT3       scale = { 1,1,1 };          //�X�P�[��
    DirectX::XMFLOAT4X4     transform = {               //��̃p�����[�^�����ƂɌv�Z����s��
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
    DirectX::XMFLOAT3 playerPositionDual = { 0,0,0 };
    DirectX::XMFLOAT3 playerPositionGreat = { 0,0,0 };
    DirectX::XMFLOAT3 playerPositionGun = { 0,0,0 };

    // switch���p
    State state = State::Dual;

    std::unique_ptr<Effect> change;


    float sizeefk = 0.5f;


};
