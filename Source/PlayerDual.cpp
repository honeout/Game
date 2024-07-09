#include <imgui.h>
#include "PlayerDual.h"
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraite.h"
#include "ProjectileHoming.h"
#include "LightManager.h"
#include "PlayerManager.h"
#include "Graphics/Model.h"

//�R���X�g���N�^
PlayerDual::PlayerDual()
{
    //���f���̃��[�h
    model = new Model("Data/Model/Player/dualsword_player.mdl");

    //�̗͂̐ݒ�
    health = 15;
    maxHealth = 15;

    //���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.y = scale.z = 0.02f;

    //�q�b�g�G�t�F�N�g�ǂݍ���
    hitEffect = new Effect("Data/Effect/slash.efk");
    invincible_hitEffect = new Effect("Data/Effect/invincible.efk");
    
    attackcount = 0;
    weapon = 0;
    attackcount = 0;
    bullets = 60;
    totalattackcount = 0;
    changetime = 0.0f;

    node = model->FindNode("player_upper");

    position.z = 33;

    //SE�̐ݒ�
    attack_01 = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Swing7.wav");
    attack_02 = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Swing8.wav");
    attack_03 = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Swing9.wav");
    damaged = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Player_Damaged.wav");
    brake = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Player_Brake.wav");
    run_fast = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Run_Fast_Dirt.wav");
    run_slow = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Run_Slow_Dirt.wav");

    //�I�u�W�F�N�g�s����X�V
    UpdateTransform();

    //���f���s��X�V
    model->UpdateTransform(transform);

    //�ҋ@�X�e�[�g�֑J��
    TransitionIdle_DualState();
}

//�f�X�g���N�^
PlayerDual::~PlayerDual()
{
    invincible_hitEffect->stop(invincible_hitEffect->GetHandle());
    delete  invincible_hitEffect;

    delete hitEffect;

    delete model;
}

//�X�V����
void PlayerDual::Update(float elapsedTime)
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
    if (position.y >= 1)
    {
        position.y = -1;
    }
    if (position.y < -10.0f)
    {
        position.y = -1;
    }


    if (PlayerManager::Instance().GetWeapon() == 0)
    {
        //�X�e�[�g���̏���
        switch (state)
        {
            //�o��
        case State::Idle_Dual:
            UpdateIdle_DualState(elapsedTime);
            break;
        case State::Walk_Dual:
            UpdateWalk_DualState(elapsedTime);
            break;
        case State::Run_Dual:
            UpdateRun_DualState(elapsedTime);
            break;
        case State::Attack_Dual:
            UpdateAttack_DualState(elapsedTime);
            ////�m�[�h�擾
            //Model::Node* leftnode = model->FindNode("master_Large_sword_02:J_blade_L_01");
            //Model::Node* rightnode = model->FindNode("master_Large_sword_02:J_blade_R_01");
            //{
            //    // �Ō�̒��_�������Ă��ׂĂ�1�����ɂ��炷
            //    for (int i = MAX_POLYGON - 1; i > 0; --i)
            //    {
            //        trailPositions[0][i] = trailPositions[0][i - 1];
            //        trailPositions[1][i] = trailPositions[1][i - 1];
            //    }
            //}
            //{
            //    // ���̌��_���獪�{�Ɛ�[�܂ł̃I�t�Z�b�g�l
            //    DirectX::XMVECTOR RootOffset = DirectX::XMVectorSet(0, 0, 0.3f, 0);
            //    DirectX::XMVECTOR TipOffset = DirectX::XMVectorSet(0, 0, 2.3f, 0);
            //    //����̃m�[�h��0�Ԃ̃��[���h�s���p����
            //    //���ꂼ��̃I�t�Z�b�g�ɏ�Z���Ĉʒu���Z�o����
            //    // ���̈ʒu���擾���A���_�o�b�t�@�ɕۑ�
            //    DirectX::XMMATRIX WeaponRootTransform;
            //    DirectX::XMVECTOR RootPosition, TipPosition;
            //    WeaponRootTransform = DirectX::XMLoadFloat4x4(&leftnode->worldTransform);
            //    RootPosition = DirectX::XMVector3TransformCoord(RootOffset, WeaponRootTransform);
            //    TipPosition = DirectX::XMVector3TransformCoord(TipOffset, WeaponRootTransform);
            //    DirectX::XMStoreFloat3(&trailPositions[0][0], RootPosition);
            //    DirectX::XMStoreFloat3(&trailPositions[1][0], TipPosition);
            //}
            break;
        case State::Damage_Dual:
            UpdateDamage_DualState(elapsedTime);
            break;
        case State::Death_Dual:
            UpdateDeath_DualState(elapsedTime);
            break;
        case State::Dodge_Dual:
            UpdateDodge_DualState(elapsedTime);
            break;
        case State::Change_Dual:
            UpdateChange_DualState(elapsedTime);
            break;
        case State::Brake_Dual:
            UpdateBrake_DualState(elapsedTime);
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
        model->UpdateAnimation(elapsedTime);

        attacktime++;
        attackrate++;

        //���f���s��X�V
        model->UpdateTransform(transform);
        //���I�u�W�F�N�g�̈ʒu�Ƀ��f���\�����邽�߁A
        //�I�u�W�F�N�g�̍s������f���ɓn���ă��f���̍s����v�Z����
    }
}

