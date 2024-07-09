#include <imgui.h>
#include "PlayerGreat.h"
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraite.h"
#include "ProjectileHoming.h"
#include "LightManager.h"
#include "PlayerManager.h"
//#include "shiled.h"

//�R���X�g���N�^
PlayerGreat::PlayerGreat()
{
    //���f���̃��[�h
    model = new Model("Data/Model/Player/greatsword_player.mdl");
    
    
    //�̗͂̐ݒ�
    health = 15;
    maxHealth = 15;

    //���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.y = scale.z = 0.02f;

    //�q�b�g�G�t�F�N�g�ǂݍ���
    hitEffect = new Effect("Data/Effect/slash.efk");
    invincible_hitEffect = new Effect("Data/Effect/invincible.efk");
    guardmode = new Effect("Data/Effect/invincible.efk");

    attackcount = 0;
    weapon = 0;
    attackcount = 0;
    bullets = 60;
    totalattackcount = 0;
    changetime = 0.0f;
    guardflag = false;

    //SE�̐ݒ�
    attack_01 = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Swing7.wav");
    attack_02 = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Swing8.wav");
    attack_03 = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Swing9.wav");
    guard = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Guard.wav");
    damaged = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Player_Damaged.wav");
    brake = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Player_Brake.wav");
    run_fast = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Run_Fast_Dirt.wav");
    run_slow = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Run_Slow_Dirt.wav");



    UpdateTransform();

    //�ҋ@�X�e�[�g�֑J��(�匕)
    TransitionIdle_GreatState();
    PlayerManager::Instance().SetGuardMode(false);

}

//�f�X�g���N�^
PlayerGreat::~PlayerGreat()
{
    invincible_hitEffect->stop(invincible_hitEffect->GetHandle());
    delete  invincible_hitEffect;
    delete hitEffect;



    if (guardmode != nullptr)
    {
        delete guardmode;
        guardmode = nullptr;
    }

    delete model;

}

//�X�V����
void PlayerGreat::Update(float elapsedTime)
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
    
    //���@�̏�
        //���@�̏�
    if (position.y >= 0)
    {
        position.y = -1;
    }
    if (position.y < -10.0f)
    {
        position.y = -1;
    }

    //�X�e�[�g���̏���
        switch (state)
        {
            //�匕
        case State::Idle_Great:
            UpdateIdle_GreatState(elapsedTime);
            break;
        case State::Walk_Great:
            UpdateWalk_GreatState(elapsedTime);
            break;
        case State::Run_Great:
            UpdateRun_GreatState(elapsedTime);
            break;
        case State::Attack_Great:
            UpdateAttack_GreatState(elapsedTime);
            break;
        case State::Damage_Great:
            UpdateDamage_GreatState(elapsedTime);
            break;
        case State::Death_Great:
            UpdateDeath_GreatState(elapsedTime);
            break;
        case State::Guard_Great:
            UpdateGuard_GreatState(elapsedTime);
            break;
        case State::Dodge_Great:
            UpdateDamage_GreatState(elapsedTime);
            break;
        case State::Change_Great:
            UpdateChange_GreatState(elapsedTime);
            break;
        case State::Brake_Great:
            UpdateBrake_GreatState(elapsedTime);
            break;
        }



    //����ύX�N�[���^�C��
    changetimer++;

    //���͏����X�V
    UpdateVelocity(elapsedTime);

    //���G���ԍX�V����
    UpdateInvincibleTimer(elapsedTime);

    //�e�ۍX�V����
    projectileManager.Update(elapsedTime);

    //�v���C���[�ƓG�Ƃ̏Փˏ���
    CollisionPlayerVsEnemies();

    //�e�ۂƓG�̏Փˏ���
    CollisionProjectilesVsEnemies();

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //���f���A�j���[�V�����X�V����
    if (attackcount == 2)
    {
        model->ReverseplaybackAnimation(elapsedTime);
    }
    else
    {
        model->UpdateAnimation(elapsedTime);
    }

    attacktime++;
    attackrate++;




    //���f���s��X�V
    model->UpdateTransform(transform);
    //���I�u�W�F�N�g�̈ʒu�Ƀ��f���\�����邽�߁A
    //�I�u�W�F�N�g�̍s������f���ɓn���ă��f���̍s����v�Z����
}

