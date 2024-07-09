#include <imgui.h>
#include "PlayerRobot.h"
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraite.h"
#include "ProjectileHoming.h"
#include "LightManager.h"

static PlayerRobot* instance = nullptr;

//�C���X�^���X�擾
PlayerRobot& PlayerRobot::Instance()
{
    return *instance;
}

//�R���X�g���N�^
PlayerRobot::PlayerRobot()
{
    //�C���N���f�B�u���̃��[�h
    //model = new Model("Data/Model/Mr.Incredible/Mr.Incredible.mdl");
    //���{�b�g�̃��[�h
    //model = new Model("Data/Model/Jammo/Jammo.mdl");

    //���f���̃��[�h
    model = new Model("Data/Model/Player/dualsword_player.mdl");
    //model = new Model("Data/Model/Player/greatsword_player.mdl");
    //model = new Model("Data/Model/Player/gun_player.mdl");

    //�̗͂̐ݒ�
    health = 15;
    maxHealth = 15;

    //���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.y = scale.z = 0.02f;

    //�q�b�g�G�t�F�N�g�ǂݍ���
    hitEffect = new Effect("Data/Effect/Hit.efk");

    attackcount = 0;
    weapon = 0;
    attackcount = 0;
    bullets = 60;
    totalattackcount = 0;
    changetime = 0.0f;

    //�ҋ@�X�e�[�g�֑J��(�匕)
    TransitionIdle_GreatState();
}

//�f�X�g���N�^
PlayerRobot::~PlayerRobot()
{
    delete hitEffect;

    delete model;
}

//�X�V����
void PlayerRobot::Update(float elapsedTime)
{
    //�X�e�[�g���̏���
    switch (state)
    {
        //���햢����
    case State::Idle:
        UpdateIdleState(elapsedTime);
        break;
    case State::Walk:
        UpdateWalkState(elapsedTime);
        break;
    case State::Run:
        UpdateRunState(elapsedTime);
        break;
    case State::Jump:
        UpdateJumpState(elapsedTime);
        break;
    case State::Land:
        UpdateLandState(elapsedTime);
        break;
    case State::Attack:
        UpdateAttackState(elapsedTime);
        break;
    case State::Damage:
        UpdateDamageState(elapsedTime);
        break;
    case State::Death:
        UpdateDeathState(elapsedTime);
        break;
    case State::Revive:
        UpdateReviveState(elapsedTime);
        break;
    case State::Dodge:
        UpdateDodgeState(elapsedTime);
        break;
    case State::Guard:
        UpdateGuardState(elapsedTime);
        break;
    case State::Change:
        UpdateChangeState(elapsedTime);
        break;

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
    case State::Jump_Great:
        UpdateJump_GreatState(elapsedTime);
        break;
    case State::Land_Great:
        UpdateLand_GreatState(elapsedTime);
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
    case State::Jump_Dual:
        UpdateJump_DualState(elapsedTime);
        break;
    case State::Land_Dual:
        UpdateLand_DualState(elapsedTime);
        break;
    case State::Attack_Dual:
        UpdateAttack_DualState(elapsedTime);
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
    case State::Jump_Gun:
        UpdateJump_GunState(elapsedTime);
        break;
    case State::Land_Gun:
        UpdateLand_GunState(elapsedTime);
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
    case State::Change_Gun:
        UpdateChange_GunState(elapsedTime);
        break;

    }



    //���̒Ǐ]
    //LightManager::Instance().GetLight(1)->SetPosition({position.x, position.y + 3.0f, position.z});

    //����ύX�N�[���^�C��
    changetime++;

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

    //���f���s��X�V
    model->UpdateTransform(transform);
    //���I�u�W�F�N�g�̈ʒu�Ƀ��f���\�����邽�߁A
    //�I�u�W�F�N�g�̍s������f���ɓn���ă��f���̍s����v�Z����
}

//�`�揈��
void PlayerRobot::Render(RenderContext& rc, ModelShader* shader)
{
    rc.toonData.edgeWidth = 0.01f;

    shader->Draw(rc, model);
    //���`�悷��ۂɌv�Z���ꂽ���f���̍s������̒��ŗ��p

    //�e�ە`�揈��
    projectileManager.Render(rc, shader);
}

//�f�o�b�O�pGUI�`��
void PlayerRobot::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("PlayerRobot", nullptr, ImGuiWindowFlags_None))
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
void PlayerRobot::DrawDebugPrimitive()
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
void PlayerRobot::CollisionProjectilesVsEnemies()
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


//���n�����Ƃ��ɌĂ΂��
void PlayerRobot::OnLanding()
{
    jumpCount = 0;

    //�������̑��͂����ȏ�Ȃ璅�n�X�e�[�g��
    if (velocity.y < gravity * 5.0f)
    {
        //�_���[�W�A���S�X�e�[�g���͒��n�����Ƃ��ɃX�e�[�g�J�ڂ��Ȃ��悤�ɂ���
        if (state != State::Damage && state != State::Death)
        {
            //���n�X�e�[�g�֑J��
            if (weapon == 0)
            {
                TransitionLandState();
            }
            else if (weapon == 1)
            {
                TransitionIdle_GreatState();
            }
            else if (weapon == 2)
            {
                TransitionIdle_DualState();
            }
            else if (weapon == 3)
            {
                TransitionIdle_GunState();
            }

        }
    }
}

