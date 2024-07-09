#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "EnemyBOSS.h"
#include "Mathf.h"
#include "imgui.h"

CameraController::CameraController()
{
    Camera& camera = Camera::Instance();
    camera.SetLookAt(
        DirectX::XMFLOAT3(0, 1, 1),
        DirectX::XMFLOAT3(0, 0, 0),
        DirectX::XMFLOAT3(0, 1, 0)
    );
    camera.SetPerspectiveFov(
        DirectX::XMConvertToRadians(45),
        1280.0f / 720.0f,
        1.0f,
        10000.0f
    );
    DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&camera.GetEye());
    DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&camera.GetFocus());
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Eye, Focus);
    DirectX::XMVECTOR Length = DirectX::XMVector3Length(Vec);
    DirectX::XMStoreFloat(&distance, Length);

    DirectX::XMFLOAT3 vec;
    DirectX::XMStoreFloat3(&vec, DirectX::XMVectorDivide(Vec, Length));

    float f = sqrtf(vec.x * vec.x + vec.z * vec.z);

    rotateX = atan2f(vec.y, f);
    rotateY = atan2f(vec.x, vec.z);
    camera_operate = 0;
    cameratime = 0.0f;
}

// �X�V����
void CameraController::Update(float elapsedTime)
{
    cameratime++;
    Mouse& mouse = Input::Instance().GetMouse();

    EnemyManager& enemyManager = EnemyManager::Instance();
    int enemyCount = enemyManager.GetEnemyCount();

    //range = 17;

    //for (int i = 0; i < enemyCount; ++i)
    //{
    //    Enemy* enemy = enemyManager.GetEnemy(i);

    //    if (enemy->GetShackCheck())
    //        CameraShake(0.8f, 0.8f, 0.8f, 0.8f, 0.01f);
    //}

    //if (((GetAsyncKeyState('O') & 0x8000) && (GetAsyncKeyState(VK_SHIFT) & 0x8000) && cameratime >= 60.0f) || (mouse.GetButton() & Mouse::BTN_RIGHT && cameratime >= 60.0f)) {
    //if (((GetAsyncKeyState('O') & 0x8000) && (GetAsyncKeyState(VK_SHIFT) & 0x8000) && cameratime >= 60.0f)){
    //    // ������A�L�[�ɑ΂��鏈����ǉ�
    //    if (camera_operate == 0)
    //    {
    //        camera_operate = 1;
    //        cameratime = 0.0f;
    //    }
    //    else if (camera_operate == 1)
    //    {
    //        camera_operate = 2;
    //        cameratime = 0.0f;
    //    }
    //    else if (camera_operate == 2)
    //    {
    //        camera_operate = 0;
    //        cameratime = 0.0f;
    //    }
    //}
    //camera_operate = 1;
    if (camera_operate == 0)
    {
        //�}�E�X�J�[�\�����\���ɂ���
        ::ShowCursor(FALSE);
        ::SetCursorPos(960, 540);
        //�}�E�X����
        Mouse& mouse = Input::Instance().GetMouse();
        //float moveX = (mouse.GetPositionX() - mouse.GetOldPositionX()) * 0.02f;
        //float moveY = (mouse.GetPositionY() - mouse.GetOldPositionY()) * 0.02f;

        float moveX = static_cast<float>(mouse.GetPositionX() - 960) * 0.02f;
        float moveY = static_cast<float>(mouse.GetPositionY() - 540) * 0.02f;
        Camera& camera = Camera::Instance();
        // �����s��𐶐�
        DirectX::XMMATRIX V;
        {
            DirectX::XMVECTOR up{ DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) };
            // �}�E�X����
            {
                //if (::GetAsyncKeyState(VK_RBUTTON) & 0x8000)
                //{
                    // Y����]
                rotateY += moveX * 0.3f;
                if (rotateY > DirectX::XM_PI)
                    rotateY -= DirectX::XM_2PI;
                else if (rotateY < -DirectX::XM_PI)
                    rotateY += DirectX::XM_2PI;
                rotateX += moveY * 0.3f;
                //if (rotateX > DirectX::XMConvertToRadians(89.9f))
                //rotateX = DirectX::XMConvertToRadians(89.9f);
                //else if (rotateX < -DirectX::XMConvertToRadians(89.9f))
                //    rotateX = -DirectX::XMConvertToRadians(89.9f);
                if (rotateX > maxAngleX)
                    rotateX = maxAngleX;
                else if (rotateX < minAngleX)
                    rotateX = minAngleX;
                //}
                else if (::GetAsyncKeyState(VK_MBUTTON) & 0x8000)
                {
                    V = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&camera_position),
                        DirectX::XMLoadFloat3(&camera_focus),
                        up);
                    DirectX::XMFLOAT4X4 W;
                    DirectX::XMStoreFloat4x4(&W, DirectX::XMMatrixInverse(nullptr, V));
                    // ���s�ړ�
                    float s = distance * 0.035f;
                    float x = moveX * s;
                    float y = moveY * s;
                    camera_focus.x -= W._11 * x;
                    camera_focus.y -= W._12 * x;
                    camera_focus.z -= W._13 * x;
                    camera_focus.x += W._21 * y;
                    camera_focus.y += W._22 * y;
                    camera_focus.z += W._23 * y;
                }
                //if (mouse.GetWheel() != 0)	// �Y�[��
                //{
                //    distance -= static_cast<float>(mouse.GetWheel()) * distance * 0.001f;
                //}
            }
            float sx = ::sinf(rotateX), cx = ::cosf(rotateX);
            float sy = ::sinf(rotateY), cy = ::cosf(rotateY);
            DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&camera_focus);
            DirectX::XMVECTOR Front = DirectX::XMVectorSet(-cx * sy, -sx, -cx * cy, 0.0f);
            DirectX::XMVECTOR Distance = DirectX::XMVectorSet(distance, distance, distance, 0.0f);
            Front = DirectX::XMVectorMultiply(Front, Distance);
            DirectX::XMVECTOR Eye = DirectX::XMVectorSubtract(Focus, Front);
            DirectX::XMStoreFloat3(&camera_position, Eye);

            DirectX::XMFLOAT3 front;
            DirectX::XMStoreFloat3(&front, Front);  //�O�����x�N�g������ꂽ

            DirectX::XMFLOAT3 Target = target;

            //target.x -= 1.0f;
            target.y += 1.0f;
            //target.z -= 1.0f;

            //�����_������x�N�g�������ɍs���ċ������ꂽ�J�������_�����߂�
            DirectX::XMFLOAT3 eye;
            eye.x = Target.x - front.x * range;
            eye.y = Target.y - front.y * range;
            eye.z = Target.z - front.z * range;


            // �J�����Ɏ��_�𒍎��_��ݒ�
            Camera::Instance().SetLookAt(camera_position, camera_focus, { 0, 1, 0 });
            Camera::Instance().SetLookAt(eye, target, { 0, 1, 0 });
        }
        //if (mouse.GetWheel() != 0)
        //{
        //    // �Y�[��
           // distance -= static_cast<float>(mouse.GetWheel()) * distance * 0.001f;
        //}
    }
    else if (camera_operate == 1)
    {
        //�R���g���[���[�A�L�[����
        GamePad& gamePad = Input::Instance().GetGamePad();
        float ax = gamePad.GetAxisRX();
        float ay = gamePad.GetAxisRY();

        //�J�����̉�]���x
        float speed = rollSpeed * elapsedTime;

        //�X�e�B�b�N�̓��͒l�ɍ��킹��X����Y������]

        angle.x += ay * speed;
        angle.y -= ax * speed;
        //if���Ȃ�Ă��Ȃ��Ă����͂���ł���
        //�L�[�{�[�h��I�AJ�AK�AL�ŃJ������]���ł���͂��I

        //X���̃J������]�𐧌�
        if (angle.x < minAngleX)
        {
            angle.x = minAngleX;
        }
        //�ݒ肵���l��菬�����Ȃ肻���������炻���菬�����Ȃ�Ȃ��悤�ɐݒ肵���l�ɂȂ�悤�ɂ���
        if (angle.x > maxAngleX)
        {
            angle.x = maxAngleX;
        }
        //��̋t������

        //Y���̉�]�l��-3.14~3,14�Ɏ��܂�悤�ɂ���
        if (angle.y < -DirectX::XM_PI)
        {
            angle.y += DirectX::XM_2PI;
        }
        if (angle.y > DirectX::XM_PI)
        {
            angle.y -= DirectX::XM_2PI;
        }

        //�J������]�l����]�s��ɕϊ�
        DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z); //��XMMatrixRotationRollPitchYaw�֐��ŃI�C���[�p�x�Ɋ�Â��ĉ�]�s����쐬

        //��]�s�񂩂�O�����x�N�g�������o��
        DirectX::XMVECTOR Front = Transform.r[2];
        DirectX::XMFLOAT3 front;
        DirectX::XMStoreFloat3(&front, Front);  //�O�����x�N�g������ꂽ

        DirectX::XMFLOAT3 Target = target;

        //target.x -= 1.0f;
        target.y += 1.0f;
        //target.z -= 1.0f;

        //�����_������x�N�g�������ɍs���ċ������ꂽ�J�������_�����߂�
        DirectX::XMFLOAT3 eye;
        eye.x = Target.x - front.x * range; //range��ς����player�Ƃ̃J�����̋�����ς�����̂ŗ��p����
        eye.y = Target.y - front.y * range;
        eye.z = Target.z - front.z * range;


        //�J�����̎��_�ƒ����_��ݒ�
        Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
    }
    else if (camera_operate == 2)
    {
        //�}�E�X����
        Mouse& mouse = Input::Instance().GetMouse();
        moveX = (mouse.GetPositionX() - mouse.GetOldPositionX()) * 0.02f;
        moveY = (mouse.GetPositionY() - mouse.GetOldPositionY()) * 0.02f;
        Camera& camera = Camera::Instance();
        // �����s��𐶐�
        DirectX::XMMATRIX V;
        {
            DirectX::XMVECTOR up{ DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) };
            // �}�E�X����
            {
                //if (::GetAsyncKeyState(VK_RBUTTON) & 0x8000)
                //{
                    // Y����]
                rotateY += moveX * 0.3f;
                if (rotateY > DirectX::XM_PI)
                    rotateY -= DirectX::XM_2PI;
                else if (rotateY < -DirectX::XM_PI)
                    rotateY += DirectX::XM_2PI;
                // X����]
                rotateX += moveY * 0.3f;
                if (rotateX > DirectX::XMConvertToRadians(89.9f))
                    rotateX = DirectX::XMConvertToRadians(89.9f);
                else if (rotateX < -DirectX::XMConvertToRadians(89.9f))
                    rotateX = -DirectX::XMConvertToRadians(89.9f);
                //}
                else if (::GetAsyncKeyState(VK_MBUTTON) & 0x8000)
                {
                    V = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&camera_position),
                        DirectX::XMLoadFloat3(&camera_focus),
                        up);
                    DirectX::XMFLOAT4X4 W;
                    DirectX::XMStoreFloat4x4(&W, DirectX::XMMatrixInverse(nullptr, V));
                    // ���s�ړ�
                    float s = distance * 0.035f;
                    float x = moveX * s;
                    float y = moveY * s;
                    camera_focus.x -= W._11 * x;
                    camera_focus.y -= W._12 * x;
                    camera_focus.z -= W._13 * x;
                    camera_focus.x += W._21 * y;
                    camera_focus.y += W._22 * y;
                    camera_focus.z += W._23 * y;
                }
                if (mouse.GetWheel() != 0)	// �Y�[��
                {
                    distance -= static_cast<float>(mouse.GetWheel()) * distance * 0.001f;
                }
            }
            float sx = ::sinf(rotateX), cx = ::cosf(rotateX);
            float sy = ::sinf(rotateY), cy = ::cosf(rotateY);
            DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&camera_focus);
            DirectX::XMVECTOR Front = DirectX::XMVectorSet(-cx * sy, -sx, -cx * cy, 0.0f);
            DirectX::XMVECTOR Distance = DirectX::XMVectorSet(distance, distance, distance, 0.0f);
            Front = DirectX::XMVectorMultiply(Front, Distance);
            DirectX::XMVECTOR Eye = DirectX::XMVectorSubtract(Focus, Front);
            DirectX::XMStoreFloat3(&camera_position, Eye);

            DirectX::XMFLOAT3 front;
            DirectX::XMStoreFloat3(&front, Front);  //�O�����x�N�g������ꂽ

            //�����_������x�N�g�������ɍs���ċ������ꂽ�J�������_�����߂�
            DirectX::XMFLOAT3 eye;
            eye.x = target.x - front.x * 6.0f;
            eye.y = target.y - front.y * 6.0f;
            eye.z = target.z - front.z * 6.0f;
            // �J�����Ɏ��_�𒍎��_��ݒ�
            Camera::Instance().SetLookAt(camera_position, camera_focus, { 0, 1, 0 });
            Camera::Instance().SetLookAt(eye, target, { 0, 1, 0 });
        }
        if (mouse.GetWheel() != 0)
        {
            // �Y�[��
            distance -= static_cast<float>(mouse.GetWheel()) * distance * 0.001f;
        }


    }
}

