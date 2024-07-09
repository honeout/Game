#include <imgui.h>
#include "PlayerGun.h"
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraite.h"
#include "ProjectileHoming.h"
#include "LightManager.h"
#include "PlayerManager.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

double getRandomFromRange(double min, double max)
{
    return min + (max - min) * (rand() / (RAND_MAX + 1.0));
}

float getRandomInRange(float min, float max)
{
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

//�R���X�g���N�^
PlayerGun::PlayerGun()
{
    //���f���̃��[�h
    model = new Model("Data/Model/Player/gun_player.mdl");

    //�̗͂̐ݒ�
    health = 15;
    maxHealth = 15;

    //���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.y = scale.z = 0.02f;

    //�q�b�g�G�t�F�N�g�ǂݍ���
    hitEffect = new Effect("Data/Effect/bossHitShotDamage.efk");
    invincible_hitEffect = new Effect("Data/Effect/invincible.efk");
    shotEffect = new Effect("Data/Effect/PlayerShot.efk");

    attackcount = 18;
    weapon = 0;
    bullets = 60;
    totalattackcount = 0;
    changetime = 0.0f;
    reloadtime = 0.0f;
    braketime = 0.0f;

    //�J�����R���g���[���[������
    cameraController = new CameraController();

    UpdateTransform();

    //SE�̐ݒ�
    shot = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Gun_Attack2.wav");
    reload = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Gun_Reload.wav");
    damaged = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Player_Damaged.wav");
    brake = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Player_Brake.wav");
    run_fast = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Run_Fast_Dirt.wav");
    run_slow = Audio::Instance().LoadAudioSource("Data/Audio/Se/Player/Run_Slow_Dirt.wav");

    //�ҋ@�X�e�[�g�֑J��
    TransitionIdle_GunState();

    // �����̏�����
    //srand(time(nullptr));
}

//�f�X�g���N�^
PlayerGun::~PlayerGun()
{
    //�J�����R���g���[���[�I����
    if (cameraController != nullptr)
    {
        delete cameraController;
        cameraController = nullptr;
    }
    invincible_hitEffect->stop(invincible_hitEffect->GetHandle());
    delete  invincible_hitEffect;
    delete hitEffect;
    delete shotEffect;

    delete model;
}

//�X�V����
void PlayerGun::Update(float elapsedTime)
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
            //�e
        case State::Idle_Gun:
            UpdateIdle_GunState(elapsedTime);
            break;
        case State::Walk_Gun:
            UpdateWalk_GunState(elapsedTime);
            break;
        case State::Run_Gun:
            UpdateRun_GunState(elapsedTime);
            break;
        case State::Attack_Gun:
            UpdateAttack_GunState(elapsedTime);
            break;
        case State::Damage_Gun:
            UpdateDamage_GunState(elapsedTime);
            break;
        case State::Death_Gun:
            UpdateDeath_GunState(elapsedTime);
            break;
        case State::Dodge_Gun:
            UpdateDodge_GunState(elapsedTime);
            break;
        case State::Reload_Gun:
            UpdateReload_GunState(elapsedTime);
            break;
        case State::Change_Gun:
            UpdateChange_GunState(elapsedTime);
            break;
        case State::Brake_Gun:
            UpdateBrake_GunState(elapsedTime);
            break;
        }

        //���̒Ǐ]
        //LightManager::Instance().GetLight(1)->SetPosition({position.x, position.y + 3.0f, position.z});

        //����ύX�N�[���^�C��
        changetimer++;

        //�����[�h�^�C��
        reloadtime++;

        //���͏����X�V
        UpdateVelocity(elapsedTime);

        //���G���ԍX�V����
        UpdateInvincibleTimer(elapsedTime);

        //�e�ۍX�V����
        projectileManager.Update(elapsedTime);

        //�v���C���[�ƓG�Ƃ̏Փˏ���
        CollisionPlayerVsEnemies();

        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();


        //�e�ۂƓG�̏Փˏ���
        CollisionProjectilesVsEnemies();
        
        

        //�I�u�W�F�N�g�s����X�V
        UpdateTransform();

        //���f���A�j���[�V�����X�V����
        model->UpdateAnimation(elapsedTime);

        //���f���s��X�V
        model->UpdateTransform(transform);
        //���I�u�W�F�N�g�̈ʒu�Ƀ��f���\�����邽�߁A
        //�I�u�W�F�N�g�̍s������f���ɓn���ă��f���̍s����v�Z����

        rate++;
        braketime++;
}

