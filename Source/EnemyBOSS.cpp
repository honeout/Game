#include "EnemyBOSS.h"
#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "Player.h"
#include "PlayerManager.h"
#include "Collision.h"
#include "ProjectileStraite.h"
#include "ProjectileHoming.h"
#include "Input/Input.h"
#include "Camera.h"
#include "CameraController.h"
#include "EnemyManager.h"
#include "PlayerRobot.h"
#include "EnemeyNormalGun.h"
#include "EnemySkillGunCharge.h"
#include "EnemySkillGun.h"
#include "Projectile.h"
#include "UI.h"

static EnemyBOSS* instance = nullptr;

EnemyBOSS& EnemyBOSS::Instance()
{
    return *instance;
}

//�R���X�g���N�^
EnemyBOSS::EnemyBOSS()
{
    //model = new Model("Data/Model/BOSS/Boss.mdl");
    model = new Model("Data/Model/BOSS/BossAnim8.mdl");

    //���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.y = scale.z = 0.09f;
   
    //���A�����ݒ�
    radius = 2.9f;
    height = 10.0f;

    health = 100;
    //health = 1;

    //damageEffect = new Effect("Data/Effect/lightning ballgun boss.efk");
    //hitEffect = new Effect("Data/Effect/lightning stricked.efk");
    chargeEffect = new Effect("Data/Effect/bossCharge.efk");
    smork = new Effect("Data/Effect/stageSmork.efk");
    
    changeopen = std::make_unique<Effect>("Data/Effect/eyeopen.efk");
    changeclose = std::make_unique<Effect>("Data/Effect/eyeclose.efk");

    // damage
    damageStamp = 3;
    damageGun = 1;
    damageCharge = 5;

    camerachange = true;

    fadeoutcamera = false;

    EnemyManager::Instance().SetDie(false);

    animationChange = AnimationChange::Normal;

    startboss = false;

    stoptime = 0;

    stumpaudio = Audio::Instance().LoadAudioSource("Data/Audio/Se/Enemy/Enemy_Stomp.wav");
    //chageaudio = Audio::Instance().LoadAudioSource("Data/Audio/Se/Enemy/Enemy_Stomp.wav");
    shotaudio = Audio::Instance().LoadAudioSource("Data/Audio/Se/Enemy/Enemy_Roar.wav");
    damageaudio = Audio::Instance().LoadAudioSource("Data/Audio/Se/Enemy/Enemy_Footsteps.wav");
    hotaudio = Audio::Instance().LoadAudioSource("Data/Audio/Se/Enemy/Juwel_Generation02.wav");
    iceaudio = Audio::Instance().LoadAudioSource("Data/Audio/Se/Enemy/Juwel_Generation01.wav");

    // �{�X�̏�ԕϊ�
    changemode = 0;

    //�p�j�X�e�[�g�֑J��
    //TransitionWanderState();
    TransitionAttackDemoState();
}

//�f�X�g���N�^
EnemyBOSS::~EnemyBOSS()
{
    //delete damageEffect;
    //delete hitEffect;

    smork->stop(smork->GetHandle());
    chargeEffect->stop(chargeEffect->GetHandle());

    changeopen->stop(changeopen->GetHandle());
    changeclose->stop(changeclose->GetHandle());

    delete chargeEffect;
    delete model;
    delete smork;
}

//�X�V����
void EnemyBOSS::Update(float elapsedTime)
{
    //velocity�Ƃ����K�v�Ȃ�O�ɂ���
    //���@�̕�
    if (position.x >= 45.0f)
    {
        position.x = 45.0f;
    }
    if (position.x <= -45.0f)
    {
        position.x = -45.0f;
    }
    if (position.z >= 45.0f)
    {
        position.z = 45.0f;
    }
    if (position.z <= -45.0f)
    {
        position.z = -45.0f;
    }


    //�X�e�[�g���̍X�V����
    switch (state)
    {
    case State::Wander:
        UpdateWanderState(elapsedTime);
        break;

    case State::Idle:
        UpdateIdleState(elapsedTime);
        break;

    case State::Eye:
        UpdateEyeState(elapsedTime);
        break;

    case State::Pursuit:
        UpdatePursuitState(elapsedTime);
        break;

    case State::Attack:
        UpdateAttackState(elapsedTime);
        break;

    case State::AttackGun:
        UpdateAttackGun(elapsedTime);
        break;

    case State::IdleBattle:
        UpdateIdleBattleState(elapsedTime);
        break;

    case State::Damage:
        UpdateDamageState(elapsedTime);
        break;

    case State::Death:
        UpdateDeathState(elapsedTime);
        break;

    case State::AttackDemo:
        UpdateAttackDemoState(elapsedTime);
        break;
    }

    //���͍X�V����
    UpdateVelocity(elapsedTime);

    projectileManager.Update(elapsedTime);



    switch (attackrand)
    {
        // �ʏ�U���p�����蔻��
    //case Attack::Attack_stamp:
    //    damage = damageStamp;
    //    CollisionStampVsPlayers();
    //    break;
        // �P���e�U���p�����蔻��
    case Attack::Attack_Gun:
        damage = damageGun;

        CollisionProjectilesVsPlayers();
        break;
        // ��Z�U���p�����蔻��
    case Attack::Attack_Charge:
        damage = damageCharge;
        CollisionChargeVsPlayers();
        break;
    }



    //���G���ԍX�V
    UpdateInvincibleTimer(elapsedTime);

    //�e�ۍX�V����

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();


    switch (animationChange)
    {
    case AnimationChange::Normal:

        //���f���A�j���[�V�����X�V
        model->UpdateAnimation(elapsedTime);


        animationConfirmation = false;
        break;
    case AnimationChange::doble:
        if (!reverse)
        {
            animationConfirmation = true;

            // �㔼�g�p�A�j���[�V����
            model->UpdateUpeerBodyAnimation(elapsedTime, "body2");
            // �����g�p�A�j���[�V����
            model->UpdateLowerBodyAnimation(elapsedTime, "body2");
        }

        if (reverse)
            model->ReverseplaybackAnimation(elapsedTime);
        break;
    }


    //switch (animationChange)
    //{
    //case AnimationChange::Normal:
    //    if (!reverse)
    //    {
    //        //���f���A�j���[�V�����X�V
    //        model->UpdateAnimation(elapsedTime);
    //    }
    //    if (reverse)
    //        model->ReverseplaybackAnimation(elapsedTime);
    //    animationConfirmation = false;
    //    break;
    //case AnimationChange::doble:
    //    if (!reverse)
    //    {
    //        animationConfirmation = true;

    //        // �㔼�g�p�A�j���[�V����
    //        model->UpdateUpeerBodyAnimation(elapsedTime, "body2");
    //        // �����g�p�A�j���[�V����
    //        model->UpdateLowerBodyAnimation(elapsedTime, "body2");
    //    }

   
    //    break;
    //}

    //switch (animationChange)
    //{
    //case AnimationChange::Normal:
    //{
    //    // �t�Đ�
    //    if (reverse)
    //    {
    //       // model->ReverseplaybackAnimation(elapsedTime);
    //        model->ReverseplaybackAnimation(elapsedTime);
    //    }
    //    // �Đ�
    //    else
    //    {
    //        //���f���A�j���[�V�����X�V
    //        model->UpdateAnimation(elapsedTime);
    //    }


    //    animationConfirmation = false;
    //    break;
    //}
    //case AnimationChange::doble:
    //{
    //    //if (!reverse)
    //    //{
    //    animationConfirmation = true;

    //    // �㔼�g�p�A�j���[�V����
    //    model->UpdateUpeerBodyAnimation(elapsedTime, "body2");
    //    // �����g�p�A�j���[�V����
    //    model->UpdateLowerBodyAnimation(elapsedTime, "body2");


    //    break;
    //}
    //}



    //model->IsPlayUpeerBodyAnimation(elapsedTime);

    //���f���s��X�V
    model->UpdateTransform(transform);

    //CollisionStampVsPlayers();

    //CollisionProjectilesVsPlayers();

    //CollisionChargeVsPlayers();

}

