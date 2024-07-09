#pragma once

#include "Graphics/Shader.h"
#include "Character.h"

//エネミー
class Enemy : public Character
{
public:
    Enemy() {}
    ~Enemy() override {}

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
};