//�`�揈��
void PlayerGun::Render(RenderContext& rc, ModelShader* shader)
{
    rc.toonData.edgeWidth = 0.02f;

    shader->Draw(rc, model);
    //���`�悷��ۂɌv�Z���ꂽ���f���̍s������̒��ŗ��p

    //�e�ە`�揈��
    projectileManager.Render(rc, shader);
}

//�f�o�b�O�pGUI�`��
void PlayerGun::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("PlayerGun", nullptr, ImGuiWindowFlags_None))
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
void PlayerGun::DrawDebugPrimitive()
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
        Model::Node* leftHandBone = model->FindNode("mixamorig:LeftHand");
        debugRenderer->DrawSphere(DirectX::XMFLOAT3(
            leftHandBone->worldTransform._41,
            leftHandBone->worldTransform._42,
            leftHandBone->worldTransform._43),
            leftHandRadius,
            DirectX::XMFLOAT4(1, 0, 0, 1)
        );
    }
}

//�e�ۂƓG�̏Փˏ���
void PlayerGun::CollisionProjectilesVsEnemies()
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
                
                if (enemy->GetAnimationChange() == 1)
                {
                    invincible_hitEffect->Play(projectile->GetPosition(), 0.3f);
                    invincible_hitEffect->SetRotation(invincible_hitEffect->GetHandle(), projectile->GetAngle());
                    return;
                }

                //�_���[�W��^����
                if (enemy->ApplyDamage(1, 0.1f))
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

                        //enemy->AddImpulse(impulse);
                    }

                    //�q�b�g�G�t�F�N�g�Đ�
                    {

                        //weponNode = model->FindNode("Greadsword");


                        //DirectX::XMFLOAT3 weponbreadposition =
                        //{
                        //    weponNode->worldTransform._41,
                        //    weponNode->worldTransform._42,
                        //    weponNode->worldTransform._43
                        //};

                        hitEffect->Play(projectile->GetPosition(), 0.4f);
                        hitEffect->SetRotation(hitEffect->GetHandle(), projectile->GetAngle());
                        //DirectX::XMFLOAT3 e = enemy->GetPosition();
                        //e.y += enemy->GetHeight() * 0.5f;
                        //hitEffect->Play(projectile->GetPosition(), 0.1f);
                    }

                    //�e�۔j��
                    projectile->Destroy();
                }
            }
        }
    }
}


DirectX::XMFLOAT3 PlayerGun::GetMoveVec() const
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
bool PlayerGun::InputMove(float elapsedTime)
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

bool PlayerGun::InputWalk(float elapsedTime)
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

bool PlayerGun::InputRun(float elapsedTime)
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

        moveSpeed = 15.0f;
        maxMoveSpeed = 15.0f;

        //�ړ�����
        Move(moveVec.x, moveVec.z, moveSpeed);

        //���񏈗�
        Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

        //�i�s�x�N�g����0�x�N�g���łȂ��ꍇ�͓��͂��ꂽ
        return moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
    }

    return false;
}

//�v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
void PlayerGun::CollisionPlayerVsEnemies()
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
            else
            {
                //position = position;
               /* if (GetMoveVec().x >= 0.0f)
                {
                    AddImpulse({ -5.0f, 0.0f, 0.0f });
                }
                else if (GetMoveVec().x < 0.0f)
                {
                    AddImpulse({ 5.0f, 0.0f, 0.0f });
                }
                if (GetMoveVec().z >= 0.0f)
                {
                    AddImpulse({ 0.0f, 0.0f, -5.0f });
                }
                else if (GetMoveVec().z < 0.0f)
                {
                    AddImpulse({ 0.0f, 0.0f, 5.0f });
                }

                TransitionBrake_GunState();*/
                //enemy->SetPosition(enemy->GetPosition());
            }
        }
    }
}

//�m�[�h�ƓG�̏Փˏ���
void PlayerGun::CollisionNodeVsEnemies(const char* nodeName, float nodeRadius)
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
            //�_���[�W��^����
            if (enemy->ApplyDamage(1, 0.5f))
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

                    enemy->AddImpulse(impulse);
                }
                //�q�b�g�G�t�F�N�g�Đ�
                {
                    DirectX::XMFLOAT3 e = enemy->GetPosition();
                    e.y += enemy->GetHeight() * 0.5f;
                    hitEffect->Play(e, 0.05f);
                }
            }
        }
    }

}

