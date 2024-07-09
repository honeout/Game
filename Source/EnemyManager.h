#pragma once

#include<vector>
#include "Enemy.h"

//�G�l�~�[�}�l�[�W���[
class EnemyManager
{
private:
    EnemyManager() {}
    ~EnemyManager() {}

public:
    //�B��̃C���X�^���X�擾(�V���O���g����)
    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }

    //�X�V����
    void Update(float elapsedTime);

    //�`�揈��
    void Render(RenderContext& rc, ModelShader* shader);

    //�G�l�~�[�o�^
    void Register(Enemy* enemy);

    //�G�l�~�[�폜
    void Remove(Enemy* enemy);

    //�G�l�~�[�S�폜
    void Clear();

    //�f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

    //�f�o�b�O���`��
    void DrawDebugGUI();

    //�G�l�~�[���擾
    int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

    //�G�l�~�[�擾
    Enemy* GetEnemy(int index) { return enemies.at(index); }

    bool GetDie() { return die; }
    void SetDie(bool die) { this->die = die; }

private:
    //�G�l�~�[���m�̏Փˏ���
    void CollisionEnemyVsEnemies();

private:
    //�����̃G�l�~�[�̊Ǘ��̂��߂ɃG�l�~�[�|�C���^��std::vector�ŊǗ�
    std::vector<Enemy*> enemies;
    std::vector<Enemy*> removes;

    bool die = false;

};