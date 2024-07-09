#pragma once

#include "Graphics/Model.h"
#include "Player.h"
#include "ProjectileManager.h"
#include "StateMachineRobot.h"
#include "Audio/Audio.h"
#include "shiled.h"

//���{�b�g
class PlayerGreat : public Player
{
public:
    PlayerGreat();
    ~PlayerGreat() override;

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

    ////�e�ۓ��͏���
    //void InputProjectile();

    //�U�����͏���
    bool InputAttack();

    //����ύX
    bool InputWeapon();

    //�K�[�h���͏���
    bool InputGuard(float elapsedTime);

    ////�����[�h���͏���
    //bool InputReload();

private:
    //�匕 GreatSword
    //�ҋ@�X�e�[�g�֑J��
    void TransitionIdle_GreatState();

    //�ҋ@�X�e�[�g�X�V����
    void UpdateIdle_GreatState(float elapsedTime);

    //�����X�e�[�g�֑J��
    void TransitionWalk_GreatState();

    //�����X�e�[�g�X�V����
    void UpdateWalk_GreatState(float elapsedTime);

    //����X�e�[�g�֑J��
    void TransitionRun_GreatState();

    //����X�e�[�g�X�V����
    void UpdateRun_GreatState(float elapsedTime);

    ////�W�����v�X�e�[�g�֑J��
    //void TransitionJump_GreatState();

    ////�W�����v�X�e�[�g�X�V����
    //void UpdateJump_GreatState(float elapsedTime);

    //���n�X�e�[�g�֑J��
    void TransitionLand_GreatState();

    //���n�X�e�[�g�X�V����
    void UpdateLand_GreatState(float elapsedTIme);

    //�U���X�e�[�g�֑J��
    void TransitionAttack_GreatState();

    //�U���X�e�[�g�X�V����
    void UpdateAttack_GreatState(float elapsedTime);

    //�_���[�W�X�e�[�g�֑J��
    void TransitionDamage_GreatState();

    //�_���[�W�X�e�[�g�X�V����
    void UpdateDamage_GreatState(float elapsedTime);

    //���S�X�e�[�g�֑J��
    void TransitionDeath_GreatState();

    //���S�X�e�[�g�X�V����
    void UpdateDeath_GreatState(float elapsedTime);

    ////����X�e�[�g�֑J��
    //void TransitionDodge_GreatState();

    ////����X�e�[�g�X�V����
    //void UpdateDodge_GreatState(float elapsedTime);

    //�K�[�h�X�e�[�g�֑J��
    void TransitionGuard_GreatState();

    //�K�[�h�X�e�[�g�X�V����
    void UpdateGuard_GreatState(float elapsedTime);

    //����ύX�X�e�[�g�J��
    void TransitionChange_GreatState();

    //����ύX�X�e�[�g�X�V����
    void UpdateChange_GreatState(float elapsedTime);

    //�u���[�L�X�e�[�g�J��
    void TransitionBrake_GreatState();

    //�u���[�L�X�e�[�g�X�V����
    void UpdateBrake_GreatState(float elapsedTime);

private:
    //�X�e�[�g
    enum class State
    {
        //�匕 GreaatSword
        Idle_Great, //�ҋ@
        Walk_Great, //����
        Run_Great,  //����
        Jump_Great, //�W�����v
        Land_Great, //���n
        Attack_Great, //�U��
        Damage_Great, //�_���[�W
        Death_Great, //���S
        Dodge_Great, //���
        Guard_Great, //�K�[�h
        Change_Great, //����ύX
        Brake_Great,
    };

private:
    //�匕�A�j���[�V����
    enum GreatSwordAnimation
    {
        Anim_Great_Idle, //�ҋ@
        Anim_Great_Dash, //�_�b�V��
        Anim_Great_Attack_1,   //�U��
        Anim_Great_Attack_2,
        Anim_Great_Attack_3,
        Anim_Great_Damaged,
        Anim_Great_Guard,
        Anim_Great_Guard_Damaged,
        Anim_Great_Brake,
        Anim_Great_Dodge,    //���
        Anim_Great_Change,   //����ύX
        Anim_Great_GetHit,   //�q�b�g
        Anim_Great_Death,    //���S
        Anim_Great_Jump,     //�W�����v
        Anim_Great_Falling,  //�����Ă���
        Anim_Great_Landing,  //���n
    };
protected:
    ////���n�����Ƃ��ɌĂ΂��
    //void OnLanding() override;

    //���S�����Ƃ��ɌĂ΂��
    void OnDead() override;

    //�_���[�W���󂯂��Ƃ��ɌĂ΂��
    void OnDamaged() override;

private:
    Model* model = nullptr;
    Shiled* weaponModel = nullptr;
    float moveSpeed = 5.0f;
    float turnSpeed = DirectX::XMConvertToRadians(720);
    float jumpSpeed = 20.0f;
    int jumpCount = 0;      //�����񂾂�
    int jumpLimit = 1;      //�W�����v�񐔐���
    ProjectileManager projectileManager;
    Effect* hitEffect = nullptr;
    Effect* invincible_hitEffect = nullptr;
    Effect* guardmode = nullptr;
    State state = State::Idle_Great;      //�X�e�[�g
    float greatRadius = 0.8f;
    bool attackCollisionFlag = false;
    bool attackCollisionFlagLast = false;
    int statenumber = 0;
    int weapon = 0; //����p�ϐ�
    float changetime = 0.0f;
    int attackcount = 0;
    int bullets = 60;   //�c�e��
    float reloadtime = 0.0f;
    int totalattackcount = 0;
    float attacktime = 0.0f;
    float attackrate = 0.0f;

    // �K�[�h�̎���
    int guardtime;

    std::unique_ptr<AudioSource> attack_01;
    std::unique_ptr<AudioSource> attack_02;
    std::unique_ptr<AudioSource> attack_03;
    std::unique_ptr<AudioSource> guard;
    std::unique_ptr<AudioSource> damaged;
    std::unique_ptr<AudioSource> brake;
    std::unique_ptr<AudioSource> run_fast;
    std::unique_ptr<AudioSource> run_slow;
};