//�`�揈��
void PlayerGreat::Render(RenderContext& rc, ModelShader* shader)
{
    rc.toonData.edgeWidth = 0.02f;

   

    shader->Draw(rc, model);
    //���`�悷��ۂɌv�Z���ꂽ���f���̍s������̒��ŗ��p

  

    //�e�ە`�揈��
    projectileManager.Render(rc, shader);
}

//�f�o�b�O�pGUI�`��
void PlayerGreat::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("PlayerGreat", nullptr, ImGuiWindowFlags_None))
    {
        //�g�����X�t�H�[��
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //�ʒu
            ImGui::InputFloat3("Position", &position.x);

            //��]
            DirectX::XMFLOAT3 a;
            a.x = DirectX::XMConvertToDegrees(angle.x);
            a.y = DirectX::XMConvertToDegrees(angle.y);
            a.z = DirectX::XMConvertToDegrees(angle.z);
            ImGui::InputFloat3("Angle", &a.x);
            angle.x = DirectX::XMConvertToRadians(a.x);
            angle.y = DirectX::XMConvertToRadians(a.y);
            angle.z = DirectX::XMConvertToRadians(a.z);

            //�X�P�[��
            ImGui::InputFloat3("Scale", &scale.x);
        }

        //���[�u�����g�i�����̑����j
        if (ImGui::CollapsingHeader("Movement", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //���x
            ImGui::InputFloat("MoveSpeed", &moveSpeed);

            //��]���x
            float ts;
            ts = DirectX::XMConvertToDegrees(turnSpeed);
            ImGui::InputFloat("TurnSpeed", &ts);
            turnSpeed = DirectX::XMConvertToRadians(ts);

            //�W�����v���x
            ImGui::InputFloat("JumpSpeed", &jumpSpeed);

            //�d��
            ImGui::InputFloat("Gravity", &gravity);

            //�����x�H
            ImGui::InputFloat3("Velocity", &velocity.x);

            //���̃X�e�[�g
            ImGui::InputInt("State", &statenumber);

            //���̕���ԍ�
            ImGui::InputInt("Weapon", &weapon);
        }

        //�R���W����
        if (ImGui::CollapsingHeader("Collision", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //���a
            ImGui::InputFloat("Radius", &radius);
        }
    }
    ImGui::End();
}

//�f�o�b�O�v���~�e�B�u�`��
void PlayerGreat::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //�Փ˔���p�̃f�o�b�O�~����`��
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

    //�e�ۃf�o�b�O�v���~�e�B�u�`��
    projectileManager.DrawDebugPrimitive();

    //�U���Փ˗p�̍���m�[�h�̃f�o�b�O����`��
    if (attackCollisionFlag)
    {
        Model::Node* swordBone = model->FindNode("master_Large_sword_01:J_hook_R_03");
        debugRenderer->DrawSphere(DirectX::XMFLOAT3(
            swordBone->worldTransform._41,
            swordBone->worldTransform._42,
            swordBone->worldTransform._43),
            greatRadius,
            DirectX::XMFLOAT4(1, 0, 0, 1)
        );
    }
}

//�e�ۂƓG�̏Փˏ���
void PlayerGreat::CollisionProjectilesVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //���ׂĂ̒e�ۂƑS�Ă̓G�𑍓���ŏՓˏ���
    int projectileCount = projectileManager.GetProjectileCount();
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < projectileCount; ++i)
    {
        Projectile* projectile = projectileManager.GetProjectile(i);

        for (int j = 0; j < enemyCount; ++j)
        {
            Enemy* enemy = enemyManager.GetEnemy(j);

            //�Փˏ���
            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectSphereVsCylinder(
                projectile->GetPosition(),
                projectile->GetRadius(),
                enemy->GetPosition(),
                enemy->GetRadius(),
                enemy->GetHeight(),
                outPosition))
            {
                //�_���[�W��^����
                if (enemy->ApplyDamage(1, 0.5f))
                {
                    //������΂�
                    {
                        DirectX::XMFLOAT3 impulse;
                        const float power = 10.0f;
                        const DirectX::XMFLOAT3& e = enemy->GetPosition();
                        const DirectX::XMFLOAT3& p = projectile->GetPosition();
                        float vx = e.x - p.x;
                        float vz = e.z - p.z;
                        float lengthXZ = sqrtf(vx * vx + vz * vz);
                        vx /= lengthXZ;
                        vz /= lengthXZ;

                        impulse.x = vx * power;
                        impulse.y = power * 0.5f;
                        impulse.z = vx * power;

                        enemy->AddImpulse(impulse);
                    }

                    //�q�b�g�G�t�F�N�g�Đ�
                    {
                        DirectX::XMFLOAT3 e = enemy->GetPosition();
                        e.y += enemy->GetHeight() * 0.5f;
                        hitEffect->Play(e);
                    }

                    //�e�۔j��
                    projectile->Destroy();
                }
            }
        }
    }
}




