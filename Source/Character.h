#pragma once

#include <DirectXMath.h>

//�L�����N�^�[
class Character
{
public:
    Character() {}
    virtual ~Character() {}
    //���p������ړI�̃N���X�̓f�X�g���N�^��virtual������
    //�łȂ���Όp����̃f�X�g���N�^���Ă΂�Ȃ��Ȃ��Ă��܂�

    //�s��X�V����
    void UpdateTransform();
    //�ʒu�擾
    const DirectX::XMFLOAT3& GetPosition() const { return position; }
    //�ʒu�ݒ�
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }
    //��]�擾
    const DirectX::XMFLOAT3& GetAngle() const { return angle; }
    //��]�ݒ�
    void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }
    //�X�P�[���擾
    const DirectX::XMFLOAT3& GetScale() const { return scale; }
    //�X�P�[���ݒ�
    void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }
    //���a�擾
    float GetRadius() const { return radius; }
    //�n�ʂɐڒn���Ă��邩
    bool IsGround() const { return isGround; }
    //�����擾
    float GetHeight() const { return height; }
    //�_���[�W��^����
    bool ApplyDamage(int damage, float invincibleTime);
    //�Ռ���^����
    void AddImpulse(const DirectX::XMFLOAT3& impulse);
    //�̗͂��擾
    int GetHealth() const { return health; }
    //�ő�̗͂��擾
    int GetMaxHealth() const { return maxHealth; }
    //changetime get/set
    float GetChangeTimer() const { return changetimer; }
    void SetChangeTimer(float timer) { changetimer = timer; }

    bool GetShackCheck() { return shackChack; }
    bool GetShackStrongCheck() { return shackStrongChack; }

    // �f���p�J�����̐؂�ւ�
    bool GetCameraChange() { return camerachange; }

    // �f���p�J�����Ō�̃A�b�v�p
    bool GetRastApCamera() { return rastapcamera; }
    bool GetFadeOutCamera() { return   fadeoutcamera; }

    int GetAnimationChange() { return static_cast<int>(animationChange); }

    int GetAttackCount() { return attackcount; }


    

    bool GetChangeTimeMode() { return changetimemode; }

    void SetChangeTimeMode(bool changeTimerMode) { changetimemode = changeTimerMode; }

    //�K�[�h�t���O�̎擾�ݒ�
    void SetGuardFlag(bool flag) { guardflag = flag; }
    bool GetGuardFlag() { return guardflag; }

    bool GetStartBoss() {return startboss;}
    void SetStartBoss(bool start) { this->startboss = start; }

    void SetHP(int health) { this->health = health; }
    int GetHP() { health; }


protected:
    //unit06
    //�ړ�����
    void Move(float vx, float vz, float speed);
    //���񏈗�
    void Turn(float elapsedTime, float vx, float vz, float speed);
    //�W�����v����
    void Jump(float speed);
    //���͏����X�V
    void UpdateVelocity(float elapsedTime);
    //���n�����Ƃ��ɌĂ΂��
    virtual void OnLanding() {}
    //�_���[�W���󂯂��Ƃ��ɌĂ΂��
    virtual void OnDamaged() {}
    //���S�����Ƃ��ɌĂ΂��
    virtual void OnDead() {}
    //���G���ԍX�V
    void UpdateInvincibleTimer(float elapsedTime);

private:
    //�������͍X�V����
    void UpdateVerticalVelocity(float elpasedFrame);

    //�����ړ��X�V����
    void UpdateVerticalMove(float elapsedTime);

    //�������͍X�V����
    void UpdateHorizontalVelocity(float elapsedFrame);

    //�����ړ��X�V����
    void UpdateHorizontalMove(float elpasedTime);

protected:

    // �㉺�ʊm�F�p
    enum AnimationChange
    {
        Normal,
        doble,
    };

protected:
    DirectX::XMFLOAT3       position = { -3,0,0};       //�ʒu
    DirectX::XMFLOAT3       angle = { 0,0,0 };          //��]
    DirectX::XMFLOAT3       scale = { 1,1,1 };          //�X�P�[��
    DirectX::XMFLOAT4X4     transform = {               //��̃p�����[�^�����ƂɌv�Z����s��
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
    float radius = 0.5f;                                //���a
    float gravity = -1.0f;                              //�d��
    DirectX::XMFLOAT3 velocity = { 0,0,0 };             //����
    bool isGround = false;
    float height = 2.0f;                                //����
    int health = 5;                                     //�̗�
    int maxHealth = 5;                                  //�ő�̗�
    float invincibleTimer = 1.0f;                       //���G����
    float friction = 0.5f;                              //���C
    float acceleration = 1.0f;                          //����
    float maxMoveSpeed = 5.0f;                          //�ō����x
    float moveVecX = 0.0f;                              //X�ړ��l
    float moveVecZ = 0.0f;                              //Z�ړ��l
    float airControl = 0.3f;                            //�󒆑���
    float stepOffset = 1.0f;                            //���C�L���X�g�����p
    float slopeRate = 1.0f;                             //�X�Η�

    int chargetime = 0;                                 //�׍U���̎��Ԑݒ�
    //int chargetimeMax = 1;                            //�׍U���̎��Ԑݒ�ő�
    int chargetimeMax = 260;

    bool shotstate = false;                             //���X�e�[�g�֑J�ڋ���

    float changetimer = 0.0f;

    int shotTimer = 0;

    bool shackChack;

    bool shackStrongChack;

    // �f���p�J�����̐؂�ւ�
    bool camerachange = false;

    // �f���p�J��������
    bool rastapcamera = false;

    // �f������J�����؂�ւ��p
    bool fadeoutcamera = false;

    // �A�j���[�V�����̕ύX 
    bool animationConfirmation = false;
    
    // �㉺�ʃA�j���[�V�����̍Đ��pstate
    AnimationChange animationChange;

    // �e�̕ێ��ő�
    int attackcount = 0;

    //�K�[�h�t���O
    bool guardflag = false;

    // ��̈ʒuleft
    DirectX::XMFLOAT3 handPositionL = { 0,0,0 };

    // ��̈ʒuright
    DirectX::XMFLOAT3 handPositionR = { 0,0,0 };

    bool changetimemode = false;

    float changetimermax = 120.0f;


    bool startboss = false;
};