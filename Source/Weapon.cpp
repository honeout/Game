#include "Weapon.h"
#include "PlayerManager.h"

Weapon::Weapon()
{
    //モデルのロード
    model = new Model("Data/Model/Player/Weapon/Dual/sword_twinsword.mdl");

    playerPositionDual = { 0,0,0 };
    playerPositionGreat = { 0,0,0 };
    playerPositionGun = { 0,0,0 };

    scale.x = scale.y = scale.z = 0.02f;

    state = State::Gun;

    change = std::make_unique<Effect>("Data/Effect/changeplayermode.efk");

    //TransitionGun();
}

Weapon::~Weapon()
{
    delete model;
}
//
//void Weapon::initialize()
//{
//    //モデルのロード
//    model = new Model("Data/Model/Player/Weapon/weapon.mdl");
//
//    playerPositionDual = { 0,0,0 };
//    playerPositionGreat = { 0,0,0 };
//    playerPositionGun = { 0,0,0 };
//
//    scale.x = scale.y = scale.z = 0.01f;
//}
//
//void Weapon::Clear()
//{
//    delete model;
//}

void Weapon::UpdateTransform()
{
    
    //スケール行列を作成
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

    //回転行列を作成
    DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
    DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
    DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
    DirectX::XMMATRIX R = Y * X * Z;

    //位置行列を作成
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

    //３つの行列を組み合わせ、ワールド行列を作成
    DirectX::XMMATRIX W = S * R * T;

    //計算したワールド行列を取り出す
    DirectX::XMStoreFloat4x4(&transform, W);
}

void Weapon::Update(float elapsedTime)
{
    float speed = 10;
    //angle.y += speed * elapsedTime;
    //if (angle.y >= 1800)
    //{
    //    angle.y = 0;
    //}

    switch (state)
    {
    case State::Dual:
        UpdateDual(elapsedTime);
        break;
    case State::Gread:
        UpdateGread(elapsedTime);
        break;
    case State::Gun:
        UpdateGun(elapsedTime);
        break;
    }

    Model::Node* node = model->FindNode("J_Movable_part_grip_R_01");

    //プレイヤーの位置を常に取得し続ける
    PlayerManager& playerMgr = PlayerManager::Instance();

    //playerPositionDual = playerMgr.GetPlayer(0)->GetPosition();
    //playerPositionGreat = playerMgr.GetPlayer(1)->GetPosition();
    //playerPositionGun = playerMgr.GetPlayer(2)->GetPosition();

    //playerPositionDual = playerMgr.GetPlayer(3)->GetPosition();
    //playerPositionDual = playerMgr.GetPlayer(3)->GetHandPositionR();
    //playerPositionGreat = playerMgr.GetPlayer(3)->GetHandPositionR();
    //playerPositionGun = playerMgr.GetPlayer(3)->GetHandPositionR();

    //if (playerMgr.GetBeforeWeapon() == 0&&playerMgr.GetStartAnimation())
    //{
    //   
    //    TransitionDual();
    //    
    //}
    //if (playerMgr.GetWeapon() == 1 && playerMgr.GetWeaponFlag())
    //{
    //    state = State::Gread;
    //}
    //if (playerMgr.GetWeapon() == 2 && playerMgr.GetWeaponFlag())
    //{
    //    //position = playerPositionGun;
    //    state = State::Gun;
    //}

    //if (!model->IsPlayAnimation())
    //{
    //    position = { 0,1000000000000.0f,0 };
    //}

    //if (playerMgr.GetWeapon() == 0)
    //{
    //    position = playerPositionDual;
    //}
    //if (playerMgr.GetWeapon() == 1)
    //{
    //    position = playerPositionGreat;
    //}
    //if (playerMgr.GetWeapon() == 2)
    //{
    //    position = playerPositionGun;
    //}

    //position.y = 1000000000.0f;

    //angle.x = DirectX::XMConvertToRadians(90.0f);

    //if (playerMgr.GetWeaponFlag() == true)
    //{
    //    position.y = 0.01f;
    //    model->PlayAnimation(Anim_Dual_to_Great, false);

    //}


    //行列更新
    UpdateTransform();
    if (PlayerManager::Instance().GetBeforeWeapon() != 2)
    {
        // モデルの更新処理
        model->UpdateAnimation(elapsedTime);
    }
    else
    {
        model->ReverseplaybackAnimation(elapsedTime);
    }
    //モデル行列更新
    model->UpdateTransform(transform);
}

void Weapon::Render(RenderContext& rc, ModelShader* shader)
{
    rc.toonData.edgeWidth = 0.02f;

    shader->Draw(rc, model);
}

