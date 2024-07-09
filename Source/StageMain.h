#pragma once

#include "Graphics/Model.h"
#include "Stage.h"

//ステージ
class StageMain : public Stage
{
public:
    StageMain();
    ~StageMain() override;

    //更新処理
    void Update(float elapsedTIme) override;

    //描画処理
    //void Render(ID3D11DeviceContext* dc, ModelShader* shader) override;
    void Render(RenderContext& rc, ModelShader* shader) override;

    //レイキャスト
    bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit) override;

private:
    Model* model = nullptr;
};