//�`�揈��
void EnemyBOSS::Render(RenderContext& rc, ModelShader* shader)
{
    rc.toonData.edgeWidth = 0.1f;
    shader->Draw(rc, model);

    //�e�ە`�揈��
    projectileManager.Render(rc, shader);
}

//�f�o�b�O�v���~�e�B�u�`��
void EnemyBOSS::DrawDebugPrimitive()
{
    //���N���X�̃f�o�b�O�v���~�e�B�u�`��
    Enemy::DrawDebugPrimitive();

    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //�꒣��͈͂��f�o�b�O�~���`��
    debugRenderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f, DirectX::XMFLOAT4(0, 1, 0, 1));

    //�^�[�Q�b�g�ʒu���f�o�b�O����`��
    debugRenderer->DrawSphere(targetPosition, radius, DirectX::XMFLOAT4(1, 1, 0, 1));

    //���G�͈͂��f�o�b�O�~���`��
    debugRenderer->DrawCylinder(position, searchRange, 1.0f, DirectX::XMFLOAT4(0, 0, 1, 1));

    //�U���͈͂��f�o�b�O�~���`��
    debugRenderer->DrawCylinder(position, attackRange, 1.0f, DirectX::XMFLOAT4(1, 0, 0, 1));

    //�X�^���v�U���͈͂��f�o�b�O�~���`��
    debugRenderer->DrawCylinder(position, attack_stampRange, 1.0f, DirectX::XMFLOAT4(1, 0, 1, 1));
}

//�꒣��ݒ�
void EnemyBOSS::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
    territoryOrigin = origin;
    territoryRange = range;
}

//�^�[�Q�b�g�ʒu�������_���ݒ�
void EnemyBOSS::SetRandomTargetPosition()
{
    float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
    float range = Mathf::RandomRange(0.0f, territoryRange);
    targetPosition.x = territoryOrigin.x + sinf(theta) * range;
    targetPosition.y = territoryOrigin.y;
    targetPosition.z = territoryOrigin.z + cosf(theta) * range;
}

//�v���C���[���G
bool EnemyBOSS::SearchPlayer()
{
    //�v���C���[�Ƃ̍��፷���l�����ĂRD�ł̋������������
    PlayerManager& playerMgr = PlayerManager::Instance();
    const DirectX::XMFLOAT3& playerPosition = playerMgr.Instance().GetPlayer(playerMgr.GetWeapon())->GetPosition();
    
    //BOSS��������͈� 1000
    float vx = playerPosition.x - position.x;
    float vy = playerPosition.y - position.y;
    float vz = playerPosition.z - position.z;
    float dist = sqrtf(vx * vx + vy * vy + vz * vz);

    if (dist < searchRange)
    {
        float distXZ = sqrtf(vx * vx + vz * vz);
        //�P�ʃx�N�g����
        vx /= distXZ;
        vz /= distXZ;
        //�S�|�x�N�g��
        float frontX = sinf(angle.y);
        float frontZ = cosf(angle.y);
        //�Q�p���N�I���̓��ϒn�őO�㔻��
        float dot = (frontX * vx) + (frontZ * vz);
        if (dot > 0.0f)
        {
            return true;
        }
    }

    return false;
}