DirectX::XMFLOAT3 PlayerGreat::GetMoveVec() const
{
    //���͏����擾
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();     //���X�e�B�b�NX�����͏�Ԃ��擾
    float ay = gamePad.GetAxisLY();     //���X�e�B�b�NY�����͏�Ԃ��擾

    //�J���������ƃX�e�B�b�N�̓��͒l�ɂ���Đi�s�������v�Z����
    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

    //�ړ��x�N�g����XZ���ʂɐ����ȃx�N�g���ɂȂ�悤�ɂ���

    //�J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLength > 0.0f)
    {
        //�P�ʃx�N�g����
        cameraRightX /= cameraRightLength;
        cameraRightZ /= cameraRightLength;
    }

    //�J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraRightLength > 0.0f)
    {
        //�P�ʃx�N�g����
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }

    //�X�e�B�b�N�̐������͒l���J�����E�����ɔ��f���A
    //�X�e�B�b�N�̐������͒l���J�����O�����ɔ��f���A
    //�i�s�x�N�g�����v�Z����
    DirectX::XMFLOAT3 vec;
    vec.x = (cameraRightX * ax) + (cameraFrontX * ay);
    vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);
    //Y�������ɂ͈ړ����Ȃ�
    vec.y = 0.0f;

    return vec;
}

//�ړ����͏���
bool PlayerGreat::InputMove(float elapsedTime)
{
    //�i�s�x�N�g���擾
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    //�ړ�����
    Move(moveVec.x, moveVec.z, moveSpeed);

    //���񏈗�
    Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

    //�i�s�x�N�g����0�x�N�g���łȂ��ꍇ�͓��͂��ꂽ
    return moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
}

bool PlayerGreat::InputWalk(float elapsedTime)
{
    maxMoveSpeed = 7.0f;
    moveSpeed = 7.0f;

    //run�p�̃L�[�����͂���Ă��Ȃ��ꍇ�͕���
    //�i�s�x�N�g���擾
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    //�ړ�����
    Move(moveVec.x, moveVec.z, moveSpeed);

    //���񏈗�
    Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

    //�i�s�x�N�g����0�x�N�g���łȂ��ꍇ�͓��͂��ꂽ
    return moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
}

bool PlayerGreat::InputRun(float elapsedTime)
{
    //run�p�̃L�[�����͂��ꂽ�ꍇ�͑���
    //�i�s�x�N�g���擾
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    //���������悤��
    if (GetKeyState(VK_SHIFT) & 0x8000)
    {

        moveVec.x *= 1.5f;
        moveVec.y *= 1.5f;
        moveVec.z *= 1.5f;

        moveSpeed = 12.0f;
        maxMoveSpeed = 12.0f;

        //�ړ�����
        Move(moveVec.x, moveVec.z, moveSpeed);

        //���񏈗�
        Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

        //�i�s�x�N�g����0�x�N�g���łȂ��ꍇ�͓��͂��ꂽ
        return moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
    }
    return false;
}

bool PlayerGreat::InputDodge(float elpsedTime)
{
    //�R���g���[���L�[�������ꂽ��

    return false;
}

