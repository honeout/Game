#pragma once

#include "Graphics/Model.h"
#include "Player.h"
#include "ProjectileManager.h"
#include "StateMachineRobot.h"
#include "Weapon.h"

class PlayerMaster : public Player
{
public:
    PlayerMaster();
    ~PlayerMaster() override;

    //�X�V����
    void Update(float elapsedTime) override;

    //�`�揈��
    void Render(RenderContext& rc, ModelShader* shader) override;

    //�f�o�b�O�pGUI�`��
    void DrawDebugGUI();

    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();


    //���f���擾
    Model* GetModel() { return model; }

    //�X�e�B�b�N���͒l����ړ��x�N�g�����擾
    DirectX::XMFLOAT3 GetMoveVec() const;


private:
    //�o�� MasterBlades
    //�ҋ@�X�e�[�g�֑J��
    void TransitionIdle_MasterState();

    //�ҋ@�X�e�[�g�X�V����
    void UpdateIdle_MasterState(float elapsedTime);

    //����ύX�X�e�[�g�J��
    void TransitionChange_MasterState();

    //����ύX�X�e�[�g�X�V����
    void UpdateChange_MasterState(float elapsedTime);


private:
    //�X�e�[�g
    enum class State
    {
        //����Ȃ��@�i����ύX�p�j�@Master
        Idle_Master, //�ҋ@
        Change_Master, //����ύX
    };

private:
    //�A�j���[�V����
    enum MasterSwordAnimation
    {
        Anim_Master_Idle, //�ҋ@
        Anim_Master_Dash, //�_�b�V��
        Anim_Master_Brake,
        Anim_Master_Damaged,
        Anim_Master_Switch,
        Anim_Master_Attack_1,   //�U��
        Anim_Master_Attack_2,
        Anim_Master_Attack_3,
        Anim_Master_Dodge,    //���
        Anim_Master_Change,   //����ύX
        Anim_Master_GetHit,   //�q�b�g
        Anim_Master_Death,    //���S
        Anim_Master_Jump,     //�W�����v
        Anim_Master_Falling,  //�����Ă���
        Anim_Master_Landing,  //���n
    };


private:
    Model* model = nullptr;
    float moveSpeed = 5.0f;
    float turnSpeed = DirectX::XMConvertToRadians(720);
    float jumpSpeed = 20.0f;
    int jumpCount = 0;      //�����񂾂�
    int jumpLimit = 1;      //�W�����v�񐔐���
    ProjectileManager projectileManager;
    State state = State::Idle_Master;      //�X�e�[�g
    float MasterRadius = 0.8f;
    bool attackCollisionFlag = false;
    int statenumber = 0;
    int weapon = 0; //����p�ϐ�
    float changetime = 0.0f;

    Weapon* weaponModel = nullptr;

    
};