//�m�[�h�ƃv���C���[�̏Փˏ���
void EnemyBOSS::CollisionNodeVsPlayer(const char* nodeName, float nodeRadius)
{
    //�m�[�h�̈ʒu�Ɠ����蔻����s��
    Model::Node* node = model->FindNode(nodeName);
    
    if (node != nullptr)
    {
        //�m�[�h�̃��[���h���W
        DirectX::XMFLOAT3 nodePosition(
            node->worldTransform._41,
            node->worldTransform._42,
            node->worldTransform._43
        );
        
        //�����蔻��\��
        Graphics::Instance().GetDebugRenderer()->DrawSphere(
            nodePosition, nodeRadius, DirectX::XMFLOAT4(0, 0, 1, 1)
        );

        //�v���C���[�Ɠ����蔻��
        Player* player = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetWeapon());
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectSphereVsCylinder(
            nodePosition,
            nodeRadius,
            player->GetPosition(),
            player->GetRadius(),
            player->GetHeight(),
            outPosition))
        {
            //�_���[�W��^����
            if (player->ApplyDamage(1, 5.0f))
            {
                //�G���ӂ��Ƃ΂��x�N�g�����Z�o
                DirectX::XMFLOAT3 vec;
                vec.x = outPosition.x - nodePosition.x;
                vec.z = outPosition.z - nodePosition.z;
                float length = sqrtf(vec.x * vec.x + vec.z * vec.z);
                vec.x /= length;
                vec.z /= length;
                
                //XZ���ʂɂӂ��Ƃ΂��͂�������
                float power = 20.0f;
                vec.x *= power;
                vec.z *= power;

                //Y�����ɂ��͂�������
                vec.y = 5.0f;

                //������΂�
                player->AddImpulse(vec);
            }
        }
    }
}

//�ڕW�n�_�ֈړ�
void EnemyBOSS::MoveToTarget(float elapsedTime, float speedRate)
{
    //�^�[�Q�b�g�����ւ̐i�s�x�N�g�����Z�o
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vz * vz);
    vx /= dist;
    vz /= dist;

    //�ړ�����
    Move(vx, vz, moveSpeed * speedRate);
    Turn(elapsedTime, vx, vz, turnSpeed * speedRate);
}

//�p�j�X�e�[�g�֑J��
void EnemyBOSS::TransitionWanderState()
{
    state = State::Wander;

    //�ڕW�n�_�ݒ�
    SetRandomTargetPosition();

    //static_cast<int>(animationChange);

    //�����A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Running, true);
}

//�p�j�X�e�[�g�X�V����
void EnemyBOSS::UpdateWanderState(float elapsedTime)
{
    //�ڕW�n�_�܂�XZ���ʂł̋�������
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float distSq = vx * vx + vz * vz;
    if (distSq < radius * radius)
    {
        shotTimer = 0;
        //�ҋ@�X�e�[�g�֑J��
        TransitionIdleState();
    }

    //�ڕW�n�_�ֈړ�
    MoveToTarget(elapsedTime, 0.5f);

    //�v���C���[���G
    if (SearchPlayer())
    {
        //����������ǐՃX�e�[�g�֑J��
        TransitionPursuitState();
    }
}

//�ҋ@�X�e�[�g�֑J��
void EnemyBOSS::TransitionIdleState()
{
    if (state != State::Eye)
    {
        randclass = rand() % 2;

        if (randclass == 1)
        {
            TransitionEyeState();
           
        }
    }
    if (randclass != 1)
    {

        state = State::Idle;



        //�^�C�}�[�������_���ݒ�
        stateTimer = Mathf::RandomRange(3.0f, 5.0f);

        //�ҋ@�A�j���[�V�����Đ�
        model->PlayAnimation(Anim_Idle, true);

        model->PlayUpeerBodyAnimation(Anim_Idle, true);

        shotgunpos = 1;

        // �͈͍U���`���[�W������
        chargetime = 0;
    }
    /*randclass = rand() % 5;*/

    //if (randclass == 1)
    //    TransitionEyeState();

    //attackrand = rand() % 3 + 1;

}

//�ҋ@�X�e�[�g�X�V����
void EnemyBOSS::UpdateIdleState(float elapsedTime)
{
    // �ŏ��̑����h�~
    if (UI::Instance().GetTime() >= 10500) return;
    stoptime += elapsedTime;

    //�^�C�}�[����
    stateTimer -= elapsedTime;

    if (stateTimer < 0.0f)
    {
        shotstate = false;

        possession = 0;

        //�p�j�X�e�[�g�֑J��
        TransitionWanderState();
    }

    if (!shotstate)
        //�v���C���[���G
        if (SearchPlayer())
        {
            //����������ǐՃX�e�[�g�֑J��
            TransitionPursuitState();
        }



    // �������񂾂�
    if (projectileManager.GetProjectileCount() == 0)
    {
        //TransitionEyeState();
        attackrand = 0;
    }

    // �����U��������������͈͍U����������
    if (attackrand == 0)
    {
        colosionarea = 0;
    }
    
}

void EnemyBOSS::TransitionEyeState()
{
    state = State::Eye;
    model->PlayAnimation(Anim_Eye, false);
    // �t�Đ�
    if (animationChange == AnimationChange::Normal)
    {
        changeopen->Play(position);
      
        reverse = false;
        //return;
    }
    // �Đ�
    if (animationChange == AnimationChange::doble)
    {
        changeclose->Play(position);
   
        reverse = true;
        //return;
    }
    
    // �Đ� ����
    //if (animationChange == AnimationChange::Normal)
    //{
    //    changeopen->Play(position);
    // 
    //    changemode = 1;
    //    reverse = false;
    //    model->PlayAnimation(Anim_Eye, false);
    //    
    //}
    //// �t���Đ� �J��
    //if (animationChange == AnimationChange::doble)
    //{
    //    changemode = 0;

    //    changeclose->Play(position);
    //    reverse = true;
    //    animationChange = AnimationChange::Normal;
    //    model->PlayAnimation(Anim_Eye, false);
    //}

    //    // �t�Đ�
    //if (animationChange == AnimationChange::Normal)
    //{
    //    changeopen->Play(position);

    //    reverse = true;

    //    return;
    //}
    //// �Đ�
    //if (animationChange == AnimationChange::doble)
    //{
    //    changeclose->Play(position);
    //    animationChange = AnimationChange::Normal;
    //    reverse = false;
    //    return;
    //}
}

