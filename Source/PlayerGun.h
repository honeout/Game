#pragma once

#include "Graphics/Model.h"
#include "Player.h"
#include "ProjectileManager.h"
#include "StateMachineRobot.h"
#include "CameraController.h"
#include "Audio/Audio.h"

//���{�b�g
class PlayerGun : public Player
{
public:
    PlayerGun();
    ~PlayerGun() override;

    //�X�V����
    void Update(float elapsedTime) override;

    //�`�揈��
    void Render(RenderContext& rc, ModelShader* shader) override;

    //�f�o�b�O�pGUI�`��
    void DrawDebugGUI();

    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

    //�e�ۂƓG�̏Փˏ���
    void CollisionProjectilesVsEnemies();

    //�v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
    void CollisionPlayerVsEnemies();

    //�m�[�h�ƃG�l�~�[�̏Փˏ���
    void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);

    //���f���擾
    Model* GetModel() { return model; }

    //�X�e�B�b�N���͒l����ړ��x�N�g�����擾
    DirectX::XMFLOAT3 GetMoveVec() const;

    //�ړ����͏���
    bool InputMove(float elapsedTime);

    //�������͏���
    bool InputWalk(float elapsedTime);

    //������͏���
    bool InputRun(float elpsedTime);

    //�W�����v���͏���
    bool InputJump();

    //�e�ۓ��͏���
    void InputProjectile();

    //�U�����͏���
    bool InputAttack();

    //����ύX
    bool InputWeapon();

    //�K�[�h���͏���
    bool InputGuard(float elapsedTime);

    //�����[�h���͏���
    bool InputReload();

private:
    //�e Gun
    //�ҋ@�X�e�[�g�֑J��
    void TransitionIdle_GunState();

    //�ҋ@�X�e�[�g�X�V����
    void UpdateIdle_GunState(float elapsedTime);

    //�����X�e�[�g�֑J��
    void TransitionWalk_GunState();

    //�����X�e�[�g�X�V����
    void UpdateWalk_GunState(float elapsedTime);

    //����X�e�[�g�֑J��
    void TransitionRun_GunState();

    //����X�e�[�g�X�V����
    void UpdateRun_GunState(float elapsedTime);

    //�U���X�e�[�g�֑J��
    void TransitionAttack_GunState();

    //�U���X�e�[�g�X�V����
    void UpdateAttack_GunState(float elapsedTime);

    //�_���[�W�X�e�[�g�֑J��
    void TransitionDamage_GunState();

    //�_���[�W�X�e�[�g�X�V����
    void UpdateDamage_GunState(float elapsedTime);

    //���S�X�e�[�g�֑J��
    void TransitionDeath_GunState();

    //���S�X�e�[�g�X�V����
    void UpdateDeath_GunState(float elapsedTime);

    //����X�e�[�g�֑J��
    void TransitionDodge_GunState();

    //����X�e�[�g�X�V����
    void UpdateDodge_GunState(float elapsedTime);

    //�����[�h�X�e�[�g�֑J��
    void TransitionReload_GunState();

    //�����[�h�X�e�[�g�X�V����
    void UpdateReload_GunState(float elapsedTime);

    //����ύX�X�e�[�g�֑J��
    void TransitionChange_GunState();

    //����ύX�X�e�[�g�X�V����
    void UpdateChange_GunState(float elapsedTime);

    //�u���[�L�X�e�[�g�J��
    void TransitionBrake_GunState();

    //�u���[�L�X�e�[�g�X�V����
    void UpdateBrake_GunState(float elapsedTime);

private:
    //�X�e�[�g
    enum class State
    {
        //�e�@Gun
        Idle_Gun, //�ҋ@
        Walk_Gun, //����
        Run_Gun,  //����
        Jump_Gun, //�W�����v
        Land_Gun, //���n
        Attack_Gun, //�U��
        Damage_Gun, //�_���[�W
        Death_Gun, //���S
        Dodge_Gun, //���
        Reload_Gun, //�����[�h
        Change_Gun, //����ύX
        Brake_Gun,
    };

private:
    //�e�A�j���[�V����
    enum GunAnimation
    {
        Anim_Gun_Idle, //�ҋ@
        Anim_Gun_Dash, //�_�b�V��
        Anim_Gun_Dash_02,
        Anim_Gun_Attack,   //�U��
        Anim_Gun_Damaged,
        Anim_Gun_Reload,
        Anim_Gun_Brake,
        Anim_Gun_Dodge,    //���
        Anim_Gun_Change,   //����ύX
        Anim_Gun_GetHit,   //�q�b�g
        Anim_Gun_Death,    //���S
        Anim_Gun_Jump,     //�W�����v
        Anim_Gun_Falling,  //�����Ă���
        Anim_Gun_Landing,  //���n
    };

    enum class LockonState
    {
        NotLocked,
        Locked,
    };

protected:


    //���S�����Ƃ��ɌĂ΂��
    void OnDead() override;

    //�_���[�W���󂯂��Ƃ��ɌĂ΂��
    void OnDamaged() override;

private:
    Model* model = nullptr;
    float moveSpeed = 5.0f;
    float turnSpeed = DirectX::XMConvertToRadians(720);
    float jumpSpeed = 20.0f;
    int jumpCount = 0;      //�����񂾂�
    int jumpLimit = 1;      //�W�����v�񐔐���
    ProjectileManager projectileManager;
    Effect* hitEffect = nullptr;
    Effect* invincible_hitEffect = nullptr;
    Effect* shotEffect = nullptr;
    State state = State::Idle_Gun;      //�X�e�[�g
    float leftHandRadius = 0.4f;
    bool attackCollisionFlag = false;
    int statenumber = 0;
    int weapon = 0; //����p�ϐ�
    float changetime = 0.0f;
    
    int bullets = 60;   //�c�e��
    float reloadtime = 0.0f;
    int totalattackcount = 0;
    float rate = 0.0f;
    float braketime = 0.0f;
    CameraController* cameraController = nullptr;
    std::unique_ptr<AudioSource> shot;
    std::unique_ptr<AudioSource> reload;
    std::unique_ptr<AudioSource> damaged;
    std::unique_ptr<AudioSource> brake;
    std::unique_ptr<AudioSource> run_fast;
    std::unique_ptr<AudioSource> run_slow;

    Model::Node* weponNode;
};