//�v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
void PlayerGreat::CollisionPlayerVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //���ׂĂ̓G�Ƃ̑�������ŏՓˏ���
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        //�Փˏ���
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectCylinderVsCylinder(
            position,
            radius,
            height,
            enemy->GetPosition(),
            enemy->GetRadius(),
            enemy->GetHeight(),
            outPosition))
        {
            //�G�̐^��t�߂ɂ����������𔻒�
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
            DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
            DirectX::XMFLOAT3 normal;
            DirectX::XMStoreFloat3(&normal, N);
            //�ォ�瓥��Â����ꍇ�͏��W�����v����
            if (normal.y > 0.8f)
            {
                //���W�����v����
                Jump(jumpSpeed * 0.5f);
            }
   
        }
    }
}

//�m�[�h�ƓG�̏Փˏ���
void PlayerGreat::CollisionNodeVsEnemies(const char* nodeName, float nodeRadius)
{
    //�m�[�h�擾
    Model::Node* node = model->FindNode(nodeName);

    //�m�[�h�ʒu�擾
    DirectX::XMFLOAT3 nodePosition;
    nodePosition.x = node->worldTransform._41;
    nodePosition.y = node->worldTransform._42;
    nodePosition.z = node->worldTransform._43;

    //�w��̃m�[�h�ƑS�Ă̓G�𑍓���ŏՓˏ���
    EnemyManager& enemyManager = EnemyManager::Instance();
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);

        //�Փˏ���
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectSphereVsCylinder(
            nodePosition, nodeRadius,
            enemy->GetPosition(),
            enemy->GetRadius(),
            enemy->GetHeight(),
            outPosition))
        {
            if (enemy->GetAnimationChange() == 0)
            {
                invincible_hitEffect->Play(nodePosition, 0.3f);
                return;
            }
            //�_���[�W��^����
            if (enemy->ApplyDamage(1, 0.2f))
            {
                //������΂�
                {
                    const float power = 10.0f;
                    const DirectX::XMFLOAT3& e = enemy->GetPosition();
                    float vx = e.x - nodePosition.x;
                    float vz = e.z - nodePosition.z;
                    float lengthXZ = sqrtf(vx * vx + vz * vz);
                    vx /= lengthXZ;
                    vz /= lengthXZ;

                    DirectX::XMFLOAT3 impulse;
                    impulse.x = vx * power;
                    impulse.y = power * 0.5f;
                    impulse.z = vz * power;

                    
                }
                //�q�b�g�G�t�F�N�g�Đ�
                {

                    hitEffect->Play(nodePosition, 0.2f);
                }
            }
        }
    }

}




//�U�����͏���
bool PlayerGreat::InputAttack()
{

    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();

    //TODO ��Ŗ߂�
    if ((gamePad.GetButtonDown() & GamePad::BTN_B) || (mouse.GetButton() & Mouse::BTN_LEFT))
    {
        return true;
    }

    return false;
}

bool PlayerGreat::InputWeapon()
{
    //TODO Manager�Ǘ�
    if ((GetAsyncKeyState(VK_SPACE) & 0x8001) && changetimer >= changetimermax)
    {


        return true;
    }
    return false;
}

bool PlayerGreat::InputGuard(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();

    if ((gamePad.GetButtonDown() & GamePad::BTN_LEFT_TRIGGER) || (mouse.GetButton() & Mouse::BTN_RIGHT))
    {
        guardflag = true;
        return true;
    }

    return false;
}


//�_���[�W���󂯂��Ƃ��ɌĂ΂��
void PlayerGreat::OnDamaged()
{
    TransitionDamage_GreatState();
}

//���S�����Ƃ��ɌĂ΂��
void PlayerGreat::OnDead()
{
    TransitionDeath_GreatState();
}
//�匕
void PlayerGreat::TransitionIdle_GreatState()
{
    statenumber = 11;

    //�匕�̑ҋ@�ɑJ��
    state = State::Idle_Great;

    //�ҋ@�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Great_Idle, true);
}