void EnemyBOSS::UpdateEyeState(float elapsedTime)
{

    if (!model->IsPlayAnimation() && reverse == true)
    {
        changeclose->stop(changeclose->GetHandle());
        animationChange = AnimationChange::Normal;
        reverse = false;
        randclass = 0;
        TransitionIdleState();
    }
    if (!model->IsPlayAnimation() && reverse == false)
    {
        animationChange = AnimationChange::doble;
        changeopen->stop(changeopen->GetHandle());
        reverse = false;
        randclass = 0;
        TransitionIdleState();
    }


    //if (!model->IsPlayAnimation() && changemode == 1)
    //{

    //    reverse = false;
    //    changeopen->stop(changeopen->GetHandle());
    //    animationChange = AnimationChange::doble;
    //    
    //    TransitionIdleState();

    //}

    //if (!model->IsPlayAnimation() && changemode == 0)
    //{

    //    reverse = false;
    //    changeclose->stop(changeclose->GetHandle());

    //    TransitionIdleState();

    //}


    //if (!model->IsPlayAnimation() && reverse == false)
    //{
    //    //animationChange = AnimationChange::doble;
    //    changeclose->stop(changeclose->GetHandle());
    //    reverse = false;
    //    TransitionIdleState();
    //}

    //if (!model->IsPlayAnimation() && reverse == true)
    //{

    //    changeopen->stop(changeopen->GetHandle());
    //    reverse = false;
    //    animationChange = AnimationChange::doble;
    //    TransitionIdleState();
    //}
}

//�ǐՃX�e�[�g�֑J��
void EnemyBOSS::TransitionPursuitState()
{
    state = State::Pursuit;

    //���b�ԒǐՂ���^�C�}�[�������_���ݒ�
    stateTimer = Mathf::RandomRange(3.0f, 5.0f);

    //�����A�j���[�V�������Đ�
    model->PlayAnimation(Anim_Running, true);

    model->PlayUpeerBodyAnimation(Anim_Eye, true);
}

//�ǐՃX�e�[�g�X�V����
void EnemyBOSS::UpdatePursuitState(float elapsedTime)
{

    
        PlayerManager& playerMgr = PlayerManager::Instance();

        //�ڕW�n�_���v���C���[�ʒu�ɐݒ�
        targetPosition = playerMgr.Instance().GetPlayer(playerMgr.GetWeapon())->GetPosition();

        //�ڕW�n�_�ֈړ�
        MoveToTarget(elapsedTime, 1.0f);

        //�^�C�}�[����
        stateTimer -= elapsedTime;
        if (stateTimer < 0.0f)
        {
            //�ҋ@�X�e�[�g�֑J��
            TransitionIdleState();
        }

        //�v���C���[�ɋ߂Â��ƍU���X�e�[�g�֑J��
        float vx = targetPosition.x - position.x;
        float vy = targetPosition.y - position.y;
        float vz = targetPosition.z - position.z;
        float dist = sqrtf(vx * vx + vy * vy + vz * vz);

        if (dist < attack_stampRange)
        {
            //�U���X�e�[�g�֑J��
            TransitionAttackState();
        }
        else if (dist < attackRange)
        {
            // �ˌ��X�e�[�g�֑J��
            TransitionAttackGun();
        }

        if (shotstate)
        {
            TransitionIdleState();
        }
    
}

//�U���X�e�[�g�֑J��
void EnemyBOSS::TransitionAttackState()
{
    state = State::Attack;

   // //�U���A�j���[�V�����Đ�
   //model->PlayAnimation(Anim_AttackCharge, false);

   //model->PlayUpeerBodyAnimation(Anim_AttackCharge, false);
 
    //state = State::Attack;

    ////�U���A�j���[�V�����Đ�
    //model->PlayAnimation(Anim_Attack_Stamp, false);

    //model->PlayUpeerBodyAnimation(Anim_Attack_Stamp, false);
 
    //if (attackrand == Attack::Attack_stamp)
    //{
    model->PlayAnimation(Anim_Attack_Stamp, false);
    model->PlayUpeerBodyAnimation(Anim_Attack_Stamp, false);
    
    chackeaudiostump = false;

}

//�U���X�e�[�g�X�V����
void EnemyBOSS::UpdateAttackState(float elapsedTime)
{
    ////�C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
    //float animationTime = model->GetCurrentAnimationSeconds();
    //if (animationTime >= 1.0f && animationTime <= 5.00f)
    //{
    //    // �ڋʃm�[�h�ƃv���C���[�̏Փˏ���
    //    CollisionNodeVsPlayer("boss_left_foot1", 5.0f);
    //}
    //{

    //   // NormalGun("blend_root", 2.5f);
    //}
    //�t�����
    ////�U���A�j���[�V�������I�������퓬�ҋ@�X�e�[�g�֑J��
    //if (!model->IsPlayAnimation())
    //{
    //    TransitionIdleBattleState();
    //}
        // �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
    //float animationTime = model->GetCurrentAnimationSeconds();
    //if (animationTime >= 0.1f && animationTime <= 0.35f)
    //{
    //    // �ڋʃm�[�h�ƃv���C���[�̏Փˏ���
    //    CollisionNodeVsPlayer("EyeBall", 4.0f);
    //}

    //// �U���A�j���[�V�������I�������퓬�ҋ@�X�e�[�g�֑J��
    //if (!model->IsPlayAnimation())
    //{
    //    TransitionIdleBattleState();
    //}

    //Attack::Attack_stamp:
    //�C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
    float animationTime = model->GetCurrentAnimationSeconds();
    attackCollisionFlag = animationTime >= 1.0f && animationTime <= 3.0f;
    if (attackCollisionFlag)
    {
        //attack_stampRange;
        stamp("boss_left_foot1", 5.0f);
    }
   
    if (animationTime >= 1.0f && animationTime <= 1.1f && !chackeaudiostump)
    {
        stumpaudio->Play(false, 1.0f);
        chackeaudiostump = true;
    }
    CollisionStampVsPlayers();

}