DirectX::XMFLOAT3 PlayerRobot::GetMoveVec() const
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
bool PlayerRobot::InputMove(float elapsedTime)
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

bool PlayerRobot::InputWalk(float elapsedTime)
{
    maxMoveSpeed = 5.0f;
    moveSpeed = 5.0f;

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

bool PlayerRobot::InputRun(float elapsedTime)
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
}

bool PlayerRobot::InputDodge(float elpsedTime)
{
    //�R���g���[���L�[�������ꂽ��

    return false;
}

//�v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
void PlayerRobot::CollisionPlayerVsEnemies()
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
                position = position;
                //�����o����̈ʒu�ݒ�
                //enemy->SetPosition(outPosition);
            }
        }
    }
}

//�m�[�h�ƓG�̏Փˏ���
void PlayerRobot::CollisionNodeVsEnemies(const char* nodeName, float nodeRadius)
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
                    hitEffect->Play(e);
                }
            }
        }
    }
}

//�W�����v���͏���
bool PlayerRobot::InputJump()
{
    //�{�^�����͂ŃW�����v�i�W�����v�񐔐����t���j
    GamePad& gamePad = Input::Instance().GetGamePad();
    if ((gamePad.GetButtonDown() & GamePad::BTN_A) || (GetAsyncKeyState(VK_SPACE) & 0x8001))
    {
        //�W�����v�񐔐���
        if (jumpCount < jumpLimit)
        {
            //�W�����v
            jumpCount++;
            Jump(jumpSpeed);

            //�W�����v���͂���
            return true;
        }
    }
    return false;
}

//�e�ۓ��͏���
void PlayerRobot::InputProjectile()
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
bool PlayerRobot::InputAttack()
{
    attackcount++;

    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();

    //TODO ��Ŗ߂�
    //if ((gamePad.GetButtonDown() & GamePad::BTN_B) || (mouse.GetButton() & Mouse::BTN_LEFT))
    //{
    //    return true;
    //}

    return false;
}

bool PlayerRobot::InputWeapon()
{
    if (GetAsyncKeyState('F') & 0x8000 && changetime >= 120.0f)
    {
        if (weapon == 0)    //����Ȃ��i���͂Ȃ��j
        {
            weapon = 1;
        }
        else if (weapon == 1)   //�匕
        {
            weapon = 2;
        }
        else if (weapon == 2)   //�o��
        {
            weapon = 3;
        }
        else if (weapon == 3)   //�e
        {
            weapon = 1;
        }

        return true;
    }
    return false;
}

bool PlayerRobot::InputGuard(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    Mouse& mouse = Input::Instance().GetMouse();

    if ((gamePad.GetButtonDown() & GamePad::BTN_LEFT_TRIGGER) || (mouse.GetButton() & Mouse::BTN_RIGHT))
    {
        return true;
    }

    return false;
}

bool PlayerRobot::InputReload()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    if ((gamePad.GetButtonDown() & GamePad::BTN_Y) || (GetAsyncKeyState('R') & 0x8000))
    {
        return true;
    }
    return false;
}

//�_���[�W���󂯂��Ƃ��ɌĂ΂��
void PlayerRobot::OnDamaged()
{
    //�_���[�W�X�e�[�g�֑J��
    if (weapon == 0)
    {
        TransitionDamageState();
    }
    else if (weapon == 1)
    {
        TransitionDamage_GreatState();
    }
    else if (weapon == 2)
    {
        TransitionDamage_DualState();
    }
    else if (weapon == 3)
    {
        TransitionDamage_GunState();
    }
}

//���S�����Ƃ��ɌĂ΂��
void PlayerRobot::OnDead()
{
    //���S�X�e�[�g�֑J��
    if (weapon == 0)
    {
        TransitionDeathState();
    }
    else if (weapon == 1)
    {
        TransitionDeath_GreatState();
    }
    else if (weapon == 2)
    {
        TransitionDeath_DualState();
    }
    else if (weapon == 3)
    {
        TransitionDeath_GunState();
    }
}

//�ҋ@�X�e�[�g�֑J��
void PlayerRobot::TransitionIdleState()
{
    state = State::Idle;

    //�ҋ@�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Dual_Idle, true);
}

//�ҋ@�X�e�[�g�X�V����
void PlayerRobot::UpdateIdleState(float elapsedTime)
{
    statenumber = 1;
    //�ړ����͏���
    //if (InputMove(elapsedTime))
    //{
    //    //�ړ��X�e�[�g�֑J��
    //    TransitionMoveState();
    //}

    //���邫���͏���
    if (InputWalk(elapsedTime))
    {
        //�����X�e�[�g�֑J��
        TransitionWalkState();
    }

    //������͏���
    if (InputRun(elapsedTime))
    {
        //����X�e�[�g�֑J��
        TransitionRunState();
    }

    //�W�����v���͏���
    if (InputJump())
    {
        //�W�����v�X�e�[�g�֑J��
        TransitionJumpState();
    }

    //�e�ۓ��͏���
    InputProjectile();

    //�U�����͏���
    if (InputAttack())
    {
        //�U���X�e�[�g�֑J��
        TransitionAttackState();
    }

    //����ύX���͏���
    if (InputWeapon())
    {
        //����ύX�X�e�[�g�֑J��
        TransitionChangeState();
    }

    //�����͏���
    if (InputDodge(elapsedTime))
    {
        //����X�e�[�g�֑J��
        TransitionDodgeState();
    }

    //�K�[�h���͏���
    if (InputGuard(elapsedTime))
    {
        //�K�[�h�X�e�[�g�֑J��
        TransitionGuardState();
    }
}