void CameraController::UpdateDemo(float elapsedTime)
{

    //�J������]�l����]�s��ɕϊ�
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y + 3, angle.z); //��XMMatrixRotationRollPitchYaw�֐��ŃI�C���[�p�x�Ɋ�Â��ĉ�]�s����쐬

    EnemyManager& enemyManager = EnemyManager::Instance();
    int enemyCount = enemyManager.GetEnemyCount();

    target.y += 1.0f;

    //��]�s�񂩂�O�����x�N�g�������o��
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);  //�O�����x�N�g������ꂽ

    DirectX::XMFLOAT3 Target = target;



    //�����_������x�N�g�������ɍs���ċ������ꂽ�J�������_�����߂�
    DirectX::XMFLOAT3 eye;
    eye.x = Target.x - front.x * range;
    eye.y = Target.y - front.y * range;
    eye.z = Target.z - front.z * range;

    // �J�����Ɏ��_�𒍎��_��ݒ�
    //Camera::Instance().SetLookAt(camera_position, camera_focus, { 0, 1, 0 });
    Camera::Instance().SetLookAt(eye, target, { 0, 1, 0 });
}

void CameraController::DrawGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("cameracontrol", nullptr, ImGuiWindowFlags_None))
    {
        //�ʒu
        ImGui::SliderFloat("cameraX", &camerapower.x, 0, 1);
        ImGui::SliderFloat("cameraY", &camerapower.y, -1, 1);
        ImGui::SliderFloat("angle.x", &angle.x, -10, 10);
        ImGui::SliderFloat("angle.y", &angle.y, -10, 10);
        ImGui::SliderFloat("range", &range, -10, 20);

    }
    ImGui::End();

}

