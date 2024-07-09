#pragma once

#include <DirectXMath.h>

//キャラクター
class Character
{
public:
    Character() {}
    virtual ~Character() {}
    //↑継承する目的のクラスはデストラクタにvirtualをつける
    //でなければ継承先のデストラクタが呼ばれなくなってしまう

    //行列更新処理
    void UpdateTransform();
    //位置取得
    const DirectX::XMFLOAT3& GetPosition() const { return position; }
    //位置設定
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }
    //回転取得
    const DirectX::XMFLOAT3& GetAngle() const { return angle; }
    //回転設定
    void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }
    //スケール取得
    const DirectX::XMFLOAT3& GetScale() const { return scale; }
    //スケール設定
    void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }
    //半径取得
    float GetRadius() const { return radius; }
    //地面に接地しているか
    bool IsGround() const { return isGround; }
    //高さ取得
    float GetHeight() const { return height; }
    //ダメージを与える
    bool ApplyDamage(int damage, float invincibleTime);
    //衝撃を与える
    void AddImpulse(const DirectX::XMFLOAT3& impulse);
    //体力を取得
    int GetHealth() const { return health; }
    //最大体力を取得
    int GetMaxHealth() const { return maxHealth; }
    //changetime get/set
    float GetChangeTimer() const { return changetimer; }
    void SetChangeTimer(float timer) { changetimer = timer; }

    bool GetShackCheck() { return shackChack; }
    bool GetShackStrongCheck() { return shackStrongChack; }

    // 映像用カメラの切り替え
    bool GetCameraChange() { return camerachange; }

    // 映像用カメラ最後のアップ用
    bool GetRastApCamera() { return rastapcamera; }
    bool GetFadeOutCamera() { return   fadeoutcamera; }

    int GetAnimationChange() { return static_cast<int>(animationChange); }

    int GetAttackCount() { return attackcount; }


    

    bool GetChangeTimeMode() { return changetimemode; }

    void SetChangeTimeMode(bool changeTimerMode) { changetimemode = changeTimerMode; }

    //ガードフラグの取得設定
    void SetGuardFlag(bool flag) { guardflag = flag; }
    bool GetGuardFlag() { return guardflag; }

    bool GetStartBoss() {return startboss;}
    void SetStartBoss(bool start) { this->startboss = start; }

    void SetHP(int health) { this->health = health; }
    int GetHP() { health; }


protected:
    //unit06
    //移動処理
    void Move(float vx, float vz, float speed);
    //旋回処理
    void Turn(float elapsedTime, float vx, float vz, float speed);
    //ジャンプ処理
    void Jump(float speed);
    //速力処理更新
    void UpdateVelocity(float elapsedTime);
    //着地したときに呼ばれる
    virtual void OnLanding() {}
    //ダメージを受けたときに呼ばれる
    virtual void OnDamaged() {}
    //死亡したときに呼ばれる
    virtual void OnDead() {}
    //無敵時間更新
    void UpdateInvincibleTimer(float elapsedTime);

private:
    //垂直速力更新処理
    void UpdateVerticalVelocity(float elpasedFrame);

    //垂直移動更新処理
    void UpdateVerticalMove(float elapsedTime);

    //水平速力更新処理
    void UpdateHorizontalVelocity(float elapsedFrame);

    //水平移動更新処理
    void UpdateHorizontalMove(float elpasedTime);

protected:

    // 上下別確認用
    enum AnimationChange
    {
        Normal,
        doble,
    };

protected:
    DirectX::XMFLOAT3       position = { -3,0,0};       //位置
    DirectX::XMFLOAT3       angle = { 0,0,0 };          //回転
    DirectX::XMFLOAT3       scale = { 1,1,1 };          //スケール
    DirectX::XMFLOAT4X4     transform = {               //上のパラメータをもとに計算する行列
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
    float radius = 0.5f;                                //半径
    float gravity = -1.0f;                              //重力
    DirectX::XMFLOAT3 velocity = { 0,0,0 };             //速力
    bool isGround = false;
    float height = 2.0f;                                //高さ
    int health = 5;                                     //体力
    int maxHealth = 5;                                  //最大体力
    float invincibleTimer = 1.0f;                       //無敵時間
    float friction = 0.5f;                              //摩擦
    float acceleration = 1.0f;                          //加速
    float maxMoveSpeed = 5.0f;                          //最高速度
    float moveVecX = 0.0f;                              //X移動値
    float moveVecZ = 0.0f;                              //Z移動値
    float airControl = 0.3f;                            //空中操作
    float stepOffset = 1.0f;                            //レイキャスト処理用
    float slopeRate = 1.0f;                             //傾斜率

    int chargetime = 0;                                 //為攻撃の時間設定
    //int chargetimeMax = 1;                            //為攻撃の時間設定最大
    int chargetimeMax = 260;

    bool shotstate = false;                             //次ステートへ遷移許可

    float changetimer = 0.0f;

    int shotTimer = 0;

    bool shackChack;

    bool shackStrongChack;

    // 映像用カメラの切り替え
    bool camerachange = false;

    // 映像用カメラ制御
    bool rastapcamera = false;

    // 映像からカメラ切り替え用
    bool fadeoutcamera = false;

    // アニメーションの変更 
    bool animationConfirmation = false;
    
    // 上下別アニメーションの再生用state
    AnimationChange animationChange;

    // 銃の保持最大
    int attackcount = 0;

    //ガードフラグ
    bool guardflag = false;

    // 手の位置left
    DirectX::XMFLOAT3 handPositionL = { 0,0,0 };

    // 手の位置right
    DirectX::XMFLOAT3 handPositionR = { 0,0,0 };

    bool changetimemode = false;

    float changetimermax = 120.0f;


    bool startboss = false;
};