//�ړ��X�e�[�g�֑J��
void PlayerRobot::TransitionMoveState()
{
    state = State::Move;

    //����A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Dual_Dash, true);
}

//�ړ��X�e�[�g�X�V����
void PlayerRobot::UpdateMoveState(float elapsedTime)
{
    //�ړ����͏���
    if (!InputMove(elapsedTime))
    {
        //�ҋ@�X�e�[�g�֑J��
        TransitionIdleState();
    }

    //����ύX���͏���
    if (InputWeapon())
    {
        //����ύX�X�e�[�g�֑J��
        TransitionChangeState();
    }

    //�W�����v���͏���
    if (InputJump())
    {
        //�W�����v�X�e�[�g�֑J��
        TransitionJumpState();
    }

    //�e�ۓ��͏���
    InputProjectile();

    //�U�����͏���
    if (InputAttack())
    {
        //�U���X�e�[�g�֑J��
        TransitionAttackState();
    }

}

//�W�����v�X�e�[�g�֑J��
void PlayerRobot::TransitionJumpState()
{
    state = State::Jump;

    //�W�����v�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Jump, false);
}

//�W�����v�X�e�[�g�X�V����
void PlayerRobot::UpdateJumpState(float elapsedTime)
{
    //���邫���͏���
    if (InputWalk(elapsedTime))
    {
        //�����X�e�[�g�֑J��
        TransitionWalkState();
    }

    //������͏���
    if (InputRun(elapsedTime))
    {
        //����X�e�[�g�֑J��
        TransitionRunState();
    }

    //�W�����v���͏���
    if (InputJump())
    {
        //�W�����v�X�e�[�g�֑J��
        TransitionJumpState();
    }

    //�e�ۓ��͏���
    InputProjectile();

    //�U�����͏���
    if (InputAttack())
    {
        //�U���X�e�[�g�֑J��
        TransitionAttackState();
    }


    //�����͏���
    //if (InputDodge(elapsedTime))
    //{
    //    //����X�e�[�g�֑J��
    //    TransitionDodgeState();
    //}

    //if (weapon == 0)
    //{
    //    //�K�[�h���͏���
    //    InputGuard();
    //}

    //�W�����v���͏���
    if (InputJump())
    {
        //�W�����v�t���b�v�A�j���[�V�����Đ�
        model->PlayAnimation(Anim_Jump_Flip, false);
    }
    //�W�����v�A�j���[�V�����I����
    if (!model->IsPlayAnimation())
    {
        //�����A�j���[�V�����Đ�
        model->PlayAnimation(Anim_Falling, true);
    }
}

//���n�X�e�[�g�֑J��
void PlayerRobot::TransitionLandState()
{
    state = State::Land;

    //���n�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Landing, false);
}

//���n�X�e�[�g�X�V����
void PlayerRobot::UpdateLandState(float elapsedTIme)
{
    //���n�A�j���[�V�����I����
    if (!model->IsPlayAnimation())
    {
        //�ҋ@�X�e�[�g�֑J��
        TransitionIdleState();
    }
}

//�U���X�e�[�g�֑J��
void PlayerRobot::TransitionAttackState()
{
    state = State::Attack;

    //�U���A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Attack, false);
}

//�U���X�e�[�g�X�V����
void PlayerRobot::UpdateAttackState(float elapsedTime)
{
    //�U���A�j���[�V�����I����
    if (!model->IsPlayAnimation())
    {
        //�ҋ@�X�e�[�g�֑J��
        TransitionIdleState();
    }

    //�C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
    float animationTime = model->GetCurrentAnimationSeconds();
    attackCollisionFlag = animationTime >= 0.3f && animationTime <= 0.4f;
    if (attackCollisionFlag)
    {
        //����m�[�h�ƃG�l�~�[�̏Փˏ���
        CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);
    }
}

//�_���[�W�X�e�[�g�֑J��
void PlayerRobot::TransitionDamageState()
{
    state = State::Damage;

    //�_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_GetHit1, false);
}

//�_���[�W�X�e�[�g�X�V����
void PlayerRobot::UpdateDamageState(float elapsedTime)
{
    //�_���[�W�A�j���[�V�������I�������ҋ@�X�e�[�g�֑J��
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}

//���S�X�e�[�g�֑J��
void PlayerRobot::TransitionDeathState()
{
    state = State::Death;

    //�_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Death, false);
}

//���S�X�e�[�g�X�V����
void PlayerRobot::UpdateDeathState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        //�{�^�����������畜���X�e�[�g�֑J��
        GamePad& gamePad = Input::Instance().GetGamePad();
        if (gamePad.GetButtonDown() & GamePad::BTN_A)
        {
            TransitionReviveState();
        }
    }
}

//�����X�e�[�g�֑J��
void PlayerRobot::TransitionReviveState()
{
    state = State::Revive;

    //�̗͉�
    health = maxHealth;

    //�����A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Revive, false);
}

