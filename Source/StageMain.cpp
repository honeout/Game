#include "StageMain.h"

//コンストラクタ
StageMain::StageMain()
{
    //ステージモデルを読み込み
    model = new Model("Data/Model/ExampleStage/M_stage (1).mdl");
    //この下でポジション、スケールを設定
    scale = { 1.0, 1.0, 1.0 };
    position = {0.0, -30.0, 0.0};
}

//デストラクタ
StageMain::~StageMain()
{
    //ステージモデルを破棄
    delete model;
}

//更新処理
void StageMain::Update(float elapsedTime)
{
    //前回の情報を保存
    oldTransform = transform;
    oldAngle = angle;

    //行列更新
    UpdateTransform();


    //モデル行列更新
    model->UpdateTransform(transform);
}

void StageMain::Render(RenderContext& rc, ModelShader* shader)
{
    rc.toonData.edgeWidth = 0.05f;
    //シェーダーにモデルを描画してもらう
    shader->Draw(rc, model);
}

//レイキャスト
bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}