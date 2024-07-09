#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"
#include "ProjectileManager.h"
#include "Effect.h"

#include <Audio/Audio.h>
#include <Audio/AudioSource.h>

//�X���C��
class EnemyBOSS : public Enemy
{
public:

    //�C���X�^���X�擾  
    static EnemyBOSS& Instance();

    EnemyBOSS();
    ~EnemyBOSS() override;

    //�X�V����
    void Update(float elapsedTime) override;

    //�`�揈��
    void Render(RenderContext& rc, ModelShader* shader) override;

    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive() override;

    //�꒣��ݒ�
    void SetTerritory(const DirectX::XMFLOAT3& origin, float range);

private:
    //�^�[�Q�b�g�ʒu�������_���ݒ�
    void SetRandomTargetPosition();

    //�v���C���[���G
    bool SearchPlayer();

    //�m�[�h�ƃv���C���[�̏Փˏ���
    void CollisionNodeVsPlayer(const char* nodeName, float nodeRadius);

    //�ڕW�n�_�ֈړ�
    void MoveToTarget(float elapsedTime, float speedRate);

    //�p�j�X�e�[�g�֑J��
    void TransitionWanderState();

    //�p�j�X�e�[�g�X�V����
    void UpdateWanderState(float elapsedTime);

    //�ҋ@�X�e�[�g�֑J��
    void TransitionIdleState();

    //�ҋ@�X�e�[�g�X�V����
    void UpdateIdleState(float elapsedTime);

    //�ڂ̌��J���X�e�[�g�֑J��
    void TransitionEyeState();

    //�ڂ̌��J���X�e�[�g�X�V����
    void UpdateEyeState(float elapsedTime);

    //�ǐՃX�e�[�g�֑J��
    void TransitionPursuitState();

    //�ǐՃX�e�[�g�X�V����
    void UpdatePursuitState(float elapsedTime);

    //�U���X�e�[�g�֑J��
    void TransitionAttackState();

    //�U���X�e�[�g�X�V����
    void UpdateAttackState(float elapsedTime);

    //�퓬�ҋ@�X�e�[�g�֑J��
    void TransitionIdleBattleState();

    //�퓬�ҋ@�X�e�[�g�X�V����
    void UpdateIdleBattleState(float elapsedTime);

    //�_���[�W�X�e�[�g�֑J��
    void TransitionDamageState();

    //�_���[�W�X�e�[�g�X�V����
    void UpdateDamageState(float elapsedTime);

    //���S�X�e�[�g�֑J��
    void TransitionDeathState();

    //���S�X�e�[�g�X�V����
    void UpdateDeathState(float elapsedTime);

    //�G�X�^���v�̏Փˏ���
    void CollisionStampVsPlayers();

    //�G�e�ۂ̏Փˏ���
    void CollisionProjectilesVsPlayers();

    // �͈͍U���̓����蔻��
    void CollisionChargeVsPlayers();

    //�������U���X�e�[�g�֑J��
    void TransitionAttackGun();

    //�������U���X�e�[�g�X�V����
    void UpdateAttackGun(float elapsedTime);

    // �U���ł��p
    void TransitionAttackDemoState();
    // �U���ł��p�X�V
    void UpdateAttackDemoState(float elapsedTime);

    //�X�^���v�U��
    void stamp(const char* nodeName, float nodeRadius);

    //�ʏ�e�U�� 
    void NormalGun(const char* nodeName, float nodeRadius);

    //�͈̓`���[�W�U�� 
    void ChargeGun(const char* nodeName, float nodeRadius);

   // void CollisionPlayerVsFlower();

   

private:
    //�X�e�[�g
    enum class State
    {
        Wander,
        Idle,
        Eye,
        Pursuit,
        Attack,
        AttackGun,
        Damage,
        Death,
        IdleBattle,
        AttackDemo,
    };

    //�A�j���[�V����
    enum Animation
    {
        Anim_Running,           //����
        Anim_Idle,              //�ҋ@
        Anim_AttackCharge,      //�`���[�W�U��
        Anim_Attack_Stamp,      //�X�^���v
        Anim_Eye,               //�ڂ̎�_
        Anim_Attack_NormalGun,         //�m�[�}���K��(�P���e)
        Anim_Death,             //���S
        Anim_GetHit1,           //�q�b�g�P
        //Anim_GetHit2,           //�q�b�g�Q
        Anim_movie              //�{�X�f���p
    };

    enum Attack
    {
        //Attack_stamp = 1,
        Attack_Gun= 1,
        Attack_Charge,
    };



protected:
    //���S�����Ƃ��ɌĂ΂��
    void OnDead() override;

    //�_���[�W���󂯂��Ƃ��ɌĂ΂��
    void OnDamaged() override;

private:

    int damageStamp = 0;
    int damageGun = 0;
    int damageCharge = 0;
    int damage = 0;

    int attackrand = 0;                     // �U�������_���p

    int possession = 0;                     //�A���ˌ���
    
    float speed = 5.0f;
    float lifeTimer = 1.0f;

    // �U������u�Ԃ̃A�j���[�V����
    bool attackCollisionFlag = false;

    // �`���[�W�̃G���A�̑���
    float colosionarea = 0;

    Model* model = nullptr;
    State state = State::Wander;
    DirectX::XMFLOAT3 targetPosition = { 0,0,0 };
    DirectX::XMFLOAT3 territoryOrigin = { 0,0,0 };

    // �R���W�����̈ʒu�X�V
    DirectX::XMFLOAT3 oldposition = { 0,0,0 };

    float territoryRange = 100.0f;
    float moveSpeed = 3.0f;
    float turnSpeed = DirectX::XMConvertToRadians(560);
    float stateTimer = 0.0f;
    float searchRange = 1000000000.0f;
    float attackRange = 45.0f;
    float attack_stampRange = 8.0f;
    ProjectileManager projectileManager;
    
    Effect* damageEffect = nullptr;         // ���ߋ��G�t�F�N�g
    Effect* hitEffect = nullptr;            // �H�炤�G�t�F�N�g
    Effect* chargeEffect = nullptr;         // ���߃G�t�F�N�g
    Effect* smork = nullptr;                // �����G�t�F�N�g

    std::unique_ptr<Effect> changeopen;     // �ڊJ�����
    std::unique_ptr<Effect> changeclose;    // �ڕ�����


    float shotgunpos = 1;

    float                   moveRate = 0.0f;



    int randclass = 0;

    // �t�Đ�����
    bool reverse = false;

    float stoptime = 0;

    std::unique_ptr<AudioSource> stumpaudio;
    std::unique_ptr<AudioSource> chageaudio;
    std::unique_ptr<AudioSource> shotaudio;
    std::unique_ptr<AudioSource> damageaudio;
    std::unique_ptr<AudioSource> hotaudio;
    std::unique_ptr<AudioSource> iceaudio;

    bool chackeaudiostump = false;

    int changemode = 0;
public:
        int num = 0;
};