//�퓬�ҋ@�X�e�[�g�֑J��
void EnemyBOSS::TransitionIdleBattleState()
{
    state = State::IdleBattle;

    //���b�ԑҋ@����^�C�}�[�������_���ݒ�
    stateTimer = Mathf::RandomRange(3.0f, 3.0f);

    shotstate = false;

    //�퓬�ҋ@�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Idle, true);

    model->PlayUpeerBodyAnimation(Anim_Idle, true);

    shotgunpos = 1;

    // �͈͍U���`���[�W������
    chargetime = 0;

    // �������񂾂�
    if (projectileManager.GetProjectileCount() == 0)
    {
        attackrand = 0;
    }

    //randclass = rand() % 2;

    //if (randclass == 1)
    //    TransitionEyeState();
}

//�퓬�ҋ@�X�e�[�g�X�V����
void EnemyBOSS::UpdateIdleBattleState(float elapsedTime)
{
    //�ڕW�n�_���v���C���[�ʒu�ɐݒ�
    targetPosition = PlayerManager::Instance().GetPlayer(0)->GetPosition();

    if (projectileManager.GetProjectileCount() == 0)
    {
        attackrand = 0;
    }

    //�^�C�}�[����
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        //�v���C���[���U���͈͂ɂ����ꍇ�͍U���X�e�[�g�֑J��
        float vx = targetPosition.x - position.x;
        float vy = targetPosition.y - position.y;
        float vz = targetPosition.z - position.z;
        float dist = sqrtf(vx * vx + vy * vy + vz * vz);
        if (dist < attackRange)
        {
            //�U���X�e�[�g�֑J��
            TransitionAttackGun();


        }
        if (dist < attack_stampRange)
        {
            TransitionAttackState();
        }
        else
        {
            //�p�j�X�e�[�g�֑J��
            TransitionWanderState();
        }

        //if (dist < attack_stampRange)
        //{
        //    TransitionAttackState();
        //}
        //else
        //{
        //    //�p�j�X�e�[�g�֑J��
        //    TransitionWanderState();
        //}
    }
    MoveToTarget(elapsedTime, 0.0f);
    
    if (attackrand == 0)
    {
        colosionarea = 0;
    }
}


//�_���[�W�X�e�[�g�֑J��
void EnemyBOSS::TransitionDamageState()
{
/*    if (chargetime < chargetimeMax)
    {
      */  // �������񂾂�
    if (projectileManager.GetProjectileCount() == 0)
    {
        attackrand = 0;
    }

   // }
    state = State::Damage;

    //�_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_GetHit1, false);
    model->PlayUpeerBodyAnimation(Anim_GetHit1, false);
}

//�_���[�W�X�e�[�g�X�V����
void EnemyBOSS::UpdateDamageState(float elapsedTime)
{
    //�_���[�W�A�j���[�V�������I�������퓬�ҋ@�X�e�[�g�֑J��
    if (!model->IsPlayAnimation())
    {
        TransitionIdleBattleState();
    }
}

//���S�X�e�[�g�֑J��
void EnemyBOSS::TransitionDeathState()
{
    state = State::Death;

    //�_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Death, false);
}

//���S�X�e�[�g�X�V����
void EnemyBOSS::UpdateDeathState(float elapsedTime)
{
    //�_���[�W�A�j���[�V�������I������玩���𗚂�
    if (!model->IsPlayAnimation())
    {
        EnemyManager::Instance().SetDie(true);
        Destroy();
    }
}


// �X�^���v�ƃv���C���[�̓����蔻��
void EnemyBOSS::CollisionStampVsPlayers()
{
    // �͈͍U�����a������
    colosionarea = 0;

    // �v���C���[
    PlayerManager& playerMgr = PlayerManager::Instance();

    //���ׂĂ̒e�ۂƑS�Ă̓G�𑍓���ŏՓˏ���
    int projectileCount = projectileManager.GetProjectileCount();
    for (int i = 0; i < projectileCount; ++i)
    {
        Projectile* projectile = projectileManager.GetProjectile(i);
        DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

        debugRenderer->DrawSphere(projectile->GetPosition(), projectile->GetRadius(), { 1,1,1,1 });

        //�Փˏ���
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectSphereVsCylinder(
            projectile->GetPosition(),
            projectile->GetRadius(),
            playerMgr.GetPlayer(playerMgr.GetWeapon())->GetPosition(),
            playerMgr.GetPlayer(playerMgr.GetWeapon())->GetRadius(),
            playerMgr.GetPlayer(playerMgr.GetWeapon())->GetHeight(),
            outPosition))
        {
            if (!playerMgr.GetPlayer(playerMgr.GetWeapon())->GetGuardFlag()) {
                //������΂�
                {
                    //�_���[�W��^����
                    if (playerMgr.GetPlayer(playerMgr.GetWeapon())->ApplyDamage(damageStamp, 3.0f))
                    {
                        //������΂�
                        {
                            DirectX::XMFLOAT3 impulse;
                            const float power = 0.0f;
                            const DirectX::XMFLOAT3& e = playerMgr.GetPlayer(playerMgr.GetWeapon())->GetPosition();
                            const DirectX::XMFLOAT3& p = projectile->GetPosition();
                            float vx = e.x - p.x;
                            float vz = e.z - p.z;
                            float lengthXZ = sqrtf(vx * vx + vz * vz);
                            vx /= lengthXZ;
                            vz /= lengthXZ;

                            impulse.x = vx * 10.0f;
                            impulse.y = power * 0.0f;
                            impulse.z = vx * power;

                            playerMgr.GetPlayer(playerMgr.GetWeapon())->AddImpulse(impulse);
                        }

                        //�q�b�g�G�t�F�N�g�Đ�
                        {
                            //  DirectX::XMFLOAT3 p = player.GetPosition();
                            //  p.y += player.GetHeight() * 0.5f;
                              /*hitEffect->Play(p);*/
                        }

                        //d hitEffect->Play(player.GetPosition());
                         //�e�۔j��
                        projectile->Destroy();
                    }
                }
            }
        }
    }
}

