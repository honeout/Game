#pragma once
#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "Effect.h"

//プレイヤー
class Player : public Character
{
public:
    Player() {};
    ~Player() override {};

    //更新処理
    virtual void Update(float elapsedTime) = 0;

    //描画処理
    virtual void Render(RenderContext& rc, ModelShader* shader) = 0;

    //破棄
    void Destroy();

    //デバッグプリミティブ描画
    virtual void DrawDebugPrimitive();

    //デバッグ情報描画
    virtual void DrawDebugGUI();

    //インスタンス取得
    //static Player& Instance();
};