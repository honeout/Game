#pragma once


#include "Graphics/Sprite.h"
#include "Graphics/Texture.h"
#include "Graphics/RenderTarget.h"
#include "Graphics/DepthStencil.h"

#include "Scene.h"
#include "Audio/Audio.h"

//タイトルシーン
class SceneTitle : public Scene
{
public:
    SceneTitle() {}
    ~SceneTitle()override {}

    //初期化
    void Initialize()override;

    //終了化
    void Finalize()override;

    //更新処理
    void Update(float elapsedTime) override;

    //描画処理
    void Render() override;

private:
    //CameraController* cameraController = nullptr;
    std::unique_ptr<Sprite> bg;    
    std::unique_ptr<Sprite> start;
    std::unique_ptr<Sprite> quit;

    // Toon深度レンダリング用描画ターゲット
    std::unique_ptr<DepthStencil> depthStencil;

    // トゥーン情報
    //ToonData			toonData;

    bool isTryagain = false;
    bool isMenu = false;
    std::unique_ptr <Sprite> texts[8];

    DirectX::XMFLOAT2 pos;
    float screentime = 0.0f;

    std::unique_ptr<AudioSource> Title_Bgm;

};