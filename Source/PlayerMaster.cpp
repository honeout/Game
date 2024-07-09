#include <imgui.h>
#include "PlayerMaster.h"
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraite.h"
#include "ProjectileHoming.h"
#include "LightManager.h"
#include "PlayerManager.h"

//�R���X�g���N�^
PlayerMaster::PlayerMaster()
{
    //���f���̃��[�h
    model = new Model("Data/Model/Player/player_master.mdl");

    //�̗͂̐ݒ�
    health = 15;
    maxHealth = 15;

    //���f�����傫���̂ŃX�P�[�����O
    scale.x = scale.y = scale.z = 0.02f;

    attackcount = 0;
    weapon = 0;

    changetime = 0.0f;

    //�ҋ@�X�e�[�g�֑J��
    TransitionIdle_MasterState();
    weaponModel = new Weapon();
}

//�f�X�g���N�^
PlayerMaster::~PlayerMaster()
{
    //����I����
    delete weaponModel;
    weaponModel = nullptr;



    delete model;
}

//�X�V����
void PlayerMaster::Update(float elapsedTime)
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
    if (position.y < -8.627f)
    {
        position.y = -8.627f;
    }

    if (PlayerManager::Instance().GetWeapon() == 3)
    {
        //�X�e�[�g���̏���
        switch (state)
        {
            //�o��
        case State::Idle_Master:
            UpdateIdle_MasterState(elapsedTime);
            break;
        case State::Change_Master:
            UpdateChange_MasterState(elapsedTime);
            break;

        }

        //����ύX�N�[���^�C��
        changetimer++;

        //���͏����X�V
        UpdateVelocity(elapsedTime);


        //�I�u�W�F�N�g�s����X�V
        UpdateTransform();

        //���f���A�j���[�V�����X�V����
        model->UpdateAnimation(elapsedTime);


        weaponModel->Update(elapsedTime);

        //���f���s��X�V
        model->UpdateTransform(transform);
        //���I�u�W�F�N�g�̈ʒu�Ƀ��f���\�����邽�߁A
        //�I�u�W�F�N�g�̍s������f���ɓn���ă��f���̍s����v�Z����
    }
}

//�`�揈��
void PlayerMaster::Render(RenderContext& rc, ModelShader* shader)
{
    rc.toonData.edgeWidth = 0.02f;


    weaponModel->Render(rc, shader);

    shader->Draw(rc, model);
    //���`�悷��ۂɌv�Z���ꂽ���f���̍s������̒��ŗ��p

    //�e�ە`�揈��
    projectileManager.Render(rc, shader);
}

//�f�o�b�O�pGUI�`��
void PlayerMaster::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("PlayerMaster", nullptr, ImGuiWindowFlags_None))
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
void PlayerMaster::DrawDebugPrimitive()
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
            MasterRadius,
            DirectX::XMFLOAT4(1, 0, 0, 1)
        );
        debugRenderer->DrawSphere(DirectX::XMFLOAT3(
            swordBone_L->worldTransform._41,
            swordBone_L->worldTransform._42,
            swordBone_L->worldTransform._43),
            MasterRadius,
            DirectX::XMFLOAT4(1, 0, 0, 1)
        );
    }
}




DirectX::XMFLOAT3 PlayerMaster::GetMoveVec() const
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


//�o��
void PlayerMaster::TransitionIdle_MasterState()
{
    statenumber = 21;

    //�o���̑ҋ@�ɑJ��
    state = State::Idle_Master;

    //�ҋ@�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Master_Idle, true);
}

void PlayerMaster::UpdateIdle_MasterState(float elapsedTime)
{
  

    TransitionChange_MasterState();
}


void PlayerMaster::TransitionChange_MasterState()
{
    state = State::Change_Master;

    weapon = 3;
    changetime = 0.0f;

    //PlayerManager::Instance().SetStartAnimation(true);

    //����ύX���[�V�����Đ�
    //model->PlayAnimation(Anim_Master_Change, false);
    model->PlayAnimation(Anim_Master_Switch, false);

    //PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetWeapon())
    //    ->SetChangeTimeMode(true);
    changetimemode = true;
}

void PlayerMaster::UpdateChange_MasterState(float elapsedTime)
{
    //�v���C���[�̈ʒu����Ɏ擾��������
    PlayerManager& playerMgr = PlayerManager::Instance();


    //Model::Node* node = model->FindNode("player_right_arm1");
    //DirectX::XMFLOAT3 hand_position = {
    //    node->worldTransform._41,
    //    node->worldTransform._42,
    //    node->worldTransform._43
    //};


    //SetSetHandPositionR(hand_position);

    DirectX::XMFLOAT3 position_Master = position;
    DirectX::XMFLOAT3 angle_Master = angle;

    //playerMgr.GetPlayer(PlayerManager::Instance().GetWeapon())->
    //    SetHandPositionR(hand_position);


    shackChack = false;
        // �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓˏ���������
    float animationTime = model->GetCurrentAnimationSeconds();

    if (animationTime >= 0.5f && animationTime <= 0.8f)
        shackChack = true;


    if (!model->IsPlayAnimation())
    {
        changetimemode = false;
        DirectX::XMFLOAT3 position_Master = position;
        DirectX::XMFLOAT3 angle_Master = angle;
        playerMgr.SetWeaponFlag(true);
        //PlayerManager::Instance().SetWeapon(0);
        if(playerMgr.GetBeforeWeapon() >= 2)
        playerMgr.SetWeapon(0);
        else
        {
            playerMgr.SetWeapon(playerMgr.GetBeforeWeapon() + 1);
        }
        Player* player = playerMgr.GetPlayer(playerMgr.GetWeapon());
        position_Master.y += 0.1f;
        player->SetPosition(position_Master);
        player->SetAngle(angle_Master);
        player->SetChangeTimer(0.0f);
        player->SetHP(health);
        // �h��Ȃ�
        shackStrongChack = false;

        TransitionIdle_MasterState();
        position = { 0,1000000000000.0f,0 };
    }

    //����ύX���[�V�����̍Đ����I���Ύ��̕���֕ύX
    //if (!model->IsPlayAnimation())
    //{
    //    //TODOManager��model���̂�ύX����
    //}
}
