#pragma once

#include "Graphics/Model.h"
#include "Player.h"
#include "ProjectileManager.h"
#include "StateMachineRobot.h"
#include "Audio/Audio.h"

//���{�b�g
class PlayerDual : public Player
{
public:
    PlayerDual();
    ~PlayerDual() override;

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

    //�����͏���
    bool InputDodge(float elpsedTime);

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
    //�o�� DualBlades
    //�ҋ@�X�e�[�g�֑J��
    void TransitionIdle_DualState();

    //�ҋ@�X�e�[�g�X�V����
    void UpdateIdle_DualState(float elapsedTime);

    //�����X�e�[�g�֑J��
    void TransitionWalk_DualState();

    //�����X�e�[�g�X�V����
    void UpdateWalk_DualState(float elapsedTime);

    //����X�e�[�g�֑J��
    void TransitionRun_DualState();

    //����X�e�[�g�X�V����
    void UpdateRun_DualState(float elapsedTime);


    //�U���X�e�[�g�֑J��
    void TransitionAttack_DualState();

    //�U���X�e�[�g�X�V����
    void UpdateAttack_DualState(float elapsedTime);

    //�_���[�W�X�e�[�g�֑J��
    void TransitionDamage_DualState();

    //�_���[�W�X�e�[�g�X�V����
    void UpdateDamage_DualState(float elapsedTime);

    //���S�X�e�[�g�֑J��
    void TransitionDeath_DualState();

    //���S�X�e�[�g�X�V����
    void UpdateDeath_DualState(float elapsedTime);

    //����X�e�[�g�֑J��
    void TransitionDodge_DualState();

    //����X�e�[�g�X�V����
    void UpdateDodge_DualState(float elapsedTime);

    //����ύX�X�e�[�g�J��
    void TransitionChange_DualState();

    //����ύX�X�e�[�g�X�V����
    void UpdateChange_DualState(float elapsedTime);

    //�u���[�L�X�e�[�g�J��
    void TransitionBrake_DualState();

    //�u���[�L�X�e�[�g�X�V����
    void UpdateBrake_DualState(float elapsedTime);

private:
    //�X�e�[�g
    enum class State
    {
        //�o�� DualBlades
        Idle_Dual, //�ҋ@
        Walk_Dual, //����
        Run_Dual,  //����
        Jump_Dual, //�W�����v
        Land_Dual, //���n
        Attack_Dual, //�U��
        Damage_Dual, //�_���[�W
        Death_Dual,  //���S
        Dodge_Dual,  //���
        Change_Dual, //����ύX
        Brake_Dual, //�u���[�L
    };

private:
    //�o���A�j���[�V����
    enum DualSwordAnimation
    {
        Anim_Dual_Idle, //�ҋ@
        Anim_Dual_Dash, //�_�b�V��
        Anim_Dual_Attack_1,   //�U��
        Anim_Dual_Attack_2,
        Anim_Dual_Attack_3,
        Anim_Dual_Damaged,
        Anim_Dual_Brake,
        Anim_Dual_Change,   //����ύX
        Anim_Dual_Dodge,    //���
        Anim_Dual_GetHit,   //�q�b�g
        Anim_Dual_Death,    //���S
        Anim_Dual_Jump,     //�W�����v
        Anim_Dual_Falling,  //�����Ă���
        Anim_Dual_Landing,  //���n
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
    State state = State::Idle_Dual;      //�X�e�[�g
    float dualRadius = 0.8f;
    bool attackCollisionFlag = false;
    int statenumber = 0;
    int weapon = 0; //����p�ϐ�
    float changetime = 0.0f;
    int attackcount = 0;
    int bullets = 60;   //�c�e��
    float reloadtime = 0.0f;
    int totalattackcount = 0;
    float attacktime = 0.0f;
    float attackrate = 0.0f;
    std::unique_ptr<AudioSource> attack_01;
    std::unique_ptr<AudioSource> attack_02;
    std::unique_ptr<AudioSource> attack_03;
    std::unique_ptr<AudioSource> damaged;
    std::unique_ptr<AudioSource> brake;
    std::unique_ptr<AudioSource> run_fast;
    std::unique_ptr<AudioSource> run_slow;

    static const int MAX_POLYGON = 32;
    DirectX::XMFLOAT3					trailPositions[2][MAX_POLYGON];
    Model::Node* node;

    Model::Node* weponNode;

};