//�P���e�̓����蔻��
void EnemyBOSS::CollisionProjectilesVsPlayers()
{
    // �P���U�����a������
    colosionarea = 0;

    // �v���C���[
    PlayerManager& playerMgr = PlayerManager::Instance();

    //���ׂĂ̒e�ۂƑS�Ă̓G�𑍓���ŏՓˏ���
    int projectileCount = projectileManager.GetProjectileCount();
    for (int i = 0; i < projectileCount; ++i)
    {
        Projectile* projectile = projectileManager.GetProjectile(i);
        DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

        debugRenderer->DrawSphere(projectile->GetPosition(), projectile->GetRadius(), { 1,1,1,1 });

        //�Փˏ���
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectSphereVsCylinder(
            projectile->GetPosition(),
            projectile->GetRadius(),
            playerMgr.GetPlayer(playerMgr.GetWeapon())->GetPosition(),
            playerMgr.GetPlayer(playerMgr.GetWeapon())->GetRadius(),
            playerMgr.GetPlayer(playerMgr.GetWeapon())->GetHeight(),
            outPosition))
        {
            if (!playerMgr.GetPlayer(playerMgr.GetWeapon())->GetGuardFlag()) {
                //������΂�

                   //�_���[�W��^����
                if (playerMgr.GetPlayer(playerMgr.GetWeapon())->ApplyDamage(damageGun, 0.95f))
                {

                    damageaudio->Play(false, 1);
                    //DirectX::XMFLOAT3 impulse;
                   // const float power = 10.0f;
                    const DirectX::XMFLOAT3& e = playerMgr.GetPlayer(playerMgr.GetWeapon())->GetPosition();
                    const DirectX::XMFLOAT3& p = projectile->GetPosition();
                    float vx = e.x - p.x;
                    float vz = e.z - p.z;
                    float lengthXZ = sqrtf(vx * vx + vz * vz);
                    vx /= lengthXZ;
                    vz /= lengthXZ;

                   // impulse.x = vx * power;
                    //impulse.y = power * 0.0f;
                    //impulse.z = vx * power;

                   // playerMgr.GetPlayer(playerMgr.GetWeapon())->AddImpulse(impulse);
                }

                //�q�b�g�G�t�F�N�g�Đ�
                {
                    //  DirectX::XMFLOAT3 p = player.GetPosition();
                    //  p.y += player.GetHeight() * 0.5f;
                      /*hitEffect->Play(p);*/
                }
            }
            //d hitEffect->Play(player.GetPosition());
             //�e�۔j��
            projectile->Destroy();
        }
    }
}
    


//�͈͍U��
void EnemyBOSS::CollisionChargeVsPlayers()
{ // �����蔻��̊g�� �ł���̕K�v���Ȃ��Ȃ�����
    if (shotstate)
        //colosionarea += 1.9f;
    Graphics::Instance().GetDebugRenderer()->DrawSphere(oldposition, colosionarea, DirectX::XMFLOAT4(1, 0, 1, 1));

    // �v���C���[
    PlayerManager& playerMgr = PlayerManager::Instance();

    //���ׂĂ̒e�ۂƑS�Ă̓G�𑍓���ŏՓˏ���
    int projectileCount = projectileManager.GetProjectileCount();
    for (int i = 0; i < projectileCount; ++i)
    {
        Projectile* projectile = projectileManager.GetProjectile(i);

        //�Փˏ���
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectSphereVsCylinder(
            projectile->GetPosition(),
            colosionarea,
            playerMgr.GetPlayer(playerMgr.GetWeapon())->GetPosition(),
            playerMgr.GetPlayer(playerMgr.GetWeapon())->GetRadius(),
            playerMgr.GetPlayer(playerMgr.GetWeapon())->GetHeight(),
            outPosition))
        {
            if (!playerMgr.GetPlayer(playerMgr.GetWeapon())->GetGuardFlag()) {
                //�_���[�W��^����
                if (playerMgr.GetPlayer(playerMgr.GetWeapon())->ApplyDamage(damageCharge, 0.95f))
                {

                    //������΂�
                    {
                        damageaudio->Play(false, 1);
                        /*                  DirectX::XMFLOAT3 impulse;
                                          const float power = 0.0f;*/
                        const DirectX::XMFLOAT3& e = playerMgr.GetPlayer(playerMgr.GetWeapon())->GetPosition();
                        const DirectX::XMFLOAT3& p = projectile->GetPosition();
                        float vx = e.x - p.x;
                        float vz = e.z - p.z;
                        float lengthXZ = sqrtf(vx * vx + vz * vz);
                        vx /= lengthXZ;
                        vz /= lengthXZ;

                        //impulse.x = vx * power;
                        ////impulse.y = power * 1.0f;
                        //impulse.z = vx * power;

                        //playerMgr.GetPlayer(playerMgr.GetWeapon())->AddImpulse(impulse);
                    }

                    //�q�b�g�G�t�F�N�g�Đ�
                    {
                        //DirectX::XMFLOAT3 p = player.GetPosition();
                        //p.y += player.GetHeight() * 0.5f;
                        /*hitEffect->Play(p);*/
                    }

                    //�e�۔j��
                   // projectile->Destroy();
                  // hitEffect->Play(playerMgr.GetPlayer(playerMgr.GetWeapon())->GetPosition());

                }
            }
        }
    }
}

