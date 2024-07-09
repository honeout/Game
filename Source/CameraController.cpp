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

// 更新処理
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
    //    // ここにAキーに対する処理を追加
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
        //マウスカーソルを非表示にする
        ::ShowCursor(FALSE);
        ::SetCursorPos(960, 540);
        //マウス操作
        Mouse& mouse = Input::Instance().GetMouse();
        //float moveX = (mouse.GetPositionX() - mouse.GetOldPositionX()) * 0.02f;
        //float moveY = (mouse.GetPositionY() - mouse.GetOldPositionY()) * 0.02f;

        float moveX = static_cast<float>(mouse.GetPositionX() - 960) * 0.02f;
        float moveY = static_cast<float>(mouse.GetPositionY() - 540) * 0.02f;
        Camera& camera = Camera::Instance();
        // 視線行列を生成
        DirectX::XMMATRIX V;
        {
            DirectX::XMVECTOR up{ DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) };
            // マウス操作
            {
                //if (::GetAsyncKeyState(VK_RBUTTON) & 0x8000)
                //{
                    // Y軸回転
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
                    // 平行移動
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
                //if (mouse.GetWheel() != 0)	// ズーム
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
            DirectX::XMStoreFloat3(&front, Front);  //前方向ベクトルを入れた

            DirectX::XMFLOAT3 Target = target;

            //target.x -= 1.0f;
            target.y += 1.0f;
            //target.z -= 1.0f;

            //注視点から後ろベクトル方向に行って距離離れたカメラ視点を求める
            DirectX::XMFLOAT3 eye;
            eye.x = Target.x - front.x * range;
            eye.y = Target.y - front.y * range;
            eye.z = Target.z - front.z * range;


            // カメラに視点を注視点を設定
            Camera::Instance().SetLookAt(camera_position, camera_focus, { 0, 1, 0 });
            Camera::Instance().SetLookAt(eye, target, { 0, 1, 0 });
        }
        //if (mouse.GetWheel() != 0)
        //{
        //    // ズーム
           // distance -= static_cast<float>(mouse.GetWheel()) * distance * 0.001f;
        //}
    }
    else if (camera_operate == 1)
    {
        //コントローラー、キー操作
        GamePad& gamePad = Input::Instance().GetGamePad();
        float ax = gamePad.GetAxisRX();
        float ay = gamePad.GetAxisRY();

        //カメラの回転速度
        float speed = rollSpeed * elapsedTime;

        //スティックの入力値に合わせてX軸とY軸を回転

        angle.x += ay * speed;
        angle.y -= ax * speed;
        //if文なんてしなくても実はこれでいい
        //キーボードのI、J、K、Lでカメラ回転ができるはず！

        //X軸のカメラ回転を制限
        if (angle.x < minAngleX)
        {
            angle.x = minAngleX;
        }
        //設定した値より小さくなりそうだったらそれより小さくならないように設定した値になるようにする
        if (angle.x > maxAngleX)
        {
            angle.x = maxAngleX;
        }
        //上の逆をする

        //Y軸の回転値を-3.14~3,14に収まるようにする
        if (angle.y < -DirectX::XM_PI)
        {
            angle.y += DirectX::XM_2PI;
        }
        if (angle.y > DirectX::XM_PI)
        {
            angle.y -= DirectX::XM_2PI;
        }

        //カメラ回転値を回転行列に変換
        DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z); //←XMMatrixRotationRollPitchYaw関数でオイラー角度に基づいて回転行列を作成

        //回転行列から前方向ベクトルを取り出す
        DirectX::XMVECTOR Front = Transform.r[2];
        DirectX::XMFLOAT3 front;
        DirectX::XMStoreFloat3(&front, Front);  //前方向ベクトルを入れた

        DirectX::XMFLOAT3 Target = target;

        //target.x -= 1.0f;
        target.y += 1.0f;
        //target.z -= 1.0f;

        //注視点から後ろベクトル方向に行って距離離れたカメラ視点を求める
        DirectX::XMFLOAT3 eye;
        eye.x = Target.x - front.x * range; //rangeを変えるとplayerとのカメラの距離を変えられるので利用する
        eye.y = Target.y - front.y * range;
        eye.z = Target.z - front.z * range;


        //カメラの視点と注視点を設定
        Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
    }
    else if (camera_operate == 2)
    {
        //マウス操作
        Mouse& mouse = Input::Instance().GetMouse();
        moveX = (mouse.GetPositionX() - mouse.GetOldPositionX()) * 0.02f;
        moveY = (mouse.GetPositionY() - mouse.GetOldPositionY()) * 0.02f;
        Camera& camera = Camera::Instance();
        // 視線行列を生成
        DirectX::XMMATRIX V;
        {
            DirectX::XMVECTOR up{ DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) };
            // マウス操作
            {
                //if (::GetAsyncKeyState(VK_RBUTTON) & 0x8000)
                //{
                    // Y軸回転
                rotateY += moveX * 0.3f;
                if (rotateY > DirectX::XM_PI)
                    rotateY -= DirectX::XM_2PI;
                else if (rotateY < -DirectX::XM_PI)
                    rotateY += DirectX::XM_2PI;
                // X軸回転
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
                    // 平行移動
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
                if (mouse.GetWheel() != 0)	// ズーム
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
            DirectX::XMStoreFloat3(&front, Front);  //前方向ベクトルを入れた

            //注視点から後ろベクトル方向に行って距離離れたカメラ視点を求める
            DirectX::XMFLOAT3 eye;
            eye.x = target.x - front.x * 6.0f;
            eye.y = target.y - front.y * 6.0f;
            eye.z = target.z - front.z * 6.0f;
            // カメラに視点を注視点を設定
            Camera::Instance().SetLookAt(camera_position, camera_focus, { 0, 1, 0 });
            Camera::Instance().SetLookAt(eye, target, { 0, 1, 0 });
        }
        if (mouse.GetWheel() != 0)
        {
            // ズーム
            distance -= static_cast<float>(mouse.GetWheel()) * distance * 0.001f;
        }


    }
}