void Weapon::TransitionDual()
{
    state = State::Dual;
    PlayerManager::Instance().SetStartAnimation(false);
    model->PlayAnimation(Anim_Dual_to_Gread, false);
    //model->PlayAnimation(Anim_Gun_to_Dual, false);

    playerPositionDual = PlayerManager::Instance().
        GetPlayer(3)->GetPosition();

    playerPositionDual.y += 2.5f;

    //angle.y = PlayerManager::Instance().
    //    GetPlayer(3)->GetAngle().y;

    //angle = PlayerManager::Instance().GetPlayer(3)->GetAngle();

    angle.x = DirectX::XMConvertToRadians(90);

    //angle.x = sinf(PlayerManager::Instance().GetPlayer(3)->GetAngle().y);

    //angle.y = 0;

    //angle.z = cosf(PlayerManager::Instance().GetPlayer(3)->GetAngle().y);

    //angle.y = PlayerManager::Instance().GetPlayer(3)->GetAngle().y;
    //angle.z = PlayerManager::Instance().GetPlayer(3)->GetAngle().z;
    change->Play(playerPositionDual, sizeefk);
}

void Weapon::UpdateDual(float elapsedTime)
{
    //プレイヤーの位置を常に取得し続ける
    PlayerManager& playerMgr = PlayerManager::Instance();

    

    //playerPositionDual = playerMgr.GetPlayer(3)->GetHandPositionR();



    position = playerPositionDual;
    

    if (playerMgr.GetBeforeWeapon() == 1)
    {
        //state = State::Gread;
        TransitionGread();
    }
    if (playerMgr.GetBeforeWeapon() == 2)
    {
        //position = playerPositionGun;
        //state = State::Gun;
        TransitionGun();
    }

    
    //if (!model->IsPlayAnimation())
    if (playerMgr.GetPlayer(playerMgr.GetWeapon())->GetPosition().y > 0)
    {
        position = { 0,1000000000000.0f,0 };
    }
}

void Weapon::TransitionGread()
{
    state = State::Gread;

    model->PlayAnimation(Anim_Gread_to_Gun, false);

    PlayerManager::Instance().SetStartAnimation(false);

    playerPositionGreat = PlayerManager::Instance().
        GetPlayer(3)->GetPosition();

    playerPositionGreat.y += 2.5f;

    //angle.y = PlayerManager::Instance().
    //    GetPlayer(3)->GetAngle().y;

    angle.x = DirectX::XMConvertToRadians(90);

    //angle.x = sinf(PlayerManager::Instance().GetPlayer(3)->GetAngle().y);
    //angle.y = 0;
    //angle.z = cosf(PlayerManager::Instance().GetPlayer(3)->GetAngle().y);
    //angle.y = PlayerManager::Instance().GetPlayer(3)->GetAngle().y;
    //angle.z = PlayerManager::Instance().GetPlayer(3)->GetAngle().z;
    change->Play(playerPositionGreat, sizeefk);
}

void Weapon::UpdateGread(float elapsedTime)
{
    //プレイヤーの位置を常に取得し続ける
    PlayerManager& playerMgr = PlayerManager::Instance();

    //playerPositionGreat = playerMgr.GetPlayer(3)->GetHandPositionR();

    position = playerPositionGreat;
    

    if (playerMgr.GetBeforeWeapon() == 0)
    {
        TransitionDual();

    }
    if (playerMgr.GetBeforeWeapon() == 2)
    {
        TransitionGun();
    }

    if (!model->IsPlayAnimation())
    {
        position = { 0,1000000000000.0f,0 };
    }
}

void Weapon::TransitionGun()
{
    state = State::Gun;

    model->PlayAnimation(Anim_Gun_to_Dual, false);

    PlayerManager::Instance().SetStartAnimation(false);

    playerPositionGun = PlayerManager::Instance().
        GetPlayer(3)->GetPosition();

    playerPositionGun.y += 2.5f;

    //angle.y = PlayerManager::Instance().
    //    GetPlayer(3)->GetAngle().y;

    //angle.y = PlayerManager::Instance().GetPlayer(3)->GetAngle().y;


    angle.x = DirectX::XMConvertToRadians(90);
    //angle.x = sinf(PlayerManager::Instance().GetPlayer(3)->GetAngle().y);
    //angle.y = 0;
    //angle.z = cosf(PlayerManager::Instance().GetPlayer(3)->GetAngle().y);
    //angle.y = PlayerManager::Instance().GetPlayer(3)->GetAngle().y;
    //angle.z = PlayerManager::Instance().GetPlayer(3)->GetAngle().z;

    change->Play(playerPositionGun, sizeefk);
}

void Weapon::UpdateGun(float elapsedTime)
{
    //プレイヤーの位置を常に取得し続ける
    PlayerManager& playerMgr = PlayerManager::Instance();

    //playerPositionGun = playerMgr.GetPlayer(3)->GetHandPositionR();

    position = playerPositionGun;
    

    if (playerMgr.GetBeforeWeapon() == 0)
    {
        TransitionDual();
    }
    if (playerMgr.GetBeforeWeapon() == 1)
    {
        TransitionGread();
    }

    if (!model->IsPlayAnimation())
    {
        position = { 0,1000000000000.0f,0 };
    }
}
