#include "StageManager.h"

//�X�V����
void StageManager::Update(float elapsedTime)
{
    for (Stage* stage : stages)
    {
        stage->Update(elapsedTime);
    }
}

//�`�揈��
void StageManager::Render(RenderContext& context, ModelShader* shader)
{
    for (Stage* stage : stages)
    {
        stage->Render(context, shader);
    }
}

//�X�e�[�W�o�^
void StageManager::Register(Stage* stage)
{
    stages.emplace_back(stage);
}

//�X�e�[�W�S�폜
void StageManager::Clear()
{
    for (Stage* stage : stages)
    {
        delete stage;
    }

    stages.clear();
}

//���C�L���X�g
bool StageManager::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    bool result = false;
    hit.distance = FLT_MAX;

    //���ׂẴX�e�[�W�I�u�W�F�N�g�ɑ΂��ă��C�L���X�g���s���A�Փ˂�����_����ԋ߂������擾����
    for (Stage* stage : stages)
    {
        HitResult tmp;
        if (stage->RayCast(start, end, tmp))
        {
            if (hit.distance > tmp.distance)
            {
                hit = tmp;
                result = true;
            }
        }
    }

    return result;
}
