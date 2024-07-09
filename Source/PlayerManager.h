#pragma once

#include <vector>
#include "Player.h"

//�v���C���[�}�l�[�W���[
class PlayerManager
{
private:
    PlayerManager() {}
    ~PlayerManager() {}

public:
    //�B��̃C���X�^���X�擾
    static PlayerManager& Instance()
    {
        static PlayerManager instance;
        return instance;
    }

    //�X�V����
    void Update(float elapsedTime);

    //�`�揈��
    void Render(RenderContext& rc, ModelShader* shader);

    //�v���C���[�o�^
    void Register(Player* player);

    //�v���C���[�폜
    void Remove(Player* player);

    //�v���C���[�S�폜
    void Clear();

    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

    //�f�o�b�O���`��
    void DrawDebugGUI();

    //�v���C���[���擾
    int GetPlayerCount() const { return static_cast<int>(players.size()); }

    //�v���C���[�擾
    Player* GetPlayer(int index) { return players.at(index); }

    //���݂̕���ԍ��̐ݒ�
    void SetWeapon(int index) { weapon_index = index; }

    //���݂̕���ԍ��̎擾
    int GetWeapon() const { return weapon_index; }

    //�O�̕���ԍ��̐ݒ�
    void SetBeforeWeapon(int index) { weapon_before_index = index; }

    //�O�̕���ԍ��̐ݒ�
    int GetBeforeWeapon() const { return weapon_before_index; }

    //����ύX�t���O����
    void SetWeaponFlag(bool flag) { weaponchangeflag = flag; }
    bool GetWeaponFlag() const { return weaponchangeflag; }
    
    void SetStartAnimation(bool startanimation) { this->startanimation = startanimation; }
    bool GetStartAnimation() { return this->startanimation; }

    bool GetGuardMode() {return inputguard;}
    void SetGuardMode(bool intpuguard) {this->inputguard = intpuguard;}
public:
    int weapon_index = 0;
    // �O�̕���(���f���ԍ�)
    int weapon_before_index = 0;
    bool weaponchangeflag = false;

    bool startanimation = false;

private:
    //�����̃v���C���[�̊Ǘ��̂��߂Ƀv���C���[�|�C���^��std::vector�ŊǗ�
    std::vector<Player*> players;
    std::vector<Player*> removes;

    bool inputguard = false;


};