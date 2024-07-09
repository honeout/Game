#pragma once
#include "Player.h"
#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"
#include <memory>
#include "Audio/Audio.h"
using namespace Microsoft::WRL;


// ゲームシーン
class SceneClear : public Scene
{
public:
    //SceneGame() {}
    ~SceneClear() override {}

    // 初期化
    void Initialize()override;

    // 終了化
    void Finalize()override;

    // 更新処理
    void Update(float elapsedTime)override;

    // 描画処理
    void Render()override;


private:
    std::unique_ptr<Sprite> bg;
    std::unique_ptr<Sprite> Retry;
    DirectX::XMFLOAT2 pos = { 1280 * 0.5f,720 * 0.75f };
    std::unique_ptr <Sprite> texts[8];

    std::unique_ptr<AudioSource> Clear_Bgm;
};
