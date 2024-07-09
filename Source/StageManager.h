#pragma once
#include <vector>
#include "Stage.h"

//�N���X�}�l�[�W���[
class StageManager
{
private:
    StageManager() {}
    ~StageManager() {}

public:
    //�B��̃C���X�^���X�擾
    static StageManager& Instance()
    {
        static StageManager instance;
        return instance;
    }

    //�X�V����
    void Update(float elapsedTime);

    //�`�揈��
    void Render(RenderContext& rc, ModelShader* shader);

    //�X�e�[�W�o�^
    void Register(Stage* stage);

    //�X�e�[�W�S�폜
    void Clear();

    //���C�L���X�g
    bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit);

private:
    std::vector<Stage*> stages;
};