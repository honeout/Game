#pragma once

#include "Graphics/Model.h"
#include "Player.h"
#include "ProjectileManager.h"
#include "StateMachineRobot.h"

//�A�j���[�V����
enum class RobotAnimation
{
    Attack,            //�U��
    Death,             //���S
    Falling,           //������
    GetHit1,           //�q�b�g�P
    GetHit2,           //�q�b�g�Q
    Idle,              //�ҋ@
    Jump,              //�W�����v
    Jump_Flip,         //
    Landing,           //
    Revive,            //�N���オ��
    Running,           //����
    Walking            //����
};

//���{�b�g
class PlayerRobot : public Player
{
public:
    PlayerRobot();
    ~PlayerRobot() override;


    //�C���X�^���X�擾
    static PlayerRobot& Instance();

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
    //���햢����
    //�ҋ@�X�e�[�g�֑J��
    void TransitionIdleState();

    //�ҋ@�X�e�[�g�X�V����
    void UpdateIdleState(float elapsedTime);

    //�ړ��X�e�[�g�֑J��
    void TransitionMoveState();

    //�ړ��X�e�[�g�X�V����
    void UpdateMoveState(float elapsedTime);

    //�W�����v�X�e�[�g�֑J��
    void TransitionJumpState();

    //�W�����v�X�e�[�g�X�V����
    void UpdateJumpState(float elapsedTime);

    //���n�X�e�[�g�֑J��
    void TransitionLandState();

    //���n�X�e�[�g�X�V����
    void UpdateLandState(float elapsedTIme);

    //�U���X�e�[�g�֑J��
    void TransitionAttackState();

    //�U���X�e�[�g�X�V����
    void UpdateAttackState(float elapsedTime);

    //�_���[�W�X�e�[�g�֑J��
    void TransitionDamageState();

    //�_���[�W�X�e�[�g�X�V����
    void UpdateDamageState(float elapsedTime);

    //���S�X�e�[�g�֑J��
    void TransitionDeathState();

    //���S�X�e�[�g�X�V����
    void UpdateDeathState(float elapsedTime);

    //�����X�e�[�g�֑J��
    void TransitionReviveState();

    //�����X�e�[�g�X�V����
    void UpdateReviveState(float elapsedTime);

    //����X�e�[�g�֑J��
    void TransitionDodgeState();

    //����X�e�[�g�X�V����
    void UpdateDodgeState(float elapsedTime);

    //�����X�e�[�g�֑J��
    void TransitionWalkState();

    //�����X�e�[�g�X�V����
    void UpdateWalkState(float elapsedTime);

    //����X�e�[�g�֑J��
    void TransitionRunState();

    //����X�e�[�g�X�V����
    void UpdateRunState(float elapsedTime);

    //�K�[�h�X�e�[�g�֑J��
    void TransitionGuardState();

    //�K�[�h�X�e�[�g�X�V����
    void UpdateGuardState(float elapsedTime);

    //����ύX�X�e�[�g�֑J��
    void TransitionChangeState();

    //����ύX�X�e�[�g�X�V����
    void UpdateChangeState(float elapsedTime);

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

    //�W�����v�X�e�[�g�֑J��
    void TransitionJump_GreatState();

    //�W�����v�X�e�[�g�X�V����
    void UpdateJump_GreatState(float elapsedTime);

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

    //����X�e�[�g�֑J��
    void TransitionDodge_GreatState();

    //����X�e�[�g�X�V����
    void UpdateDodge_GreatState(float elapsedTime);

    //�K�[�h�X�e�[�g�֑J��
    void TransitionGuard_GreatState();

    //�K�[�h�X�e�[�g�X�V����
    void UpdateGuard_GreatState(float elapsedTime);

    //����ύX�X�e�[�g�J��
    void TransitionChange_GreatState();

    //����ύX�X�e�[�g�X�V����
    void UpdateChange_GreatState(float elapsedTime);

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

    //�W�����v�X�e�[�g�֑J��
    void TransitionJump_DualState();

    //�W�����v�X�e�[�g�X�V����
    void UpdateJump_DualState(float elapsedTime);

    //���n�X�e�[�g�֑J��
    void TransitionLand_DualState();

    //���n�X�e�[�g�X�V����
    void UpdateLand_DualState(float elapsedTIme);

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

    //�W�����v�X�e�[�g�֑J��
    void TransitionJump_GunState();

    //�W�����v�X�e�[�g�X�V����
    void UpdateJump_GunState(float elapsedTime);

    //���n�X�e�[�g�֑J��
    void TransitionLand_GunState();

    //���n�X�e�[�g�X�V����
    void UpdateLand_GunState(float elapsedTIme);

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

    

private:
    //�X�e�[�g
    enum class State
    {
        //���햢����
        Idle,        //�ҋ@�X�e�[�g
        Move,        //�ړ��X�e�[�g
        Jump,        //�W�����v�X�e�[�g
        Land,        //���n�X�e�[�g
        Attack,      //�U���X�e�[�g
        Damage,      //�_���[�W�X�e�[�g
        Death,       //���S�X�e�[�g
        Revive,      //�����X�e�[�g
        Dodge,       //����X�e�[�g
        Walk,        //�����X�e�[�g
        Run,         //����X�e�[�g
        Guard,       //�K�[�h�X�e�[�g
        Change,      //����ύX�X�e�[�g

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
    };

private:
    //�A�j���[�V����
    enum Animation
    {
        Anim_Attack,            //�U��
        Anim_Death,             //���S
        Anim_Falling,           //������
        Anim_GetHit1,           //�q�b�g�P
        Anim_GetHit2,           //�q�b�g�Q
        Anim_Idle,              //�ҋ@
        Anim_Jump,              //�W�����v
        Anim_Jump_Flip,         //
        Anim_Landing,           //
        Anim_Revive,            //�N���オ��
        Anim_Running,           //����
        Anim_Walking            //����
    };

    //�o���A�j���[�V����
    enum DualSwordAnimation
    {
        Anim_Dual_Idle, //�ҋ@
        Anim_Dual_Dash, //�_�b�V��
    };

    //�匕�A�j���[�V����
    enum GreatSwordAnimation
    {
        Anim_Great_Idle,    //�ҋ@
        Anim_Great_Dash,    //�_�b�V��
    };

    //�e�A�j���[�V����
    enum GunAnimation
    {
        Anim_Gun_Idle,  //�ҋ@
        Anim_Gun_Dash,  //�_�b�V��
    };

    //public:
    //    //�X�e�[�g�penum�N���X
    //    enum class Robot_State
    //    {
    //        Robot_Idle,           //�ҋ@
    //        Robot_Walk,           //����
    //        Robot_Run,            //����
    //        Robot_Dodge,          //���
    //        Robot_Jump,           //�W�����v
    //        Robot_Land,           //���n
    //        Robot_Attack,         //�U��
    //        Robot_Damage,         //�_���[�W
    //        Robot_Dead,           //���S
    //    };


protected:
    //���n�����Ƃ��ɌĂ΂��
    void OnLanding() override;

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
    State state = State::Idle;      //�X�e�[�g
    float leftHandRadius = 0.4f;
    bool attackCollisionFlag = false;
    int statenumber = 0;
    int weapon = 0; //����p�ϐ�
    float changetime = 0.0f;
    int attackcount = 0;
    int bullets = 60;   //�c�e��
    float reloadtime = 0.0f;
    int totalattackcount = 0;

  
  

    //StateMachine�����p
    //Robot_State robotstate = Robot_State::Robot_Idle;   //�����X�e�[�g
    //StateMachineRobot* stateMachineRobot = nullptr;   //StateMachine�̐���
};