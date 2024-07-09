#pragma once

#include "Graphics/Model.h"
#include "Graphics/Sprite.h"
#include "Graphics/Texture.h"
#include "Graphics/RenderTarget.h"
#include "Graphics/DepthStencil.h"
#include "Light.h"
#include "PostprocessingRenderer.h"
#include "CameraController.h"
#include "Scene.h"
#include "Weapon.h"
#include "Audio/Audio.h"

// ゲームシーン
class SceneGame : public Scene
{
public:
    SceneGame() {}
    ~SceneGame() override {}

    // 初期化
    void Initialize() override;

    // 終了化
    void Finalize() override;

    // 更新処理
    void Update(float elapsedTime) override;

    // 描画処理
    void Render() override;

    // 描画処理
    void DrawDebugParameter(DirectX::XMFLOAT4X4& transform, const char* label);

    // グリッド描画
    void DrawGrid(ID3D11DeviceContext* context, int subdivisions, float scale);

private:
    // 3D空間の描画
    void Render3DScene();

    // シャドウマップの描画
    void RenderShadowmap();

    // 深度の描画
    void RenderDepth();

private:
    //std::unique_ptr<Model>	jummo;
    //std::unique_ptr<Model>	uncle;
    //std::unique_ptr<Model>	stage;
    //DirectX::XMFLOAT4X4		jummo_transform = {1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1};
    //DirectX::XMFLOAT4X4		uncle_transform = {1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1};
    //DirectX::XMFLOAT4X4		stage_transform = {1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1};
    std::unique_ptr<Sprite>	sprite;
    std::unique_ptr<Sprite>	fadeout;
    std::unique_ptr<Texture> texture;
    CameraController* cameraController = nullptr;
    Sprite* gauge = nullptr;
    
    // UV スクロールデータ
    UVScrollData			uvScrollData;

    // マスク画像
    std::unique_ptr<Texture>	maskTexture;
    //Texture*	maskTexture;
    float						dissolveThreshold;
    float						edgeThreshold;		// 縁の閾値
    DirectX::XMFLOAT4			edgeColor;			// 縁の色

    // オフスクリーンレンダリング用描画ターゲット
    std::unique_ptr<RenderTarget> renderTarget;

    // シャドウマップ用情報
    Light* mainDirectionalLight = nullptr;					// シャドウマップを生成する平行光源
    DirectX::XMFLOAT3 DirectionLight = { 0,0,0 }; //　シャドウマップの法線 
    //std::unique_ptr<DepthStencil> shadowmapDepthStencil;	// シャドウマップ用深度ステンシルバッファ
    //float shadowDrawRect = 500.0f;							// シャドウマップに描画する範囲
    //DirectX::XMFLOAT4X4 lightViewProjection;				// ライトビュープロジェクション行列
    //DirectX::XMFLOAT3 shadowColor = { 0.2f, 0.2f, 0.2f };	// 影の色
    //float shadowBias = 0.001f;								// 深度比較用のオフセット値

    //	シャドウマップ用深度ステンシルバッファ
    std::unique_ptr<DepthStencil>	shadowmapDepthStencil[ShadowmapCount];
    //	ライトビュープロジェクション行列
    DirectX::XMFLOAT4X4				lightViewProjection[ShadowmapCount];
    //	深度比較用のオフセット値
    //TODO バイアスの変更
    //細かくしたらおそらく重くなる
    float							shadowBias[ShadowmapCount] = { 0.0001f, 0.002f, 0.004f, 0.01f };
    //	影の色
    DirectX::XMFLOAT3				shadowColor = { 0.2f, 0.2f, 0.2f };

    // ポストプロセス
    std::unique_ptr<PostprocessingRenderer> postprocessingRenderer;
    // SkyBoxテクスチャー
    std::unique_ptr<Texture> skyboxTexture;

    // Toon深度レンダリング用描画ターゲット
    std::unique_ptr<DepthStencil> depthStencil;

    // トゥーン情報
    ToonData			toonData;

    // 2D用画面に渡す為のカメラ目線等
    DirectX::XMFLOAT4X4 view;
    DirectX::XMFLOAT4X4 projection;

    //マウスを真ん中に戻すためのタイム
    float mousetime;

    std::unique_ptr<AudioSource> Game_Bgm;

    bool BgmStart = false;

    float alphafout = 0.0f;

    bool changeangle = false;

};