void PlayerGreat::UpdateIdle_GreatState(float elapsedTime)
{
    //����ύX���͏���
    if (InputWeapon())
    {
        //����ύX�X�e�[�g�֑J��
        TransitionChange_GreatState();
    }

    //�������͏���
    if (InputWalk(elapsedTime))
    {
        //�����X�e�[�g�֑J��
        TransitionWalk_GreatState();
    }

    //������͏���
    if (InputRun(elapsedTime))
    {
        //����X�e�[�g�֑J��
        TransitionRun_GreatState();
    }

    //�U�����͏���
    if (InputAttack() && attackrate > 20.0f)
    {
        attackcount = 1;
        //�U���X�e�[�g�֑J��
        TransitionAttack_GreatState();
    }


    //�K�[�h���͏���
    if (InputGuard(elapsedTime))
    {
        //�K�[�h�X�e�[�g�֑J��
        TransitionGuard_GreatState();
    }
}


void PlayerGreat::TransitionWalk_GreatState()
{
    statenumber = 12;

    //�匕�̕����ɑJ��
    state = State::Walk_Great;

    //�������[�V�����̍Đ�
    run_slow->Play(true, 1.0f);
    model->PlayAnimation(Anim_Great_Dash, true);
}

void PlayerGreat::UpdateWalk_GreatState(float elapsedTime)
{
    //����ύX���͏���
    if (InputWeapon())
    {
        run_slow->Stop();
        //����ύX�X�e�[�g�֑J��
        TransitionChange_GreatState();
    }

    //�ړ����͏���
    if (!InputWalk(elapsedTime))
    {
        run_slow->Stop();
        //�ҋ@�X�e�[�g�֑J��
        TransitionIdle_GreatState();
    }

    //������͏���
    if (InputRun(elapsedTime))
    {
        run_slow->Stop();
        //����X�e�[�g�֑J��
        TransitionRun_GreatState();
    }


    //�U�����͏���
    if (InputAttack() && attackrate > 20.0f)
    {
        run_slow->Stop();
        attackcount = 1;
        //�U���X�e�[�g�֑J��
        TransitionAttack_GreatState();
    }


    //�K�[�h���͏���
    if (InputGuard(elapsedTime))
    {
        run_slow->Stop();
        //�K�[�h�X�e�[�g�֑J��
        TransitionGuard_GreatState();
    }
}

void PlayerGreat::TransitionRun_GreatState()
{
    statenumber = 13;
    state = State::Run_Great;

    //���胂�[�V�������Đ�
    run_slow->Play(true, 1.0f);
    model->PlayAnimation(Anim_Great_Dash, true);
}

void PlayerGreat::UpdateRun_GreatState(float elapsedTime)
{
    //����ύX���͏���
    if (InputWeapon())
    {
        run_slow->Stop();
        //����ύX�X�e�[�g�֑J��
        TransitionChange_GreatState();
    }

    //������͏���
    if (!InputRun(elapsedTime))
    {
        run_slow->Stop();
        //�u���[�L�X�e�[�g�֑J��
        TransitionBrake_GreatState();
    }


    //�U�����͏���
    if (InputAttack() && attackrate > 20.0f)
    {
        run_slow->Stop();
        attackcount = 1;
        //�U���X�e�[�g�֑J��
        TransitionAttack_GreatState();
    }


    //�K�[�h���͏���
    if (InputGuard(elapsedTime))
    {
        run_slow->Stop();
        //�K�[�h�X�e�[�g�֑J��
        TransitionGuard_GreatState();
    }
}


void PlayerGreat::TransitionAttack_GreatState()
{
    statenumber = 16;
    state = State::Attack_Great;

    //�U���A�j���[�V�����Đ�
    if (attackcount == 1)
    {
        attack_01->Play(false, 1.0f);
        model->PlayAnimation(Anim_Great_Attack_1, false);
        attacktime = 0.0f;
    }

    if (attackcount == 2)
    {
        attack_02->Play(false, 1.0f);
        model->PlayAnimation(Anim_Great_Attack_1, false);
        attacktime = 0.0f;
    }

    if (attackcount == 3)
    {
        attack_03->Play(false, 1.0f);
        model->PlayAnimation(Anim_Great_Attack_3, false);
        attacktime = 0.0f;
    }
}