void CameraController::UpdateDemo(float elapsedTime)
{

    //カメラ回転値を回転行列に変換
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y + 3, angle.z); //←XMMatrixRotationRollPitchYaw関数でオイラー角度に基づいて回転行列を作成

    EnemyManager& enemyManager = EnemyManager::Instance();
    int enemyCount = enemyManager.GetEnemyCount();

    target.y += 1.0f;

    //回転行列から前方向ベクトルを取り出す
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);  //前方向ベクトルを入れた

    DirectX::XMFLOAT3 Target = target;



    //注視点から後ろベクトル方向に行って距離離れたカメラ視点を求める
    DirectX::XMFLOAT3 eye;
    eye.x = Target.x - front.x * range;
    eye.y = Target.y - front.y * range;
    eye.z = Target.z - front.z * range;

    // カメラに視点を注視点を設定
    //Camera::Instance().SetLookAt(camera_position, camera_focus, { 0, 1, 0 });
    Camera::Instance().SetLookAt(eye, target, { 0, 1, 0 });
}

void CameraController::DrawGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("cameracontrol", nullptr, ImGuiWindowFlags_None))
    {
        //位置
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
    // 最大値までいったら逆転
    if (this->angle.x > randMaxX)
    {
        shakeX = false;
        randMinY = Mathf::RandomRange(0, minY);
        angle.y = randMaxY;
        //this->angle.x += increase;

    }
    // 最低値までいったら逆転
    else if (this->angle.x < -randMinX)
    {
        shakeX = true;

        randMaxY = Mathf::RandomRange(0, maxY);

        angle.y = -randMinY;

        //this->angle.x -= increase;

    }

    // 最大値までいったら逆転
    if (this->angle.y > randMaxY)
    {
        shakeY = false;
        randMinX = Mathf::RandomRange(0, minX);

        //this->angle.y -= increase;
        angle.x = randMaxX;

    }
    // 最低値までいったら逆転
    else if (this->angle.y < -randMinY)
    {
        shakeY = true;
        randMaxX = Mathf::RandomRange(0, maxX);
        angle.x = -randMinX;

        //this->angle.y -= increase;


    }
    // 揺れる
    if (shakeX)
    {
        this->angle.x += increase;
    }
    if (!shakeX)
    {
        this->angle.x -= increase;
    }
    // 揺れる
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
    //カメラの回転速度
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
    // 最大値までいったら逆転
    if (this->moveX > randMaxX)
    {
        shakeMouseX = false;
        randMinY = Mathf::RandomRange(0, minY);
        moveY = randMaxY;
        //this->angle.x += increase;

    }
    // 最低値までいったら逆転
    else if (this->moveX < -randMinX)
    {
        shakeMouseX = true;

        randMaxY = Mathf::RandomRange(0, maxY);

        moveY = -randMinY;

        //this->angle.x -= increase;

    }

    // 最大値までいったら逆転
    if (this->moveY > randMaxY)
    {
        shakeMouseY = false;
        randMinX = Mathf::RandomRange(0, minX);

        //this->angle.y -= increase;
        moveX = randMaxX;

    }
    // 最低値までいったら逆転
    else if (this->moveY < -randMinY)
    {
        shakeMouseY = true;
        randMaxX = Mathf::RandomRange(0, maxX);
        moveX = -randMinX;

        //this->angle.y -= increase;


    }
    // 揺れる
    if (shakeMouseX)
    {
        this->moveX += increase;
    }
    if (!shakeMouseX)
    {
        this->moveX -= increase;
    }
    // 揺れる
    if (shakeMouseY)
    {
        this->moveY += increase;
    }
    if (!shakeMouseY)
    {
        this->moveY -= increase;
    }
}