//�W�����v���͏���
bool PlayerGun::InputJump()
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
void PlayerGun::InputProjectile()
{
    //GamePad& gamePad = Input::Instance().GetGamePad();
    //
    //���i�e�۔���
    //if (gamePad.GetButtonDown() & GamePad::BTN_X)
 //
    //�ǔ��e�۔���
    //if (gamePad.GetButtonDown() & GamePad::BTN_Y)
    //{
    //    //�O����
    //    DirectX::XMFLOAT3 dir;
    //    dir.x = sinf(angle.y);
    //    dir.y = 0.0f;
    //    dir.z = cosf(angle.y);
    //
    //    //���ˈʒu�i�v���C���[�̍��ӂ�j
    //    DirectX::XMFLOAT3 pos;
    //    pos.x = position.x;
    //    pos.y = position.y + height * 0.5f;
    //    pos.z = position.z;
    //
    //    //�^�[�Q�b�g�i�f�t�H���g�ł̓v���C���[�̑O���j
    //    DirectX::XMFLOAT3 target;
    //    target.x = pos.x + dir.x * 1000.0f;
    //    target.y = pos.y + dir.y * 1000.0f;
    //    target.z = pos.z + dir.z * 1000.0f;
    //
    //    //��ԋ߂��̓G���^�[�Q�b�g�ɂ���
    //    float dist = FLT_MAX;
    //    EnemyManager& enemyManager = EnemyManager::Instance();
    //    int enemyCount = enemyManager.GetEnemyCount();
    //    for (int i = 0; i < enemyCount; ++i)
    //    {
    //        //�G�Ƃ̋�������
    //        Enemy* enemy = EnemyManager::Instance().GetEnemy(i);
    //        DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
    //        DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
    //        DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
    //        DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
    //        float d;
    //        DirectX::XMStoreFloat(&d, D);
    //        if (d < dist)
    //        {
    //            dist = d;
    //            target = enemy->GetPosition();
    //            target.y += enemy->GetHeight() * 0.5f;
    //        }
    //    }
    //    //����
    //    ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
    //    projectile->Launch(dir, pos, target);
    //}

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
    if (attackcount >= 0)
    {
        ProjectileStraite* projectile = new ProjectileStraite(&projectileManager);
        projectile->Launch(dir, pos);
        attackcount--;
    }
}

//�U�����͏���
bool PlayerGun::InputAttack()
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

bool PlayerGun::InputWeapon()
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

bool PlayerGun::InputGuard(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();

    if ((gamePad.GetButtonDown() & GamePad::BTN_LEFT_TRIGGER) || (mouse.GetButton() & Mouse::BTN_RIGHT))
    {
        return true;
    }

    return false;
}

bool PlayerGun::InputReload()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();

    if (((gamePad.GetButtonDown() & GamePad::BTN_Y) || (GetAsyncKeyState('R') & 0x8000)) && reloadtime > 90.0f || (mouse.GetButton() & Mouse::BTN_RIGHT) && reloadtime > 90.0f)
    {
        reloadtime = 0.0f;
        return true;
    }
    return false;
}

//�_���[�W���󂯂��Ƃ��ɌĂ΂��
void PlayerGun::OnDamaged()
{
    TransitionDamage_GunState();
}

//���S�����Ƃ��ɌĂ΂��
void PlayerGun::OnDead()
{
    TransitionDeath_GunState();
}

//�e
void PlayerGun::TransitionIdle_GunState()
{
    statenumber = 31;

    //�e�̑ҋ@�ɑJ��
    state = State::Idle_Gun;

    //�ҋ@�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Gun_Idle, true);
}

void PlayerGun::UpdateIdle_GunState(float elapsedTime)
{
    //����ύX���͏���
    if (InputWeapon())
    {
        //����ύX�X�e�[�g�֑J��
        TransitionChange_GunState();
    }

    //�������͏���
    if (InputWalk(elapsedTime))
    {
        //�����X�e�[�g�֑J��
        TransitionWalk_GunState();
    }

    //������͏���
    if (InputRun(elapsedTime))
    {
        //����X�e�[�g�֑J��
        TransitionRun_GunState();
    }


    //�U�����͏���
    if (InputAttack() && attackcount >= 0)
    {
        //�U���X�e�[�g�֑J��
        TransitionAttack_GunState();
    }

    //�����[�h���͏���
    if (InputReload())
    {
        TransitionReload_GunState();
    }
}

void PlayerGun::TransitionWalk_GunState()
{
    statenumber = 32;

    //�e�̕����ɑJ��
    state = State::Walk_Gun;

    run_slow->Play(true, 1.0f);
    //�������[�V�����̍Đ�
    model->PlayAnimation(Anim_Gun_Dash, true);
}