void EnemyBOSS::TransitionAttackGun()
{
    state = State::AttackGun;                  
     //�U�����
    attackrand = rand() % 2 + 1;

    // �͈͍U���Ȃ���ʃG�t�F�N�g��
    if (attackrand == Attack::Attack_Charge)
    {
        model->PlayAnimation(Anim_AttackCharge, false);
        model->PlayUpeerBodyAnimation(Anim_AttackCharge, false);
        chargeEffect->Play(DirectX::XMFLOAT3{ position.x,position.y + height, position.z });
        //model->PlayAnimation(Anim_Walking, true);

    }
    else
    {
        //model->PlayAnimation(Anim_Attack_Stamp, false);

        model->PlayAnimation(Anim_Attack_NormalGun, false);
        model->PlayUpeerBodyAnimation(Anim_Attack_NormalGun, false);
        
        //SE_Shot->Play(false);
        shotaudio->Play(false, 1);
    }

}

void EnemyBOSS::UpdateAttackGun(float elapsedTime)
{ 
    // �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
    float animationTime = model->GetCurrentAnimationSeconds();
    PlayerManager& playerMgr = PlayerManager::Instance();
       
    ////�U��
    switch (attackrand)
    {
    //case Attack::Attack_stamp:
    //    //�C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
    //    attackCollisionFlag = animationTime >= 1.0f && animationTime <= 3.0f;
    //    if (attackCollisionFlag)
    //    { 
    //        //attack_stampRange;
    //        stamp("boss_left_foot1", 5.0f);
    //    }
    //    break;

    case Attack::Attack_Gun:
    {
        attackCollisionFlag = animationTime >= 0.8f && animationTime <= 2.0f; //1.0f, 2.0f
        if (attackCollisionFlag)
        {
            //attackRange;
            NormalGun("body2", 10.0f);
        }
        break;
    }
    case Attack::Attack_Charge:
    {
        ++chargetime;
        chargeEffect->SetPosition(chargeEffect->GetHandle(), DirectX::XMFLOAT3{ position.x,position.y + height * 2,position.z });
        attackCollisionFlag = animationTime >= 0.5f && animationTime <= 10.0f;//0.5f, 10.0f
        if (chargetime >= chargetimeMax)
        {   // Character.h �̂Ƃ���́@int chargetimeMax = 260;�@�ɂ���Ƃ��ꂢ�Ɏ��� 
            //attackRange;

            ChargeGun("boss_left_hand1", 10.0f);
        }

        if (animationTime >= 1.0f && animationTime <= 1.1f)
            hotaudio->Play(false, 1);
        if (animationTime >= 1.9f && animationTime <= 2.0f)
        {
            hotaudio->Stop();
            shotaudio->Play(false, 1);
        }
        break;
    }
    }    

    if (shotstate)
    {
        
        shotaudio->Stop();
        TransitionIdleState();
    }
}

// �f���p
void EnemyBOSS::TransitionAttackDemoState()
{
    state = State::AttackDemo;

    //�_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_GetHit1, false);

    camerachange = false;
}
// �f���panimation
void EnemyBOSS::UpdateAttackDemoState(float elapsedTime)
{
    //�m�[�h�擾
    Model::Node* nodeL = model->FindNode("boss_left_foot1");
    Model::Node* nodeR = model->FindNode("boss_right_foot1");
    
    // �����̏��left
    DirectX::XMFLOAT3 footPosL = {
        nodeL->worldTransform._41,
        nodeL->worldTransform._42,
        nodeL->worldTransform._43 + 1
    };

    // �����̏��right
    DirectX::XMFLOAT3 footPosR = {
        nodeR->worldTransform._41,
        nodeR->worldTransform._42,
        nodeR->worldTransform._43 + 1
    };

    // �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓˏ���������
    float animationTime = model->GetCurrentAnimationSeconds();

    // if�ȊO�Ȃ�
    // �J�����h��
    shackChack = false;
    //// �J�����A�b�v
    rastapcamera = false;

    // �G�t�F�N�g��~
    smork->stop(smork->GetHandle());
   
    // �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓˏ���������
    //attackCollisionFlag = animationTime >= 1.0f && animationTime <= 3.0f;
    
    // ���ԓ��Ȃ�h���
    // ����
    if (animationTime >= 0.8f && animationTime <= 1.4f || 
        animationTime >= 4.7f && animationTime <= 5.0f 
        
        )
    {
        shackChack = true;
        //smork->Play(footPosL, 0.7f);
        
    
    }
    // ��
    if (animationTime >= 0.8f && animationTime <= 0.9f ||
        animationTime >= 4.7f && animationTime <= 4.9||
        animationTime >= 3.8f && animationTime <= 4.0f
        )
    {
        stumpaudio->Play(false, 1);
    }
    else
    {
        stumpaudio->Stop();
    }
    
    //if (animationTime >= 4.7f && animationTime <= 4.9f)
    //    stumpaudio->Play(false, 1);
    //if (animationTime >= 3.8f && animationTime <= 4.0f)
    //    stumpaudio->Play(false, 1);
    if (animationTime >= 6.9f && animationTime <= 7.5f)
        hotaudio->Play(false, 1);
    else
    {
        hotaudio->Stop();
    }
    // �E��
    if (animationTime >= 3.9f && animationTime <= 4.3f)
    {
        shackChack = true;
        //smork->Play(footPosR, 0.5f);
    }

    // �E��
    if (animationTime >= 6.7f && animationTime <= 7.6f)
    {
        shackChack = true;
        rastapcamera = true;
        //smork->Play(footPosR,0.6f);
    }

    if (animationTime >= 0.8f && animationTime <= 0.9f)
        smork->Play(footPosL, 0.7f);

    if (animationTime >= 4.7f && animationTime <= 4.8f)
        smork->Play(footPosL, 0.5f);


    if (animationTime >= 3.9f && animationTime <= 4.0f)
        smork->Play(footPosR, 0.5f);


    if (animationTime >= 6.7f && animationTime <= 6.7f)
        smork->Play(footPosR, 0.5f);
    // �t�F�C�h�A�E�g�J�n
    if (animationTime >= 8.2f)
        fadeoutcamera = true;

    // animation���I�������J�����̌�����player�ɖ߂�
    if (!model->IsPlayAnimation())
    {
        shackChack = false;
      
        camerachange = true;
        fadeoutcamera = false;
        //TransitionIdleState();
        TransitionEyeState();
    }
}