//�����X�e�[�g�X�V����
void PlayerRobot::UpdateReviveState(float elapsedTime)
{
    //�����A�j���[�V�����I����ɑҋ@�X�e�[�g�֑J��
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}

void PlayerRobot::TransitionDodgeState()
{
    state = State::Dodge;

    //���G���Ԑݒ�
    invincibleTimer = 1.0f;

    //�Ƃ肠�����t���b�v�A�j���[�V�������Đ�
    model->PlayAnimation(Anim_Jump_Flip, false);
}

void PlayerRobot::UpdateDodgeState(float elapsedTime)
{
    //�t���b�v�A�j���[�V�����I����A�ҋ@�X�e�[�g�֑J��
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}

void PlayerRobot::TransitionWalkState()
{
    state = State::Walk;

    //�������[�V�������Đ�
    model->PlayAnimation(Anim_Dual_Dash, true);
}

void PlayerRobot::UpdateWalkState(float elapsedTime)
{
    statenumber = 2;
    //�ړ����͏���
    if (!InputWalk(elapsedTime))
    {
        //�ҋ@�X�e�[�g�֑J��
        TransitionIdleState();
    }

    //������͏���
    if (InputRun(elapsedTime))
    {
        //����X�e�[�g�֑J��
        TransitionRunState();
    }

    //�W�����v���͏���
    if (InputJump())
    {
        //�W�����v�X�e�[�g�֑J��
        TransitionJumpState();
    }

    //�e�ۓ��͏���
    InputProjectile();

    //�U�����͏���
    if (InputAttack())
    {
        //�U���X�e�[�g�֑J��
        TransitionAttackState();
    }

    //����ύX���͏���
    if (InputWeapon())
    {
        //����ύX�X�e�[�g�֑J��
        TransitionChangeState();
    }

    //�����͏���
    if (InputDodge(elapsedTime))
    {
        //����X�e�[�g�֑J��
        TransitionDodgeState();
    }
    //�K�[�h���͏���
    if (InputGuard(elapsedTime))
    {
        //�K�[�h�X�e�[�g�֑J��
        TransitionGuardState();
    }
}

void PlayerRobot::TransitionRunState()
{
    state = State::Run;

    //���胂�[�V�������Đ�
    model->PlayAnimation(Anim_Dual_Dash, true);
}

void PlayerRobot::UpdateRunState(float elapsedTime)
{
    statenumber = 3;

    //������͏���
    if (!InputRun(elapsedTime))
    {
        //�ҋ@�X�e�[�g�֑J��
        TransitionIdleState();
    }
    //�W�����v���͏���
    if (InputJump())
    {
        //�W�����v�X�e�[�g�֑J��
        TransitionJumpState();
    }

    //�e�ۓ��͏���
    InputProjectile();

    //�U�����͏���
    if (InputAttack())
    {
        //�U���X�e�[�g�֑J��
        TransitionAttackState();
    }

    //����ύX���͏���
    if (InputWeapon())
    {
        //����ύX�X�e�[�g�֑J��
        TransitionChangeState();
    }

    //�����͏���
    if (InputDodge(elapsedTime))
    {
        //����X�e�[�g�֑J��
        TransitionDodgeState();
    }

    //�K�[�h���͏���
    if (InputGuard(elapsedTime))
    {
        //�K�[�h�X�e�[�g�֑J��
        TransitionGuardState();
    }
}

void PlayerRobot::TransitionGuardState()
{
    state = State::Guard;

    //�K�[�h���[�V�����Đ�
    model->PlayAnimation(Anim_GetHit1, true);
}

void PlayerRobot::UpdateGuardState(float elapsedTime)
{
    statenumber = 4;

    if (!InputGuard(elapsedTime))
    {
        //�ҋ@�X�e�[�g�֑J��
        TransitionIdleState();
    }

    //�������͏���
    if (InputWalk(elapsedTime))
    {
        //�����X�e�[�g�֑J��
        TransitionWalkState();
    }

    //������͏���
    if (InputRun(elapsedTime))
    {
        //����X�e�[�g�֑J��
        TransitionRunState();
    }
    //�W�����v���͏���
    if (InputJump())
    {
        //�W�����v�X�e�[�g�֑J��
        TransitionJumpState();
    }

    //�e�ۓ��͏���
    InputProjectile();

    //�U�����͏���
    if (InputAttack())
    {
        //�U���X�e�[�g�֑J��
        TransitionAttackState();
    }

    //����ύX���͏���
    if (InputWeapon())
    {
        //����ύX�X�e�[�g�֑J��
        TransitionChangeState();
    }

    //�����͏���
    if (InputDodge(elapsedTime))
    {
        //����X�e�[�g�֑J��
        TransitionDodgeState();
    }
}

void PlayerRobot::TransitionChangeState()
{
    state = State::Change;

    weapon = 1;
    changetime = 0.0f;

    //����ύX���[�V�����Đ�
    model->PlayAnimation(Anim_Jump_Flip, false);
}

void PlayerRobot::UpdateChangeState(float elapsedTime)
{
    //����ύX���[�V�����̍Đ����I���Ύ��̕���֕ύX
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_GreatState();
    }
}