void PlayerGreat::UpdateAttack_GreatState(float elapsedTime)
{
    if (InputAttack() && attacktime > 30.0f && attackcount == 1)
    {
        attackcount = 2;
        attacktime = 0.0f;
        //�U���X�e�[�g�֑@��
        TransitionAttack_GreatState();
    }

    if (attackcount == 2 && InputAttack() && !model->IsPlayAnimation() && attacktime < 60.0f)
    {
        attackcount = 3;
        attacktime = 0.0f;
        TransitionAttack_GreatState();
    }

    //�C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
    float animationTime = model->GetCurrentAnimationSeconds();
    attackCollisionFlag = animationTime >= 0.0f && animationTime <= 1.0f;

    CollisionNodeVsEnemies("master_Large_sword_01:J_hook_R_03", greatRadius);

    //�U���A�j���[�V�����I����
    if ((!model->IsPlayAnimation() && !InputAttack()) || attacktime >= 60.0f)
    {
        attackCollisionFlag = false;
        attackrate = 0.0f;
        //�ҋ@�X�e�[�g�֑J��
        TransitionIdle_GreatState();
        attackcount = 0;
    }
}

void PlayerGreat::TransitionDamage_GreatState()
{
    statenumber = 17;
    state = State::Damage_Great;
    run_slow->Stop();

    //�_���[�W�A�j���[�V�����Đ�
    if (guardflag)
    {
        model->PlayAnimation(Anim_Great_Guard_Damaged, false);
    }
    else
    {
        model->PlayAnimation(Anim_Great_Damaged, false);
    }
}

void PlayerGreat::UpdateDamage_GreatState(float elapsedTime)
{

    //�_���[�W�A�j���[�V�������I�������ҋ@�X�e�[�g�֑J��
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_GreatState();
    }
}

void PlayerGreat::TransitionDeath_GreatState()
{
    statenumber = 18;
    state = State::Death_Great;
    run_slow->Stop();

    //�_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Great_Death, false);
}

void PlayerGreat::UpdateDeath_GreatState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        //�A�j���[�V�����Đ����I�������Q�[���I�[�o�[�ɂȂ�悤�ɂ���
    }
}


void PlayerGreat::TransitionGuard_GreatState()
{
    statenumber = 20;
    state = State::Guard_Great;
    guardflag = true;
    guardtime = 0;

   

    //�K�[�h���[�V�����Đ�
    //guard->Play(false, 1.0f);
    model->PlayAnimation(Anim_Great_Guard, false);
   
    //guardmode->Play(position, 0.5f);
    PlayerManager::Instance().SetGuardMode(true);
}

void PlayerGreat::UpdateGuard_GreatState(float elapsedTime)
{
    //�O����
    DirectX::XMFLOAT3 dir;
    dir.x = sinf(angle.y);
    dir.y = 0.0f;
    dir.z = cos(angle.y);

    guardtime++;
    if (guardtime >= 60)
    {
    
      
        guardtime = 0;
    }
    if (!InputGuard(elapsedTime))
    {
     
        guardflag = false;
        PlayerManager::Instance().SetGuardMode(false);
        //�ҋ@�X�e�[�g�֑J��
        TransitionIdle_GreatState();
    }

}

void PlayerGreat::TransitionChange_GreatState()
{
    state = State::Change_Great;
    weapon = 2;
    changetime = 0.0f;

    //����ύX���[�V�����Đ�
    model->PlayAnimation(Anim_Great_Change, false);


}

void PlayerGreat::UpdateChange_GreatState(float elapsedTime)
{
    PlayerManager::Instance().SetStartAnimation(true);
    DirectX::XMFLOAT3 position_great = position;
    DirectX::XMFLOAT3 angle_great = angle;
    PlayerManager::Instance().SetWeaponFlag(true);
    PlayerManager::Instance().SetBeforeWeapon(1);
    PlayerManager::Instance().SetWeapon(3);
    Player* player = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetWeapon());
    player->SetPosition(position_great);
    player->SetAngle(angle_great);
    player->SetChangeTimer(0.0f);
    player->SetHP(health);
    TransitionIdle_GreatState();
    position = { 0,1000000000000.0f,0 };

}

void PlayerGreat::TransitionBrake_GreatState()
{
    state = State::Brake_Great;

    run_slow->Stop();
    brake->Play(false, 1.0f);
    model->PlayAnimation(Anim_Great_Brake, false);
}

void PlayerGreat::UpdateBrake_GreatState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_GreatState();
    }
}
