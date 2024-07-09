#pragma once

#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "Graphics/ModelResource.h"

// モデル
class Model
{
public:
    Model(const char* filename);
    ~Model() {}

    struct Node
    {
        const char* name;
        Node* parent;
        DirectX::XMFLOAT3	scale;
        DirectX::XMFLOAT4	rotate;
        DirectX::XMFLOAT3	translate;
        DirectX::XMFLOAT4X4	localTransform;
        DirectX::XMFLOAT4X4	worldTransform;

        std::vector<Node*>	children;

        int now;
    };



    // 行列計算
    void UpdateTransform(const DirectX::XMFLOAT4X4& transform);

    // ノードリスト取得
    const std::vector<Node>& GetNodes() const { return nodes; }
    std::vector<Node>& GetNodes() { return nodes; }

    // リソース取得
    const ModelResource* GetResource() const { return resource.get(); }

    //アニメーション更新処理
    void UpdateAnimation(float elapsedTime);

    // 逆再生
    void ReverseplaybackAnimation(float elapsedTime);

    //上半身アニメーション更新処理
    void UpdateUpeerBodyAnimation(float elapsedTime,  const char* start);

    // 上半身用アニメーション更新
    void ReverseplaybackUpeerBodyAnimation(float elapsedTime, const char* start);
    
    //下半身アニメーション更新処理
    void UpdateLowerBodyAnimation(float elapsedTime, const char* end);

    void ReverseplaybackLowerBodyAnimation(float elapsedTime, const char* end);

    //アニメーション再生
    void PlayAnimation(int index, bool loop, float blendSeconds = 0.2f);

    //逆再生アニメーション
    void PlayReverseAnimation(int index, bool loop, float blendSeconds = 0.2f);

    //アニメーション再生
    void PlayUpeerBodyAnimation(int index, bool loop, float blendSeconds = 0.2f);

    //アニメーション再生
    void PlayLowerBodyAnimation(int index, bool loop, float blendSeconds = 0.2f);

    //アニメーション再生中か
    bool IsPlayAnimation() const;
    
    bool IsPlayUpeerBodyAnimation() const;

    //ノード検索
    Node* FindNode(const char* name);

    //現在のアニメーション再生時間取得
    float GetCurrentAnimationSeconds() const { return currentAnimationSeconds; }
    float GetCurrentUpperAnimationSeconds() const { return currentAnimationSecondsUpeer; }

    // 上半身のアニメーション再生時間を変更
    void SetCurrentAnimationSecondsUpeer(float currentAnimationSeconds) { this->currentAnimationSecondsUpeer = currentAnimationSeconds; }

    int GetCurrentAnimationIndexUpeer() const { return currentAnimationIndexUpeer; }

private:
    std::shared_ptr<ModelResource>	resource;	//
    std::vector<Node>				nodes;		//
    int currentAnimationIndex = -1;				//アニメーション番号
    float currentAnimationSeconds = 0.0f;		//アニメーションの再生用時間
    bool animationLoopFlag = false;				//アニメーションがループするか
    bool animationEndFlag = false;				//アニメーションが再生終了しているか
    float animationBlendTime = 0.0f;			//アニメーション補完時間
    float animationBlendSeconds = 0.0f;			//〃

    // 上半身用のアニメーション時間
    int currentAnimationIndexUpeer = -1;				//アニメーション番号
    float currentAnimationSecondsUpeer = 0.0f;		//アニメーションの再生用時間
    bool animationLoopFlagUpeer = false;				//アニメーションがループするか
    bool animationEndFlagUpeer = false;				//アニメーションが再生終了しているか
    float animationBlendTimeUpeer = 0.0f;			//アニメーション補完時間
    float animationBlendSecondsUpeer = 0.0f;			//〃

    // 上半身のキーフレーム時間
    int keyframeUpper = 0;
    // 下半身のキーフレーム時間
    int keyframeLower = 0;

    // 再生　逆再生
    bool anim = true;

    int bonenowsize = 0;
};