//�`�揈��
void PlayerDual::Render(RenderContext& rc, ModelShader* shader)
{
    rc.toonData.edgeWidth = 0.02f;


    shader->Draw(rc, model);
    //���`�悷��ۂɌv�Z���ꂽ���f���̍s������̒��ŗ��p

    //�e�ە`�揈��
    projectileManager.Render(rc, shader);
}

//�f�o�b�O�pGUI�`��
void PlayerDual::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("PlayerDual", nullptr, ImGuiWindowFlags_None))
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

            ImGui::InputInt("bonebone", &node->now, 0, 10);
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
void PlayerDual::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //�Փ˔���p�̃f�o�b�O����`��
    //debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

    //�Փ˔���p�̃f�o�b�O�~����`��
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

    //�e�ۃf�o�b�O�v���~�e�B�u�`��
    projectileManager.DrawDebugPrimitive();

    //�U���Փ˗p�̍���m�[�h�̃f�o�b�O����`��
    if (attackCollisionFlag)
    {
        Model::Node* swordBone_R = model->FindNode("master_Large_sword_02:J_hook_R_03");
        Model::Node* swordBone_L = model->FindNode("master_Large_sword_02:J_hook_L_03");
        debugRenderer->DrawSphere(DirectX::XMFLOAT3(
            swordBone_R->worldTransform._41,
            swordBone_R->worldTransform._42,
            swordBone_R->worldTransform._43),
            dualRadius,
            DirectX::XMFLOAT4(1, 0, 0, 1)
        );
        debugRenderer->DrawSphere(DirectX::XMFLOAT3(
            swordBone_L->worldTransform._41,
            swordBone_L->worldTransform._42,
            swordBone_L->worldTransform._43),
            dualRadius,
            DirectX::XMFLOAT4(1, 0, 0, 1)
        );
    }
}

//�e�ۂƓG�̏Փˏ���
void PlayerDual::CollisionProjectilesVsEnemies()
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




DirectX::XMFLOAT3 PlayerDual::GetMoveVec() const
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
bool PlayerDual::InputMove(float elapsedTime)
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

bool PlayerDual::InputWalk(float elapsedTime)
{
    maxMoveSpeed = 10.0f;
    moveSpeed = 10.0f;

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

bool PlayerDual::InputRun(float elapsedTime)
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

        moveSpeed = 25.0f;
        maxMoveSpeed = 25.0f;

        //�ړ�����
        Move(moveVec.x, moveVec.z, moveSpeed);

        //���񏈗�
        Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

        //�i�s�x�N�g����0�x�N�g���łȂ��ꍇ�͓��͂��ꂽ
        return moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
    }
    return false;
}

bool PlayerDual::InputDodge(float elpsedTime)
{
    //�R���g���[���L�[�������ꂽ��

    return false;
}

//�v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
void PlayerDual::CollisionPlayerVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();
    DirectX::XMFLOAT3 nowposition = position;

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
            else
            {
                //position = position;

                //if (GetMoveVec().x >= 0.0f)
                //{
                //    AddImpulse({ -5.0f, 0.0f, 0.0f });
                //}
                //else if (GetMoveVec().x < 0.0f)
                //{
                //    AddImpulse({ 5.0f, 0.0f, 0.0f });
                //}
                //if (GetMoveVec().z >= 0.0f)
                //{
                //    AddImpulse({ 0.0f, 0.0f, -5.0f });
                //}
                //else if (GetMoveVec().z < 0.0f)
                //{
                //    AddImpulse({ 0.0f, 0.0f, 5.0f });
                //}

                //TransitionBrake_DualState();
                //enemy->SetPosition(enemy->GetPosition());
            }
        }
    }
}