void PlayerGun::UpdateWalk_GunState(float elapsedTime)
{
    //����ύX���͏���
    if (InputWeapon())
    {
        run_slow->Stop();
        //����ύX�X�e�[�g�֑J��
        TransitionChange_GunState();
    }

    //�ړ����͏���
    if (!InputWalk(elapsedTime))
    {
        run_slow->Stop();
        //�ҋ@�X�e�[�g�֑J��
        TransitionIdle_GunState();
    }

    //������͏���
    if (InputRun(elapsedTime))
    {
        run_slow->Stop();
        //����X�e�[�g�֑J��
        TransitionRun_GunState();
    }

    //�U�����͏���
    if (InputAttack() && attackcount >= 0)
    {
        run_slow->Stop();
        //�U���X�e�[�g�֑J��
        TransitionAttack_GunState();
    }

    //�����[�h���͏���
    if (InputReload())
    {
        run_slow->Stop();
        TransitionReload_GunState();
    }
}

void PlayerGun::TransitionRun_GunState()
{
    statenumber = 33;
    state = State::Run_Gun;

    //���胂�[�V�������Đ�
    run_fast->Play(true, 1.0f);
    model->PlayAnimation(Anim_Gun_Dash_02, true);
}

void PlayerGun::UpdateRun_GunState(float elapsedTime)
{
    //����ύX���͏���
    if (InputWeapon())
    {
        run_fast->Stop();
        //����ύX�X�e�[�g�֑J��
        TransitionChange_GunState();
    }

    //������͏���
    if (!InputRun(elapsedTime))
    {
        run_fast->Stop();
        //�u���[�L�X�e�[�g�֑J��
        TransitionBrake_GunState();

        //�ҋ@�X�e�[�g�֑J��
        //TransitionIdle_GunState();
    }

    //�U�����͏���
    if (InputAttack() && attackcount >= 0)
    {
        run_fast->Stop();
        //�U���X�e�[�g�֑J��
        TransitionAttack_GunState();
    }

    //�����[�h���͏���
    if (InputReload())
    {
        run_fast->Stop();
        TransitionReload_GunState();
    }
}

void PlayerGun::TransitionAttack_GunState()
{
    statenumber = 36;
    state = State::Attack_Gun;

    //�U���A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Gun_Attack, false);
}

void PlayerGun::UpdateAttack_GunState(float elapsedTime)
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
    float cameraFrontY = cameraFront.y;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraRightLength > 0.0f)
    {
        //�P�ʃx�N�g����
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
        cameraFrontY /= cameraFrontLength;
    }

    //�X�e�B�b�N�̐������͒l���J�����E�����ɔ��f���A
    //�X�e�B�b�N�̐������͒l���J�����O�����ɔ��f���A
    //�i�s�x�N�g�����v�Z����
    DirectX::XMFLOAT3 vec;
    vec.x = (cameraFrontX * 1.0f);
    vec.z = (cameraFrontZ * 1.0f);
    //Y�������ɂ͈ړ����Ȃ�
    vec.y = 0.0f;

    //�i�s�x�N�g���擾
    //DirectX::XMFLOAT3 moveVec = GetMoveVec();

    //���񏈗�
    Turn(elapsedTime, vec.x, vec.z, turnSpeed);

    UpdateTransform();

    {
        //�J�����R���g���[���[�X�V����
        DirectX::XMFLOAT3 target = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetWeapon())->GetPosition();
        target.y += 0.5f;										//�v���C���[�̍��ӂ�Ƀ^�[�Q�b�g��ݒ�
        cameraController->SetTarget(target);
        cameraController->Update(elapsedTime);


        // 0.1����0.8�͈̗̔͂�������
        //double randomNumber = getRandomFromRange(0.1, 1.0);
        float randomNumber = getRandomInRange(-0.2f, 0.5f);

        //�O����
        DirectX::XMFLOAT3 dir;
        dir.x = sinf(angle.y);
        dir.y = angle.x;
        dir.z = cos(angle.y);

        //���ˈʒu�i�v���C���[�̍��ӂ�j
        DirectX::XMFLOAT3 pos;
        pos.x = position.x + randomNumber;
        pos.y = position.y + height * 0.5f + randomNumber;
        pos.z = position.z + randomNumber;

        //����
        if (attackcount >= 0 && rate > 3.0f)
        {
            ProjectileStraite* projectile = new ProjectileStraite(&projectileManager);
            projectile->Launch(dir, pos);
            shot->Play(false, 1.0f);
            shotEffect->Play({ pos.x, pos.y,pos.z }, 0.05f);
            attackcount--;
            rate = 0.0f;
        }
    }


    //�U���A�j���[�V�����I����
    if (!model->IsPlayAnimation() || attackcount <= 0)
    {
        shot->Stop();
        //�ҋ@�X�e�[�g�֑J��
        TransitionIdle_GunState();
    }


}