void CameraController::CameraShake(float maxX, float minX, float maxY,
    float minY, float increase, float randMaxX , float randMinX, float randMaxY , float randMinY )
{
    // �ő�l�܂ł�������t�]
    if (this->angle.x > randMaxX)
    {
        shakeX = false;
        randMinY = Mathf::RandomRange(0, minY);
        angle.y = randMaxY;
        //this->angle.x += increase;

    }
    // �Œ�l�܂ł�������t�]
    else if (this->angle.x < -randMinX)
    {
        shakeX = true;

        randMaxY = Mathf::RandomRange(0, maxY);

        angle.y = -randMinY;

        //this->angle.x -= increase;

    }

    // �ő�l�܂ł�������t�]
    if (this->angle.y > randMaxY)
    {
        shakeY = false;
        randMinX = Mathf::RandomRange(0, minX);

        //this->angle.y -= increase;
        angle.x = randMaxX;

    }
    // �Œ�l�܂ł�������t�]
    else if (this->angle.y < -randMinY)
    {
        shakeY = true;
        randMaxX = Mathf::RandomRange(0, maxX);
        angle.x = -randMinX;

        //this->angle.y -= increase;


    }
    // �h���
    if (shakeX)
    {
        this->angle.x += increase;
    }
    if (!shakeX)
    {
        this->angle.x -= increase;
    }
    // �h���
    if (shakeY)
    {
        this->angle.y += increase;
    }
    if (!shakeY)
    {
        this->angle.y -= increase;
    }


}