//�m�[�h�ƓG�̏Փˏ���
void PlayerDual::CollisionNodeVsEnemies(const char* nodeName, float nodeRadius)
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

                    //enemy->AddImpulse(impulse);
                }
                //�q�b�g�G�t�F�N�g�Đ�
                {
                    //DirectX::XMFLOAT3 e = enemy->GetPosition();
                    //e.y += enemy->GetHeight() * 0.5f;


                    hitEffect->Play(nodePosition,0.2f);

                }
            }
        }
    }
}

//�W�����v���͏���
bool PlayerDual::InputJump()
{
    //�{�^�����͂ŃW�����v�i�W�����v�񐔐����t���j
    //GamePad& gamePad = Input::Instance().GetGamePad();
    //if ((gamePad.GetButtonDown() & GamePad::BTN_A) || (GetAsyncKeyState(VK_SPACE) & 0x8001))
    //{
    //    //�W�����v�񐔐���
    //    if (jumpCount < jumpLimit)
    //    {
    //        //�W�����v
    //        jumpCount++;
    //        Jump(jumpSpeed);

    //        //�W�����v���͂���
    //        return true;
    //    }
    //}
    return false;
}

//�e�ۓ��͏���
void PlayerDual::InputProjectile()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    //���i�e�۔���
    if (gamePad.GetButtonDown() & GamePad::BTN_X)
    {
        //�O����
        DirectX::XMFLOAT3 dir;
        dir.x = sinf(angle.y);
        dir.y = 0.0f;
        dir.z = cos(angle.y);

        //���ˈʒu�i�v���C���[�̍��ӂ�j
        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y + height * 0.5f;
        pos.z = position.z;

        //����
        ProjectileStraite* projectile = new ProjectileStraite(&projectileManager);
        projectile->Launch(dir, pos);
    }
    //�ǔ��e�۔���
    if (gamePad.GetButtonDown() & GamePad::BTN_Y)
    {
        //�O����
        DirectX::XMFLOAT3 dir;
        dir.x = sinf(angle.y);
        dir.y = 0.0f;
        dir.z = cosf(angle.y);

        //���ˈʒu�i�v���C���[�̍��ӂ�j
        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y + height * 0.5f;
        pos.z = position.z;

        //�^�[�Q�b�g�i�f�t�H���g�ł̓v���C���[�̑O���j
        DirectX::XMFLOAT3 target;
        target.x = pos.x + dir.x * 1000.0f;
        target.y = pos.y + dir.y * 1000.0f;
        target.z = pos.z + dir.z * 1000.0f;

        //��ԋ߂��̓G���^�[�Q�b�g�ɂ���
        float dist = FLT_MAX;
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();
        for (int i = 0; i < enemyCount; ++i)
        {
            //�G�Ƃ̋�������
            Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
            DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
            float d;
            DirectX::XMStoreFloat(&d, D);
            if (d < dist)
            {
                dist = d;
                target = enemy->GetPosition();
                target.y += enemy->GetHeight() * 0.5f;
            }
        }
        //����
        ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
        projectile->Launch(dir, pos, target);
    }
}

//�U�����͏���
bool PlayerDual::InputAttack()
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

//TODO������Manager�ŊǗ����Ȃ��Ƃ��܂������Ȃ�
bool PlayerDual::InputWeapon()
{
    if ((GetAsyncKeyState(VK_SPACE) & 0x8001) && changetimer >= changetimermax)
    {
        //if (weapon == 0)    //����Ȃ��i���͂Ȃ��j
        //{
        //    weapon = 1;
        //}
        //else if (weapon == 1)   //�匕
        //{
        //    weapon = 2;
        //}
        //else if (weapon == 2)   //�o��
        //{
        //    weapon = 3;
        //}
        //else if (weapon == 3)   //�e
        //{
        //    weapon = 1;
        //}

        return true;
    }
    return false;
}

bool PlayerDual::InputGuard(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();

    if ((gamePad.GetButtonDown() & GamePad::BTN_LEFT_TRIGGER) || (mouse.GetButton() & Mouse::BTN_RIGHT))
    {
        return true;
    }

    return false;
}

bool PlayerDual::InputReload()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    if ((gamePad.GetButtonDown() & GamePad::BTN_Y) || (GetAsyncKeyState('R') & 0x8000))
    {
        return true;
    }
    return false;
}

//�_���[�W���󂯂��Ƃ��ɌĂ΂��
void PlayerDual::OnDamaged()
{
    TransitionDamage_DualState();
}

//���S�����Ƃ��ɌĂ΂��
void PlayerDual::OnDead()
{
    TransitionDeath_DualState();
}

