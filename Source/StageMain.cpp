#include "StageMain.h"

//�R���X�g���N�^
StageMain::StageMain()
{
    //�X�e�[�W���f����ǂݍ���
    model = new Model("Data/Model/ExampleStage/M_stage (1).mdl");
    //���̉��Ń|�W�V�����A�X�P�[����ݒ�
    scale = { 1.0, 1.0, 1.0 };
    position = {0.0, -30.0, 0.0};
}

//�f�X�g���N�^
StageMain::~StageMain()
{
    //�X�e�[�W���f����j��
    delete model;
}

//�X�V����
void StageMain::Update(float elapsedTime)
{
    //�O��̏���ۑ�
    oldTransform = transform;
    oldAngle = angle;

    //�s��X�V
    UpdateTransform();


    //���f���s��X�V
    model->UpdateTransform(transform);
}

void StageMain::Render(RenderContext& rc, ModelShader* shader)
{
    rc.toonData.edgeWidth = 0.05f;
    //�V�F�[�_�[�Ƀ��f����`�悵�Ă��炤
    shader->Draw(rc, model);
}

//���C�L���X�g
bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}