//�匕
void PlayerRobot::TransitionIdle_GreatState()
{
    statenumber = 11;

    //�匕�̑ҋ@�ɑJ��
    state = State::Idle_Great;

    //�ҋ@�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Idle, true);
}

void PlayerRobot::UpdateIdle_GreatState(float elapsedTime)
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

    //�W�����v���͏���
    if (InputJump())
    {
        //�W�����v�X�e�[�g�֑J��
        TransitionJump_GreatState();
    }

    //�U�����͏���
    if (InputAttack())
    {
        //�U���X�e�[�g�֑J��
        TransitionAttack_GreatState();
    }

    //�����͏���
    if (InputDodge(elapsedTime))
    {
        //����X�e�[�g�֑J��
        TransitionDodge_GreatState();
    }

    //�K�[�h���͏���
    if (InputGuard(elapsedTime))
    {
        //�K�[�h�X�e�[�g�֑J��
        TransitionGuard_GreatState();
    }
}


void PlayerRobot::TransitionWalk_GreatState()
{
    statenumber = 12;

    //�匕�̕����ɑJ��
    state = State::Walk_Great;

    //�������[�V�����̍Đ�
    model->PlayAnimation(Anim_Walking, true);
}

void PlayerRobot::UpdateWalk_GreatState(float elapsedTime)
{
    //����ύX���͏���
    if (InputWeapon())
    {
        //����ύX�X�e�[�g�֑J��
        TransitionChange_GreatState();
    }

    //�ړ����͏���
    if (!InputWalk(elapsedTime))
    {
        //�ҋ@�X�e�[�g�֑J��
        TransitionIdle_GreatState();
    }

    //������͏���
    if (InputRun(elapsedTime))
    {
        //����X�e�[�g�֑J��
        TransitionRun_GreatState();
    }

    //�W�����v���͏���
    if (InputJump())
    {
        //�W�����v�X�e�[�g�֑J��
        TransitionJump_GreatState();
    }

    //�U�����͏���
    if (InputAttack())
    {
        //�U���X�e�[�g�֑J��
        TransitionAttack_GreatState();
    }

    //�����͏���
    if (InputDodge(elapsedTime))
    {
        //����X�e�[�g�֑J��
        TransitionDodge_GreatState();
    }

    //�K�[�h���͏���
    if (InputGuard(elapsedTime))
    {
        //�K�[�h�X�e�[�g�֑J��
        TransitionGuard_GreatState();
    }
}

void PlayerRobot::TransitionRun_GreatState()
{
    statenumber = 13;
    state = State::Run_Great;

    //���胂�[�V�������Đ�
    model->PlayAnimation(Anim_Running, true);
}

void PlayerRobot::UpdateRun_GreatState(float elapsedTime)
{
    //����ύX���͏���
    if (InputWeapon())
    {
        //����ύX�X�e�[�g�֑J��
        TransitionChange_GreatState();
    }

    //������͏���
    if (!InputRun(elapsedTime))
    {
        //�ҋ@�X�e�[�g�֑J��
        TransitionIdle_GreatState();
    }
    //�W�����v���͏���
    if (InputJump())
    {
        //�W�����v�X�e�[�g�֑J��
        TransitionJump_GreatState();
    }

    //�U�����͏���
    if (InputAttack())
    {
        //�U���X�e�[�g�֑J��
        TransitionAttack_GreatState();
    }

    //�����͏���
    if (InputDodge(elapsedTime))
    {
        //����X�e�[�g�֑J��
        TransitionDodge_GreatState();
    }

    //�K�[�h���͏���
    if (InputGuard(elapsedTime))
    {
        //�K�[�h�X�e�[�g�֑J��
        TransitionGuard_GreatState();
    }
}

void PlayerRobot::TransitionJump_GreatState()
{
    statenumber = 14;
    state = State::Jump_Great;

    //�W�����v�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Jump, false);
}

void PlayerRobot::UpdateJump_GreatState(float elapsedTime)
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
    if (InputAttack())
    {
        //�U���X�e�[�g�֑J��
        TransitionAttack_GreatState();
    }

    //�W�����v�A�j���[�V�����I����
    if (!model->IsPlayAnimation())
    {
        //�����A�j���[�V�����Đ�
        model->PlayAnimation(Anim_Falling, true);
    }
}

void PlayerRobot::TransitionLand_GreatState()
{
    statenumber = 15;
    state = State::Land_Great;

    //���n�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Landing, false);
}

void PlayerRobot::UpdateLand_GreatState(float elapsedTIme)
{
    //���n�A�j���[�V�����I����
    if (!model->IsPlayAnimation())
    {
        //�ҋ@�X�e�[�g�֑J��
        TransitionIdle_GreatState();
    }
}

void PlayerRobot::TransitionAttack_GreatState()
{
    statenumber = 16;
    state = State::Attack_Great;

    //�U���A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Attack, false);
}

void PlayerRobot::UpdateAttack_GreatState(float elapsedTime)
{
    //�U���A�j���[�V�����I����
    if (!model->IsPlayAnimation())
    {
        //�ҋ@�X�e�[�g�֑J��
        TransitionIdle_GreatState();
    }

    //�C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
    float animationTime = model->GetCurrentAnimationSeconds();
    attackCollisionFlag = animationTime >= 0.3f && animationTime <= 0.4f;
    if (attackCollisionFlag)
    {
        //����m�[�h�ƃG�l�~�[�̏Փˏ���
        CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);
    }
}