//�o��
void PlayerDual::TransitionIdle_DualState()
{
    statenumber = 21;

    //�o���̑ҋ@�ɑJ��
    state = State::Idle_Dual;

    //�ҋ@�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Dual_Idle, true);
}

void PlayerDual::UpdateIdle_DualState(float elapsedTime)
{
    //����ύX���͏���
    if (InputWeapon())
    {
        //����ύX�X�e�[�g�֑J��
        TransitionChange_DualState();
    }

    //�������͏���
    if (InputWalk(elapsedTime))
    {
        //�����X�e�[�g�֑J��
        TransitionWalk_DualState();
    }

    //������͏���
    if (InputRun(elapsedTime))
    {
        //����X�e�[�g�֑J��
        TransitionRun_DualState();
    }



    //�U�����͏���
    if (InputAttack() && attackrate > 30.0f)
    {
        attackcount = 1;
        //�U���X�e�[�g�֑J��
        TransitionAttack_DualState();
    }

    //�����͏���
    if (InputDodge(elapsedTime))
    {
        //����X�e�[�g�֑J��
        TransitionDodge_DualState();
    }
}

void PlayerDual::TransitionWalk_DualState()
{
    statenumber = 22;

    //�o���̕����ɑJ��
    state = State::Walk_Dual;

    //�������[�V�����̍Đ�
    run_slow->Play(true, 1.0f);
    model->PlayAnimation(Anim_Dual_Dash, true);
}

void PlayerDual::UpdateWalk_DualState(float elapsedTime)
{
    //����ύX���͏���
    if (InputWeapon())
    {
        run_slow->Stop();
        //����ύX�X�e�[�g�֑J��
        TransitionChange_DualState();
    }

    //�ړ����͏���
    if (!InputWalk(elapsedTime))
    {
        run_slow->Stop();
        //�ҋ@�X�e�[�g�֑J��
        TransitionIdle_DualState();
    }

    //������͏���
    if (InputRun(elapsedTime))
    {
        run_slow->Stop();
        //����X�e�[�g�֑J��
        TransitionRun_DualState();
    }



    //�U�����͏���
    if (InputAttack() && attackrate > 30.0f)
    {
        run_slow->Stop();
        attackcount = 1;
        //�U���X�e�[�g�֑J��
        TransitionAttack_DualState();
    }

    //�����͏���
    if (InputDodge(elapsedTime))
    {
        run_slow->Stop();
        //����X�e�[�g�֑J��
        TransitionDodge_DualState();
    }
}

void PlayerDual::TransitionRun_DualState()
{
    statenumber = 23;
    state = State::Run_Dual;

    //���胂�[�V�������Đ�
    run_fast->Play(true, 1.0f);
    model->PlayAnimation(Anim_Dual_Dash, true);
}

void PlayerDual::UpdateRun_DualState(float elapsedTime)
{
    //����ύX���͏���
    if (InputWeapon())
    {
        run_fast->Stop();
        //����ύX�X�e�[�g�֑J��
        TransitionChange_DualState();
    }

    //������͏���
    if (!InputRun(elapsedTime))
    {
        run_fast->Stop();
        //�u���[�L�X�e�[�g��
        TransitionBrake_DualState();

        //�ҋ@�X�e�[�g�֑J��
        //TransitionIdle_DualState();
    }

    //�U�����͏���
    if (InputAttack() && attackrate > 30.0f)
    {
        run_fast->Stop();
        attackcount = 1;
        //�U���X�e�[�g�֑J��
        TransitionAttack_DualState();
    }

    //�����͏���
    if (InputDodge(elapsedTime))
    {
        run_fast->Stop();
        //����X�e�[�g�֑J��
        TransitionDodge_DualState();
    }
}


void PlayerDual::TransitionAttack_DualState()
{
    statenumber = 26;
    state = State::Attack_Dual;

    //�U���A�j���[�V�����Đ�
    if (attackcount == 1)
    {
        attack_01->Play(false, 1.0f);
        model->PlayAnimation(Anim_Dual_Attack_1, false);
        attacktime = 0.0f;
    }

    if (attackcount == 2)
    {
        attack_02->Play(false, 1.0f);
        model->PlayAnimation(Anim_Dual_Attack_2, false);
        attacktime = 0.0f;
    }

    if (attackcount == 3)
    {
        attack_03->Play(true, 1.0f);
        model->PlayAnimation(Anim_Dual_Attack_3, false);
        attacktime = 0.0f;
    }
}

