#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include "Audio/Audio.h"
#include <memory>

class SceneGameOver : public Scene
{
public:
    SceneGameOver() {};

    ~SceneGameOver() override {};

    //初期化
    void Initialize() override;
    //終了化
    void Finalize() override;
    //更新処理
    void Update(float elapsedTime)override;
    //描画処理
    void Render()override;


private:
    std::unique_ptr<Sprite> bg;
    std::unique_ptr<Sprite> continue_tex;
    std::unique_ptr<Sprite> end_tex;

    //オーディオ
    std::unique_ptr<AudioSource> GameOver_Bgm;

    bool isTryagain = false;
    bool isMenu = false;
    std::unique_ptr <Sprite> texts[8];

    float screentime = 0.0f;

    DirectX::XMFLOAT2 pos;
};