void PlayerRobot::TransitionDamage_GreatState()
{
    statenumber = 17;
    state = State::Damage_Great;

    //�_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_GetHit1, false);
}

void PlayerRobot::UpdateDamage_GreatState(float elapsedTime)
{
    //�_���[�W�A�j���[�V�������I�������ҋ@�X�e�[�g�֑J��
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_GreatState();
    }
}

void PlayerRobot::TransitionDeath_GreatState()
{
    statenumber = 18;
    state = State::Death_Great;

    //�_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Death, false);
}

void PlayerRobot::UpdateDeath_GreatState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        //�A�j���[�V�����Đ����I�������Q�[���I�[�o�[�ɂȂ�悤�ɂ���
    }
}

void PlayerRobot::TransitionDodge_GreatState()
{
    statenumber = 19;
    state = State::Dodge_Great;

    //���G���Ԑݒ�
    invincibleTimer = 1.0f;

    //�Ƃ肠�����t���b�v�A�j���[�V�������Đ�
    model->PlayAnimation(Anim_Jump_Flip, false);
}

void PlayerRobot::UpdateDodge_GreatState(float elapsedTime)
{
    //�t���b�v�A�j���[�V�����I����A�ҋ@�X�e�[�g�֑J��
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_GreatState();
    }
}

void PlayerRobot::TransitionGuard_GreatState()
{
    statenumber = 20;
    state = State::Guard_Great;

    //�K�[�h���[�V�����Đ�
    model->PlayAnimation(Anim_GetHit1, true);
}

void PlayerRobot::UpdateGuard_GreatState(float elapsedTime)
{
    if (!InputGuard(elapsedTime))
    {
        //�ҋ@�X�e�[�g�֑J��
        TransitionIdle_GreatState();
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
    //�W�����v���͏���
    if (InputJump())
    {
        //�W�����v�X�e�[�g�֑J��
        TransitionJump_GreatState();
    }

    //�U�����͏���
    if (InputAttack())
    {
        //�U���X�e�[�g�֑J��
        TransitionAttack_GreatState();
    }

    //�����͏���
    if (InputDodge(elapsedTime))
    {
        //����X�e�[�g�֑J��
        TransitionDodge_GreatState();
    }
}

void PlayerRobot::TransitionChange_GreatState()
{
    state = State::Change_Great;
    weapon = 2;
    changetime = 0.0f;

    //����ύX���[�V�����Đ�
    model->PlayAnimation(Anim_Jump_Flip, false);
}

void PlayerRobot::UpdateChange_GreatState(float elapsedTime)
{
    //����ύX���[�V�����̍Đ����I���Ύ��̕���֕ύX
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_DualState();
    }
}

//�o��
void PlayerRobot::TransitionIdle_DualState()
{
    statenumber = 21;

    //�o���̑ҋ@�ɑJ��
    state = State::Idle_Dual;

    //�ҋ@�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Idle, true);
}