void PlayerDual::UpdateAttack_DualState(float elapsedTime)
{
    if (InputAttack() && attacktime > 52.0f && attackcount == 1)
    {
        attackcount = 2;
        attacktime = 0.0f;
        //�U���X�e�[�g�֑@��
        TransitionAttack_DualState();
    }

    if (attackcount == 2 && InputAttack() && !model->IsPlayAnimation() && attacktime < 60.0f)
    {
        attackcount = 3;
        attacktime = 0.0f;
        TransitionAttack_DualState();
    }

    //�C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
    float animationTime = model->GetCurrentAnimationSeconds();
    //attackCollisionFlag = animationTime >= 0.0f && animationTime <= 1.0f;
    attackCollisionFlag = true;

    CollisionNodeVsEnemies("master_Large_sword_02:J_hook_R_03", dualRadius);
    CollisionNodeVsEnemies("master_Large_sword_02:J_hook_L_03", dualRadius);

    //�U���A�j���[�V�����I����
    if ((!model->IsPlayAnimation() && !InputAttack()) || attacktime >= 120.0f)
    {
        attack_03->Stop();
        attackCollisionFlag = false;
        attackrate = 0.0f;
        //�ҋ@�X�e�[�g�֑J��
        TransitionIdle_DualState();
        attackcount = 0;
    }
}

void PlayerDual::TransitionDamage_DualState()
{
    statenumber = 27;
    state = State::Damage_Dual;

    run_slow->Stop();
    run_fast->Stop();

    //�_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Dual_Damaged, false);
}

void PlayerDual::UpdateDamage_DualState(float elapsedTime)
{
    //�_���[�W�A�j���[�V�������I�������ҋ@�X�e�[�g�֑J��
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_DualState();
    }
}

void PlayerDual::TransitionDeath_DualState()
{
    statenumber = 28;
    state = State::Death_Dual;

    run_slow->Stop();
    run_fast->Stop();

    //�_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Dual_Death, false);
}

void PlayerDual::UpdateDeath_DualState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        //TODO�A�j���[�V�����Đ����I�������Q�[���I�[�o�[�ɂȂ�悤�ɂ���
    }
}

void PlayerDual::TransitionDodge_DualState()
{
    statenumber = 29;
    state = State::Dodge_Dual;

    //���G���Ԑݒ�
    invincibleTimer = 1.0f;

    //�Ƃ肠�����t���b�v�A�j���[�V�������Đ�
    model->PlayAnimation(Anim_Dual_Dodge, false);
}

void PlayerDual::UpdateDodge_DualState(float elapsedTime)
{
    //�t���b�v�A�j���[�V�����I����A�ҋ@�X�e�[�g�֑J��
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_DualState();
    }
}

void PlayerDual::TransitionChange_DualState()
{
    state = State::Change_Dual;

    weapon = 3;
    changetime = 0.0f;

    //����ύX���[�V�����Đ�
    model->PlayAnimation(Anim_Dual_Death, false);
}

void PlayerDual::UpdateChange_DualState(float elapsedTime)
{
    //���̒Ǐ]
    //LightManager::Instance().GetLight(0)->SetPosition({ position.x, position.y + height, position.z });
    //LightManager::Instance().GetLight(1)->SetColor({ 0.0f, 0.0f,1.0f,1.0f });
    //LightManager::Instance().GetLight(0)->SetRange(1000000000.0f);
    //if (!model->IsPlayAnimation())
    //{
    PlayerManager::Instance().SetStartAnimation(true);
    DirectX::XMFLOAT3 position_dual = position;
    DirectX::XMFLOAT3 angle_dual = angle;
    PlayerManager::Instance().SetWeaponFlag(true);
    //PlayerManager::Instance().SetWeapon(1);
    PlayerManager::Instance().SetBeforeWeapon(0);
    PlayerManager::Instance().SetWeapon(3);
    Player* player = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetWeapon());
    player->SetPosition(position_dual);
    player->SetAngle(angle_dual);
    player->SetHP(health);
    player->SetChangeTimer(0.0f);
    TransitionIdle_DualState();
    position = { 0,1000000000000.0f,0 };
    //}

    //����ύX���[�V�����̍Đ����I���Ύ��̕���֕ύX
    //if (!model->IsPlayAnimation())
    //{
    //    //TODOManager��model���̂�ύX����
    //}
}

void PlayerDual::TransitionBrake_DualState()
{
    state = State::Brake_Dual;
    run_slow->Stop();
    run_slow->Stop();
    brake->Play(false, 1.0f);
    model->PlayAnimation(Anim_Dual_Brake, false);
}

void PlayerDual::UpdateBrake_DualState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_DualState();
    }
}