void CameraController::camerafall(float elapsedTime)
{
    //�J�����̉�]���x
    float speed = rollSpeed * elapsedTime;

    float ax = 0.01f;
    float ay = 0.01f;

    range += 0.1f;

    if (range > 20)
        range = 20;

}

void CameraController::CameraDown(float elapsedTime)
{
    range -= 1.0f;

    if (range < 8.0f)
        range = 8.0f;
}

void CameraController::CameraShackmouse(float maxX, float minX, float maxY, float minY, float increase, float randMaxX, float randMinX, float randMaxY, float randMinY)
{
    // �ő�l�܂ł�������t�]
    if (this->moveX > randMaxX)
    {
        shakeMouseX = false;
        randMinY = Mathf::RandomRange(0, minY);
        moveY = randMaxY;
        //this->angle.x += increase;

    }
    // �Œ�l�܂ł�������t�]
    else if (this->moveX < -randMinX)
    {
        shakeMouseX = true;

        randMaxY = Mathf::RandomRange(0, maxY);

        moveY = -randMinY;

        //this->angle.x -= increase;

    }

    // �ő�l�܂ł�������t�]
    if (this->moveY > randMaxY)
    {
        shakeMouseY = false;
        randMinX = Mathf::RandomRange(0, minX);

        //this->angle.y -= increase;
        moveX = randMaxX;

    }
    // �Œ�l�܂ł�������t�]
    else if (this->moveY < -randMinY)
    {
        shakeMouseY = true;
        randMaxX = Mathf::RandomRange(0, maxX);
        moveX = -randMinX;

        //this->angle.y -= increase;


    }
    // �h���
    if (shakeMouseX)
    {
        this->moveX += increase;
    }
    if (!shakeMouseX)
    {
        this->moveX -= increase;
    }
    // �h���
    if (shakeMouseY)
    {
        this->moveY += increase;
    }
    if (!shakeMouseY)
    {
        this->moveY -= increase;
    }
}