void PlayerGun::TransitionDamage_GunState()
{
    statenumber = 37;
    state = State::Damage_Gun;

    run_slow->Stop();
    run_fast->Stop();

    //�_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Gun_Damaged, false);
}

void PlayerGun::UpdateDamage_GunState(float elapsedTime)
{
    //�_���[�W�A�j���[�V�������I�������ҋ@�X�e�[�g�֑J��
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_GunState();
    }
}

void PlayerGun::TransitionDeath_GunState()
{
    statenumber = 38;
    state = State::Death_Gun;
    run_slow->Stop();
    run_fast->Stop();

    //�_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Gun_Death, false);
}

void PlayerGun::UpdateDeath_GunState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        //�A�j���[�V�����Đ����I�������Q�[���I�[�o�[�ɂȂ�悤�ɂ���
    }
}

void PlayerGun::TransitionDodge_GunState()
{
    statenumber = 39;
    state = State::Dodge_Gun;

    //���G���Ԑݒ�
    invincibleTimer = 1.0f;

    //�Ƃ肠�����t���b�v�A�j���[�V�������Đ�
    model->PlayAnimation(Anim_Gun_Dodge, false);
}

void PlayerGun::UpdateDodge_GunState(float elapsedTime)
{
    //�t���b�v�A�j���[�V�����I����A�ҋ@�X�e�[�g�֑J��
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_GunState();
    }
}

void PlayerGun::TransitionReload_GunState()
{
    statenumber = 40;
    state = State::Reload_Gun;

    //�����[�h�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Gun_Reload, false);
    reload->Play(false, 1.0f);
}

void PlayerGun::UpdateReload_GunState(float elapsedTime)
{
    //�����[�h�ɂ����鎞��
    //reloadtime--;

    //�ړ����͂��������璆�f���Ĉړ�
    //if (InputWalk(elapsedTime))
    //{
    //    TransitionWalk_GunState();
    //}
    //if (InputRun(elapsedTime))
    //{
    //    TransitionRun_GunState();
    //}

    if (!model->IsPlayAnimation() && reloadtime >= 60.0f)
    {
        //�e�򐔂����ɖ߂�
        reloadtime = 0.0f;
        attackcount = 18;

        //�A�j���[�V�������I�������ҋ@��Ԃɖ߂�
        TransitionIdle_GunState();
    }

}

void PlayerGun::TransitionChange_GunState()
{
    state = State::Change_Gun;

    weapon = 0;
    changetime = 0.0f;

    //����ύX���[�V�����Đ�
    model->PlayAnimation(Anim_Gun_Reload, false);
}

void PlayerGun::UpdateChange_GunState(float elapsedTime)
{
    projectileManager.Clear();
    PlayerManager::Instance().SetStartAnimation(true);
    DirectX::XMFLOAT3 position_gun = position;
    DirectX::XMFLOAT3 angle_gun = angle;
    PlayerManager::Instance().SetWeaponFlag(true);
    PlayerManager::Instance().SetBeforeWeapon(2);
    PlayerManager::Instance().SetWeapon(3);
    Player* player = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetWeapon());
    player->SetPosition(position_gun);
    player->SetAngle(angle_gun);
    player->SetChangeTimer(0.0f);
    player->SetHP(health);
    TransitionIdle_GunState();
    position = { 0,1000000000000.0f,0 };
    //����ύX���[�V�����̍Đ����I���Ύ��̕���֕ύX
    //if (!model->IsPlayAnimation())
    //{
    //    //
    //}
}

void PlayerGun::TransitionBrake_GunState()
{
    state = State::Brake_Gun;

    braketime = 0.0f;
    run_slow->Stop();
    model->PlayAnimation(Anim_Gun_Brake, false);
    brake->Play(false, 1.0f);
}

void PlayerGun::UpdateBrake_GunState(float elapsedTime)
{
    if (!model->IsPlayAnimation() || braketime > 30.0f)
    {
        braketime = 0.0f;
        TransitionIdle_GunState();
    }
}