//�X�^���v�U��
void EnemyBOSS::stamp(const char* nodeName, float nodeRadius)
{
    //�C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
    //float animationTime = model->GetCurrentAnimationSeconds();
    //if (animationTime >= 1.0f && animationTime <= 5.00f)
    //{
    //shotTimer -= 1;

        // �ڋʃm�[�h�ƃv���C���[�̏Փˏ���
        CollisionNodeVsPlayer("boss_left_foot1", 5.0f);
    //}
    //{

       // NormalGun("blend_root", 2.5f);
   // }

    //�U���A�j���[�V�������I�������퓬�ҋ@�X�e�[�g�֑J��
    if (!model->IsPlayAnimation() && animationChange== AnimationChange::Normal )
    {
        TransitionIdleBattleState();
    }
    if (!model->IsPlayAnimation() && !model->IsPlayUpeerBodyAnimation()&& animationChange == AnimationChange::doble)
    {
        TransitionIdleBattleState();
    }
}

//�m�[�}���K��
void EnemyBOSS::NormalGun(const char* nodeName, float nodeRadius)
{
    shotTimer -= 1;

    //�m�[�h�擾
    Model::Node* node = model->FindNode(nodeName);
    
    PlayerManager& playerMgr = PlayerManager::Instance();
    
    //�m�[�h�ʒu�擾
    DirectX::XMFLOAT3 nodePosition;
    nodePosition.x = node->worldTransform._41;
    nodePosition.y = node->worldTransform._42;
    nodePosition.z = node->worldTransform._43;

    //if (shotTimer % num <= 0)
    if (shotTimer <= 0)
    {
        //�O����
        DirectX::XMFLOAT3 dir;

        dir.x = playerMgr.Instance().GetPlayer(playerMgr.GetWeapon())->GetPosition().x;
        dir.y = playerMgr.Instance().GetPlayer(playerMgr.GetWeapon())->GetPosition().y;
        dir.z = playerMgr.Instance().GetPlayer(playerMgr.GetWeapon())->GetPosition().z;

        //���ˈʒu
        DirectX::XMFLOAT3 pos;
        pos.x = nodePosition.x;

        //�����x�N�g�����o���ăv���C���[�̃����h���W���e�̈ʒu�܂ł̌v�Z���ϊ�
        pos.y = nodePosition.y;
        pos.z = nodePosition.z;

        //����
        EnemySkillGun* projectile = new EnemySkillGun(&projectileManager, "", "Data/Model/Bullet/Ruby.mdl"); //jewel(2).mdl
        projectile->Launch(dir, { pos.x,pos.y,pos.z }, {}, 20);

        shotTimer = 50;
        ++possession;
    }
    else if (possession == 5)
    {
        shotstate = true;
    }
}

//�͈͍U��
void EnemyBOSS::ChargeGun(const char* nodeName, float nodeRadius)
{
    shotTimer -= 1;

    //�m�[�h�擾
    Model::Node* node = model->FindNode(nodeName);

    //�m�[�h�ʒu�擾
    DirectX::XMFLOAT3 nodePosition;
    nodePosition.x = node->worldTransform._41;
    nodePosition.y = node->worldTransform._42;
    nodePosition.z = node->worldTransform._43;

    

    //if (shotTimer % num <= 0)
    if (shotTimer <= 0)
    {

        //�O����
        DirectX::XMFLOAT3 dir;

        dir.x = sinf(/*this->angle.x +*/ shotgunpos - 0.5f);
        dir.y = 0.0f;//5.0f;
        dir.z = cosf(/*this->angle.y +*/ shotgunpos - 0.5f);
        
        //���ˈʒu
        DirectX::XMFLOAT3 pos;
        pos.x = nodePosition.x;
        
        //�����x�N�g�����o���ăv���C���[�̃����h���W���e�̈ʒu�܂ł̌v�Z���ϊ�
        pos.y = nodePosition.y;
        pos.z = position.z;

        ++shotgunpos;
        
       //����
        EnemySkillGunCharge* projectile = new EnemySkillGunCharge(&projectileManager, "", "Data/Model/Bullet/Ruby.mdl");
        projectile->Launch(dir, { pos.x,pos.y - 3.0f,pos.z }, {}, 25);

       oldposition = pos;

        shotTimer = 2;
        
        ++possession;
    }
    else if (possession == 70)
    {
        shotstate = true;
    }
}

//���S�����Ƃ��ɌĂ΂��
void EnemyBOSS::OnDead()
{
    //���S�X�e�[�g�֑J��
    TransitionDeathState();
}

//�_���[�W���󂯂��Ƃ��ɌĂ΂��
void EnemyBOSS::OnDamaged()
{
    //�_���[�W�X�e�[�g�֑J��
    //TransitionDamageState();
}