void PlayerRobot::UpdateIdle_DualState(float elapsedTime)
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

    //�W�����v���͏���
    if (InputJump())
    {
        //�W�����v�X�e�[�g�֑J��
        TransitionJump_DualState();
    }

    //�U�����͏���
    if (InputAttack())
    {
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

void PlayerRobot::TransitionWalk_DualState()
{
    statenumber = 22;

    //�o���̕����ɑJ��
    state = State::Walk_Dual;

    //�������[�V�����̍Đ�
    model->PlayAnimation(Anim_Walking, true);
}

void PlayerRobot::UpdateWalk_DualState(float elapsedTime)
{
    //����ύX���͏���
    if (InputWeapon())
    {
        //����ύX�X�e�[�g�֑J��
        TransitionChange_DualState();
    }

    //�ړ����͏���
    if (!InputWalk(elapsedTime))
    {
        //�ҋ@�X�e�[�g�֑J��
        TransitionIdle_DualState();
    }

    //������͏���
    if (InputRun(elapsedTime))
    {
        //����X�e�[�g�֑J��
        TransitionRun_DualState();
    }

    //�W�����v���͏���
    if (InputJump())
    {
        //�W�����v�X�e�[�g�֑J��
        TransitionJump_DualState();
    }

    //�U�����͏���
    if (InputAttack())
    {
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

void PlayerRobot::TransitionRun_DualState()
{
    statenumber = 23;
    state = State::Run_Dual;

    //���胂�[�V�������Đ�
    model->PlayAnimation(Anim_Running, true);
}

void PlayerRobot::UpdateRun_DualState(float elapsedTime)
{
    //����ύX���͏���
    if (InputWeapon())
    {
        //����ύX�X�e�[�g�֑J��
        TransitionChange_DualState();
    }

    //������͏���
    if (!InputRun(elapsedTime))
    {
        //�ҋ@�X�e�[�g�֑J��
        TransitionIdle_DualState();
    }
    //�W�����v���͏���
    if (InputJump())
    {
        //�W�����v�X�e�[�g�֑J��
        TransitionJump_DualState();
    }

    //�U�����͏���
    if (InputAttack())
    {
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

void PlayerRobot::TransitionJump_DualState()
{
    statenumber = 24;
    state = State::Jump_Dual;

    //�W�����v�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Jump, false);
}

void PlayerRobot::UpdateJump_DualState(float elapsedTime)
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
    if (InputAttack())
    {
        //�U���X�e�[�g�֑J��
        TransitionAttack_DualState();
    }

    //�W�����v�A�j���[�V�����I����
    if (!model->IsPlayAnimation())
    {
        //�����A�j���[�V�����Đ�
        model->PlayAnimation(Anim_Falling, true);
    }
}

void PlayerRobot::TransitionLand_DualState()
{
    statenumber = 25;
    state = State::Land_Great;

    //���n�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Landing, false);
}

void PlayerRobot::UpdateLand_DualState(float elapsedTIme)
{
    //���n�A�j���[�V�����I����
    if (!model->IsPlayAnimation())
    {
        //�ҋ@�X�e�[�g�֑J��
        TransitionIdle_DualState();
    }
}

void PlayerRobot::TransitionAttack_DualState()
{
    statenumber = 26;
    state = State::Attack_Dual;

    //�U���A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Attack, false);
}

void PlayerRobot::UpdateAttack_DualState(float elapsedTime)
{
    //�U���A�j���[�V�����I����
    if (!model->IsPlayAnimation())
    {
        //�ҋ@�X�e�[�g�֑J��
        TransitionIdle_DualState();
    }

    //�C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
    float animationTime = model->GetCurrentAnimationSeconds();
    attackCollisionFlag = animationTime >= 0.3f && animationTime <= 0.4f;
    if (attackCollisionFlag)
    {
        //����m�[�h�ƃG�l�~�[�̏Փˏ���
        CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);
    }
}

void PlayerRobot::TransitionDamage_DualState()
{
    statenumber = 27;
    state = State::Damage_Dual;

    //�_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_GetHit1, false);
}

void PlayerRobot::UpdateDamage_DualState(float elapsedTime)
{
    //�_���[�W�A�j���[�V�������I�������ҋ@�X�e�[�g�֑J��
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_DualState();
    }
}

void PlayerRobot::TransitionDeath_DualState()
{
    statenumber = 28;
    state = State::Death_Dual;

    //�_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Death, false);
}

void PlayerRobot::UpdateDeath_DualState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        //�A�j���[�V�����Đ����I�������Q�[���I�[�o�[�ɂȂ�悤�ɂ���
    }
}

void PlayerRobot::TransitionDodge_DualState()
{
    statenumber = 29;
    state = State::Dodge_Dual;

    //���G���Ԑݒ�
    invincibleTimer = 1.0f;

    //�Ƃ肠�����t���b�v�A�j���[�V�������Đ�
    model->PlayAnimation(Anim_Jump_Flip, false);
}

void PlayerRobot::UpdateDodge_DualState(float elapsedTime)
{
    //�t���b�v�A�j���[�V�����I����A�ҋ@�X�e�[�g�֑J��
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_DualState();
    }
}

void PlayerRobot::TransitionChange_DualState()
{
    state = State::Change_Dual;

    weapon = 3;
    changetime = 0.0f;

    //����ύX���[�V�����Đ�
    model->PlayAnimation(Anim_Jump_Flip, false);
}

void PlayerRobot::UpdateChange_DualState(float elapsedTime)
{
    //����ύX���[�V�����̍Đ����I���Ύ��̕���֕ύX
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_GunState();
    }
}

//�e
void PlayerRobot::TransitionIdle_GunState()
{
    statenumber = 31;

    //�e�̑ҋ@�ɑJ��
    state = State::Idle_Gun;

    //�ҋ@�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Idle, true);
}

void PlayerRobot::UpdateIdle_GunState(float elapsedTime)
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

    //�W�����v���͏���
    if (InputJump())
    {
        //�W�����v�X�e�[�g�֑J��
        TransitionJump_GunState();
    }

    //�U�����͏���
    if (InputAttack())
    {
        //�U���X�e�[�g�֑J��
        TransitionAttack_GunState();
    }

    //�����͏���
    if (InputDodge(elapsedTime))
    {
        //����X�e�[�g�֑J��
        TransitionDodge_GunState();
    }
}

void PlayerRobot::TransitionWalk_GunState()
{
    statenumber = 32;

    //�e�̕����ɑJ��
    state = State::Walk_Gun;

    //�������[�V�����̍Đ�
    model->PlayAnimation(Anim_Walking, true);
}

void PlayerRobot::UpdateWalk_GunState(float elapsedTime)
{
    //����ύX���͏���
    if (InputWeapon())
    {
        //����ύX�X�e�[�g�֑J��
        TransitionChange_GunState();
    }

    //�ړ����͏���
    if (!InputWalk(elapsedTime))
    {
        //�ҋ@�X�e�[�g�֑J��
        TransitionIdle_GunState();
    }

    //������͏���
    if (InputRun(elapsedTime))
    {
        //����X�e�[�g�֑J��
        TransitionRun_GunState();
    }

    //�W�����v���͏���
    if (InputJump())
    {
        //�W�����v�X�e�[�g�֑J��
        TransitionJump_GunState();
    }

    //�U�����͏���
    if (InputAttack())
    {
        //�U���X�e�[�g�֑J��
        TransitionAttack_GunState();
    }

    //�����͏���
    if (InputDodge(elapsedTime))
    {
        //����X�e�[�g�֑J��
        TransitionDodge_GunState();
    }
}

void PlayerRobot::TransitionRun_GunState()
{
    statenumber = 33;
    state = State::Run_Gun;

    //���胂�[�V�������Đ�
    model->PlayAnimation(Anim_Running, true);
}

void PlayerRobot::UpdateRun_GunState(float elapsedTime)
{
    //����ύX���͏���
    if (InputWeapon())
    {
        //����ύX�X�e�[�g�֑J��
        TransitionChange_GunState();
    }

    //������͏���
    if (!InputRun(elapsedTime))
    {
        //�ҋ@�X�e�[�g�֑J��
        TransitionIdle_GunState();
    }
    //�W�����v���͏���
    if (InputJump())
    {
        //�W�����v�X�e�[�g�֑J��
        TransitionJump_GunState();
    }

    //�U�����͏���
    if (InputAttack())
    {
        //�U���X�e�[�g�֑J��
        TransitionAttack_GunState();
    }

    //�����͏���
    if (InputDodge(elapsedTime))
    {
        //����X�e�[�g�֑J��
        TransitionDodge_GunState();
    }
}

void PlayerRobot::TransitionJump_GunState()
{
    statenumber = 34;
    state = State::Jump_Gun;

    //�W�����v�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Jump, false);
}

void PlayerRobot::UpdateJump_GunState(float elapsedTime)
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
    if (InputAttack())
    {
        //�U���X�e�[�g�֑J��
        TransitionAttack_GunState();
    }

    //�W�����v�A�j���[�V�����I����
    if (!model->IsPlayAnimation())
    {
        //�����A�j���[�V�����Đ�
        model->PlayAnimation(Anim_Falling, true);
    }
}

void PlayerRobot::TransitionLand_GunState()
{
    statenumber = 35;
    state = State::Land_Gun;

    //���n�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Landing, false);
}

void PlayerRobot::UpdateLand_GunState(float elapsedTIme)
{
    //���n�A�j���[�V�����I����
    if (!model->IsPlayAnimation())
    {
        //�ҋ@�X�e�[�g�֑J��
        TransitionIdle_DualState();
    }
}

void PlayerRobot::TransitionAttack_GunState()
{
    statenumber = 36;
    state = State::Attack_Gun;

    //�U���A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Attack, false);
}

void PlayerRobot::UpdateAttack_GunState(float elapsedTime)
{
    //�U���A�j���[�V�����I����
    if (!model->IsPlayAnimation())
    {
        //�ҋ@�X�e�[�g�֑J��
        TransitionIdle_DualState();
    }

    //�C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
    float animationTime = model->GetCurrentAnimationSeconds();
    attackCollisionFlag = animationTime >= 0.3f && animationTime <= 0.4f;
    if (attackCollisionFlag)
    {
        //����m�[�h�ƃG�l�~�[�̏Փˏ���
        CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);
    }
}

void PlayerRobot::TransitionDamage_GunState()
{
    statenumber = 37;
    state = State::Damage_Gun;

    //�_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_GetHit1, false);
}

void PlayerRobot::UpdateDamage_GunState(float elapsedTime)
{
    //�_���[�W�A�j���[�V�������I�������ҋ@�X�e�[�g�֑J��
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_GunState();
    }
}

void PlayerRobot::TransitionDeath_GunState()
{
    statenumber = 38;
    state = State::Death_Gun;

    //�_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Death, false);
}

void PlayerRobot::UpdateDeath_GunState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        //�A�j���[�V�����Đ����I�������Q�[���I�[�o�[�ɂȂ�悤�ɂ���
    }
}

void PlayerRobot::TransitionDodge_GunState()
{
    statenumber = 39;
    state = State::Dodge_Gun;

    //���G���Ԑݒ�
    invincibleTimer = 1.0f;

    //�Ƃ肠�����t���b�v�A�j���[�V�������Đ�
    model->PlayAnimation(Anim_Jump_Flip, false);
}

void PlayerRobot::UpdateDodge_GunState(float elapsedTime)
{
    //�t���b�v�A�j���[�V�����I����A�ҋ@�X�e�[�g�֑J��
    if (!model->IsPlayAnimation())
    {
        TransitionIdle_DualState();
    }
}

void PlayerRobot::TransitionReload_GunState()
{
    statenumber = 40;
    state = State::Reload_Gun;

    //�����[�h�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Revive, false);
}

void PlayerRobot::UpdateReload_GunState(float elapsedTime)
{
    //�����[�h�ɂ����鎞��
    //reloadtime--;

    if (!model->IsPlayAnimation())
    {
        //�e�򐔂����ɖ߂�
        bullets = 60;

        //�A�j���[�V�������I�������ҋ@��Ԃɖ߂�
        TransitionIdle_GunState();
    }

}

void PlayerRobot::TransitionChange_GunState()
{
    state = State::Change_Gun;

    weapon = 0;
    changetime = 0.0f;

    //����ύX���[�V�����Đ�
    model->PlayAnimation(Anim_Jump_Flip, false);
}

void PlayerRobot::UpdateChange_GunState(float elapsedTime)
{
    //����ύX���[�V�����̍Đ����I���Ύ��̕���֕ύX
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();
    }
}
