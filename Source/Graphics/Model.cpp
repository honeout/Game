#include "Graphics/Graphics.h"
#include "Graphics/Model.h"
#include "ResourceManager.h"

// �R���X�g���N�^
Model::Model(const char* filename)
{
    // ���\�[�X�ǂݍ���
    //resource = std::make_shared<ModelResource>();
    //resource->Load(Graphics::Instance().GetDevice(), filename);

    // ���\�[�X�ǂݍ���
    resource = ResourceManager::instance().LoadModelResource(filename);

    // �m�[�h
    const std::vector<ModelResource::Node>& resNodes = resource->GetNodes();
    
    nodes.resize(resNodes.size());
    for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
    {
        auto&& src = resNodes.at(nodeIndex);
        auto&& dst = nodes.at(nodeIndex);

        dst.name = src.name.c_str();
        dst.parent = src.parentIndex >= 0 ? &nodes.at(src.parentIndex) : nullptr;
        dst.scale = src.scale;
        dst.rotate = src.rotate;
        dst.translate = src.translate;

        if (dst.parent != nullptr)
        {
            dst.parent->children.emplace_back(&dst);
        }
    }

    // �s��v�Z
    const DirectX::XMFLOAT4X4 transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
    UpdateTransform(transform);
}

// �ϊ��s��v�Z
void Model::UpdateTransform(const DirectX::XMFLOAT4X4& transform)
{
    DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);

    for (Node& node : nodes)
    {
        // ���[�J���s��Z�o
        DirectX::XMMATRIX S = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
        DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&node.rotate));
        DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(node.translate.x, node.translate.y, node.translate.z);
        DirectX::XMMATRIX LocalTransform = S * R * T;

        // ���[���h�s��Z�o
        DirectX::XMMATRIX ParentTransform;
        if (node.parent != nullptr)
        {
            ParentTransform = DirectX::XMLoadFloat4x4(&node.parent->worldTransform);
        }
        else
        {
            ParentTransform = Transform;
        }
        DirectX::XMMATRIX WorldTransform = LocalTransform * ParentTransform;

        // �v�Z���ʂ��i�[
        DirectX::XMStoreFloat4x4(&node.localTransform, LocalTransform);
        DirectX::XMStoreFloat4x4(&node.worldTransform, WorldTransform);
    }
}

//�A�j���[�V�����X�V����
void Model::UpdateAnimation(float elapsedTime)
{
    //// 13��
    //Node* node = FindNode("player_upper");
    //node->now = bonenowsize;
       

    //�Đ����łȂ��Ȃ珈�����Ȃ�
    if (!IsPlayAnimation()) return;

    //�u�����h���̌v�Z
    float blendRate = 1.0f;
    if (animationBlendTime < animationBlendSeconds)
    {
        animationBlendTime += elapsedTime;
        if (animationBlendTime >= animationBlendSeconds)
        {
            animationBlendTime = animationBlendSeconds;
        }
        blendRate = animationBlendTime / animationBlendSeconds;
        blendRate *= blendRate;
    }

    //�w��̃A�j���[�V�����f�[�^���擾
    const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
    const ModelResource::Animation& animation = animations.at(currentAnimationIndex);


    //�A�j���[�V�����f�[�^����L�[�t���[���f�[�^���X�g���擾
    const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
    int keyCount = static_cast<int>(keyframes.size());

   

  
    for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
    {
        //���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ��邩���肷��
        const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
        const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex + 1);
        if (currentAnimationSeconds >= keyframe0.seconds &&
            currentAnimationSeconds < keyframe1.seconds)
        {
            //�Đ����ԂƃL�[�t���[���̎��Ԃ���⊮�����Z�o����
            float rate = (currentAnimationSeconds - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);
            

            int nodeCount =  static_cast<int>(nodes.size());
            for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
            {
                //2�̃L�[�t���[���Ԃ̕⊮�v�Z
                const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
                const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

                Node& node = nodes[nodeIndex];
                
 

                //�u�����h�⊮����
                if (blendRate < 1.0f)
                {
                    //���݂̎p���Ǝ��̃L�[�t���[���Ƃ̎p���̕⊮
                    DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&node.scale);
                    DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
                    DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&node.rotate);
                    DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
                    DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&node.translate);
                    DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

                    DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, blendRate);
                    DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, blendRate);
                    DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, blendRate);

                    //�v�Z���ʂ��{�[���Ɋi�[
                    DirectX::XMStoreFloat3(&node.scale, S);
                    DirectX::XMStoreFloat4(&node.rotate, R);
                    DirectX::XMStoreFloat3(&node.translate, T);
                }

                else
                {
                    //�O�̃L�[�t���[���Ǝ��̃L�[�t���[���̎p����⊮
                    DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
                    DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
                    DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
                    DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
                    DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
                    DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

                    DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);
                    DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);
                    DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, rate);

                    //�v�Z���ʂ��{�[���Ɋi�[
                    DirectX::XMStoreFloat3(&node.scale, S);
                    DirectX::XMStoreFloat4(&node.rotate, R);
                    DirectX::XMStoreFloat3(&node.translate, T);
                }
            }
            break;
        }
    }

    //�ŏI�t���[������
    if (animationEndFlag)
    {
        animationEndFlag = false;
        currentAnimationIndex = -1;
        return;
    }

    //���Ԍo��
    currentAnimationSeconds += elapsedTime;

    //�Đ����Ԃ��T�Z���Ԃ𒴂�����
    if (currentAnimationSeconds >= animation.secondsLength)
    {
        //�Đ����Ԃ������߂�
        if (animationLoopFlag)
        {
            currentAnimationSeconds -= animation.secondsLength;
        }
        //�Đ��I�����Ԃɂ���
        else
        {
            currentAnimationSeconds = animation.secondsLength;
            animationEndFlag = true;
        }
    }

}



//�A�j���[�V�����X�V����
void Model::ReverseplaybackAnimation(float elapsedTime)
{
    //�Đ����łȂ��Ȃ珈�����Ȃ�
    if (!IsPlayAnimation()) return;

    //�u�����h���̌v�Z
    float blendRate = 1.0f;
    if (animationBlendTime < animationBlendSeconds)
    {
        animationBlendTime += elapsedTime;
        if (animationBlendTime >= animationBlendSeconds)
        {
            animationBlendTime = animationBlendSeconds;
        }
        blendRate = animationBlendTime / animationBlendSeconds;
        blendRate *= blendRate;
    }

    //�w��̃A�j���[�V�����f�[�^���擾
    const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
    const ModelResource::Animation& animation = animations.at(currentAnimationIndex);
    if (anim)
    {
        currentAnimationSeconds = animation.secondsLength;
        anim = false;
    }

    //�A�j���[�V�����f�[�^����L�[�t���[���f�[�^���X�g���擾
    const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
    int keyCount = static_cast<int>(keyframes.size());
    for (int keyIndex = keyCount - 1; keyIndex > 0; --keyIndex)
    {

        //���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ��邩���肷��
        const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
        const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex - 1);
        if (currentAnimationSeconds <= keyframe0.seconds &&
            currentAnimationSeconds > keyframe1.seconds)
        {
            //�Đ����ԂƃL�[�t���[���̎��Ԃ���⊮�����Z�o����
            float rate = (currentAnimationSeconds - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);

            // 12

            int nodeCount = static_cast<int>(nodes.size());
            for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
            {
                //2�̃L�[�t���[���Ԃ̕⊮�v�Z
                const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
                const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

                Node& node = nodes[nodeIndex];

                //�u�����h�⊮����
                if (blendRate < 1.0f)
                {
                    //���݂̎p���Ǝ��̃L�[�t���[���Ƃ̎p���̕⊮
                    DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&node.scale);
                    DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
                    DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&node.rotate);
                    DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
                    DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&node.translate);
                    DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

                    DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, blendRate);
                    DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, blendRate);
                    DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, blendRate);

                    //�v�Z���ʂ��{�[���Ɋi�[
                    DirectX::XMStoreFloat3(&node.scale, S);
                    DirectX::XMStoreFloat4(&node.rotate, R);
                    DirectX::XMStoreFloat3(&node.translate, T);
                }

                else
                {
                    //�O�̃L�[�t���[���Ǝ��̃L�[�t���[���̎p����⊮
                    DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
                    DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
                    DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
                    DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
                    DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
                    DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

                    DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);
                    DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);
                    DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, rate);

                    //�v�Z���ʂ��{�[���Ɋi�[
                    DirectX::XMStoreFloat3(&node.scale, S);
                    DirectX::XMStoreFloat4(&node.rotate, R);
                    DirectX::XMStoreFloat3(&node.translate, T);
                }
            }
            break;
        }
    }

    //�ŏI�t���[������
    if (animationEndFlag)
    {
        animationEndFlag = false;
        currentAnimationIndex = -1;
        anim = true;
        return;
    }

    //���Ԍo��
    currentAnimationSeconds -= elapsedTime;

    //�Đ����Ԃ��T�Z���Ԃ𒴂�����
    if (currentAnimationSeconds <= 0)
    {
        //�Đ����Ԃ������߂�
        if (animationLoopFlag)
        {
            currentAnimationSeconds += animation.secondsLength;
        }
        //�Đ��I�����Ԃɂ���
        else
        {
            currentAnimationSeconds = 0;
            animationEndFlag = true;
        }
    }

}

void Model::ReverseplaybackUpeerBodyAnimation(float elapsedTime, const char* start)
{
    Model::Node* node  = FindNode(start);

    //�Đ����łȂ��Ȃ珈�����Ȃ�
    if (!IsPlayUpeerBodyAnimation()) return;

    //�u�����h���̌v�Z
    float blendRate = 1.0f;
    if (animationBlendTimeUpeer < animationBlendSecondsUpeer)
    {
        animationBlendTimeUpeer += elapsedTime;
        if (animationBlendTimeUpeer >= animationBlendSecondsUpeer)
        {
            animationBlendTimeUpeer = animationBlendSecondsUpeer;
        }
        blendRate = animationBlendTimeUpeer / animationBlendSecondsUpeer;
        blendRate *= blendRate;
    }

    //�w��̃A�j���[�V�����f�[�^���擾
    const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
    const ModelResource::Animation& animation = animations.at(currentAnimationIndexUpeer);

    //�A�j���[�V�����f�[�^����L�[�t���[���f�[�^���X�g���擾
    const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
    int keyCount = static_cast<int>(keyframes.size());


    if (anim)
    {
        currentAnimationSecondsUpeer = animation.secondsLength;
        anim = false;
    }

    for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
    {
        //���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ��邩���肷��
        const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
        const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex + 1);
        if (currentAnimationSecondsUpeer >= keyframe0.seconds &&
            currentAnimationSecondsUpeer < keyframe1.seconds)
        {
            //�Đ����ԂƃL�[�t���[���̎��Ԃ���⊮�����Z�o����
            float rate = (currentAnimationSecondsUpeer - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);

            // 13

            // �㔼�g�A�j���[�V����
            int nodeCount = static_cast<int>(nodes.size());
            for (int nodeIndex = node->now; nodeIndex < nodeCount; ++nodeIndex)
            {




                //2�̃L�[�t���[���Ԃ̕⊮�v�Z
                const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
                const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

                Node& node = nodes[nodeIndex];




                //�u�����h�⊮����
                if (blendRate < 1.0f)
                {
                    //���݂̎p���Ǝ��̃L�[�t���[���Ƃ̎p���̕⊮
                    DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&node.scale);
                    DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
                    DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&node.rotate);
                    DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
                    DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&node.translate);
                    DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

                    DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, blendRate);
                    DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, blendRate);
                    DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, blendRate);

                    //�v�Z���ʂ��{�[���Ɋi�[
                    DirectX::XMStoreFloat3(&node.scale, S);
                    DirectX::XMStoreFloat4(&node.rotate, R);
                    DirectX::XMStoreFloat3(&node.translate, T);
                }

                else
                {
                    //�O�̃L�[�t���[���Ǝ��̃L�[�t���[���̎p����⊮
                    DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
                    DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
                    DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
                    DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
                    DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
                    DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

                    DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);
                    DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);
                    DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, rate);

                    //�v�Z���ʂ��{�[���Ɋi�[
                    DirectX::XMStoreFloat3(&node.scale, S);
                    DirectX::XMStoreFloat4(&node.rotate, R);
                    DirectX::XMStoreFloat3(&node.translate, T);
                }
            }
            break;
        }
    }

    //�ŏI�t���[������
    if (animationEndFlagUpeer)
    {
        animationEndFlagUpeer = false;
        currentAnimationIndexUpeer = -1;
        return;
    }

    //���Ԍo��
    currentAnimationSecondsUpeer -= elapsedTime;

    //�Đ����Ԃ��T�Z���Ԃ𒴂�����
    if (currentAnimationSecondsUpeer <= 0)
    {
        //�Đ����Ԃ������߂�
        if (animationLoopFlagUpeer)
        {
            currentAnimationSecondsUpeer += animation.secondsLength;
        }
        //�Đ��I�����Ԃɂ���
        else
        {
            currentAnimationSecondsUpeer = 0;
            animationEndFlagUpeer = true;
        }
    }
}



// �㔼�g�p�A�j���[�V����
void Model::UpdateUpeerBodyAnimation(float elapsedTime, const char* start)
{
    Model::Node* node = FindNode(start);
    //�Đ����łȂ��Ȃ珈�����Ȃ�
    if (!IsPlayUpeerBodyAnimation()) return;

    //�u�����h���̌v�Z
    float blendRate = 1.0f;
    if (animationBlendTimeUpeer < animationBlendSecondsUpeer)
    {
        animationBlendTimeUpeer += elapsedTime;
        if (animationBlendTimeUpeer >= animationBlendSecondsUpeer)
        {
            animationBlendTimeUpeer = animationBlendSecondsUpeer;
        }
        blendRate = animationBlendTimeUpeer / animationBlendSecondsUpeer;
        blendRate *= blendRate;
    }

    //�w��̃A�j���[�V�����f�[�^���擾
    const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
    const ModelResource::Animation& animation = animations.at(currentAnimationIndexUpeer);

    //�A�j���[�V�����f�[�^����L�[�t���[���f�[�^���X�g���擾
    const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
    int keyCount = static_cast<int>(keyframes.size());
    for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
    {
        //���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ��邩���肷��
        const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
        const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex + 1);
        if (currentAnimationSecondsUpeer >= keyframe0.seconds &&
            currentAnimationSecondsUpeer < keyframe1.seconds)
        {
            //�Đ����ԂƃL�[�t���[���̎��Ԃ���⊮�����Z�o����
            float rate = (currentAnimationSecondsUpeer - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);

            // 13

            // �㔼�g�A�j���[�V����
            int nodeCount = static_cast<int>(nodes.size());
            for (int nodeIndex = node->now; nodeIndex < nodeCount; ++nodeIndex)
            {
                if (nodeIndex != 10 && nodeIndex != 20)
                {



                    //2�̃L�[�t���[���Ԃ̕⊮�v�Z
                    const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
                    const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

                    Node& node = nodes[nodeIndex];




                    //�u�����h�⊮����
                    if (blendRate < 1.0f)
                    {
                        //���݂̎p���Ǝ��̃L�[�t���[���Ƃ̎p���̕⊮
                        DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&node.scale);
                        DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
                        DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&node.rotate);
                        DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
                        DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&node.translate);
                        DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

                        DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, blendRate);
                        DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, blendRate);
                        DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, blendRate);

                        //�v�Z���ʂ��{�[���Ɋi�[
                        DirectX::XMStoreFloat3(&node.scale, S);
                        DirectX::XMStoreFloat4(&node.rotate, R);
                        DirectX::XMStoreFloat3(&node.translate, T);
                    }

                    else
                    {
                        //�O�̃L�[�t���[���Ǝ��̃L�[�t���[���̎p����⊮
                        DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
                        DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
                        DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
                        DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
                        DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
                        DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

                        DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);
                        DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);
                        DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, rate);

                        //�v�Z���ʂ��{�[���Ɋi�[
                        DirectX::XMStoreFloat3(&node.scale, S);
                        DirectX::XMStoreFloat4(&node.rotate, R);
                        DirectX::XMStoreFloat3(&node.translate, T);
                    }
                }
            }
            break;
        }
    }

    //�ŏI�t���[������
    if (animationEndFlagUpeer)
    {
        animationEndFlagUpeer = false;
        currentAnimationIndexUpeer = -1;
        return;
    }

    //���Ԍo��
    currentAnimationSecondsUpeer += elapsedTime;

    //�Đ����Ԃ��T�Z���Ԃ𒴂�����
    if (currentAnimationSecondsUpeer >= animation.secondsLength)
    {
        //�Đ����Ԃ������߂�
        if (animationLoopFlagUpeer)
        {
            currentAnimationSecondsUpeer -= animation.secondsLength;
        }
        //�Đ��I�����Ԃɂ���
        else
        {
            currentAnimationSecondsUpeer = animation.secondsLength;
            animationEndFlagUpeer = true;
        }
    }
}

// �����g�p
void Model::UpdateLowerBodyAnimation(float elapsedTime, const char* end)
{
    Model::Node* node = FindNode(end);
    //�Đ����łȂ��Ȃ珈�����Ȃ�
    if (!IsPlayAnimation()) return;

        //�u�����h���̌v�Z
        float blendRate = 1.0f;
    if (animationBlendTime < animationBlendSeconds)
    {
        animationBlendTime += elapsedTime;
        if (animationBlendTime >= animationBlendSeconds)
        {
            animationBlendTime = animationBlendSeconds;
        }
        blendRate = animationBlendTime / animationBlendSeconds;
        blendRate *= blendRate;
    }

    //�w��̃A�j���[�V�����f�[�^���擾
    const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
    const ModelResource::Animation& animation = animations.at(currentAnimationIndex);

    //�A�j���[�V�����f�[�^����L�[�t���[���f�[�^���X�g���擾
    const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;

    // �Đ�
    int keyCount = static_cast<int>(keyframes.size());
    for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
    {

        //���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ��邩���肷��
        const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
        const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex + 1);
        if (currentAnimationSeconds >= keyframe0.seconds &&
            currentAnimationSeconds < keyframe1.seconds)
        {
            //�Đ����ԂƃL�[�t���[���̎��Ԃ���⊮�����Z�o����
            float rate = (currentAnimationSeconds - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);

            // 12

            int nodeCount = static_cast<int>(node->now);
            for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
            {
                //2�̃L�[�t���[���Ԃ̕⊮�v�Z
                const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
                const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

                Node& node = nodes[nodeIndex];

                //�u�����h�⊮����
                if (blendRate < 1.0f)
                {
                    //���݂̎p���Ǝ��̃L�[�t���[���Ƃ̎p���̕⊮
                    DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&node.scale);
                    DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
                    DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&node.rotate);
                    DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
                    DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&node.translate);
                    DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

                    DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, blendRate);
                    DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, blendRate);
                    DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, blendRate);

                    //�v�Z���ʂ��{�[���Ɋi�[
                    DirectX::XMStoreFloat3(&node.scale, S);
                    DirectX::XMStoreFloat4(&node.rotate, R);
                    DirectX::XMStoreFloat3(&node.translate, T);
                }

                else
                {
                    //�O�̃L�[�t���[���Ǝ��̃L�[�t���[���̎p����⊮
                    DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
                    DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
                    DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
                    DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
                    DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
                    DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

                    DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);
                    DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);
                    DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, rate);

                    //�v�Z���ʂ��{�[���Ɋi�[
                    DirectX::XMStoreFloat3(&node.scale, S);
                    DirectX::XMStoreFloat4(&node.rotate, R);
                    DirectX::XMStoreFloat3(&node.translate, T);
                }
            }
            break;
        }
    }

    //�ŏI�t���[������
    if (animationEndFlag)
    {
        animationEndFlag = false;
        currentAnimationIndex = -1;
        return;
    }

    //���Ԍo��
    currentAnimationSeconds += elapsedTime;

    //�Đ����Ԃ��T�Z���Ԃ𒴂�����
    if (currentAnimationSeconds >= animation.secondsLength)
    {
        //�Đ����Ԃ������߂�
        if (animationLoopFlag)
        {
            currentAnimationSeconds -= animation.secondsLength;
        }
        //�Đ��I�����Ԃɂ���
        else
        {
            currentAnimationSeconds = animation.secondsLength;
            animationEndFlag = true;
        }
    }
}

// �t�Đ�
void Model::ReverseplaybackLowerBodyAnimation(float elapsedTime, const char* end)
{
    Model::Node* node = FindNode(end);
    //�Đ����łȂ��Ȃ珈�����Ȃ�
    if (!IsPlayAnimation()) return;

    //�u�����h���̌v�Z
    float blendRate = 1.0f;
    if (animationBlendTime < animationBlendSeconds)
    {
        animationBlendTime += elapsedTime;
        if (animationBlendTime >= animationBlendSeconds)
        {
            animationBlendTime = animationBlendSeconds;
        }
        blendRate = animationBlendTime / animationBlendSeconds;
        blendRate *= blendRate;
    }

    //�w��̃A�j���[�V�����f�[�^���擾
    const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
    const ModelResource::Animation& animation = animations.at(currentAnimationIndex);
    if (anim)
    {
        currentAnimationSeconds = animation.secondsLength;
        anim = false;
    }
    
    //�A�j���[�V�����f�[�^����L�[�t���[���f�[�^���X�g���擾
    const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;

    // �t�Đ�
    int keyCount = static_cast<int>(keyframes.size());
    for (int keyIndex = keyCount - 1; keyIndex > 0; --keyIndex)
    {

        //���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ��邩���肷��
        const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
        const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex - 1);
        if (currentAnimationSeconds <= keyframe0.seconds &&
            currentAnimationSeconds > keyframe1.seconds)
        {
            //�Đ����ԂƃL�[�t���[���̎��Ԃ���⊮�����Z�o����
            float rate = (currentAnimationSeconds - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);

            // 12

            int nodeCount = static_cast<int>(node->now);
            for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
            {
                //2�̃L�[�t���[���Ԃ̕⊮�v�Z
                const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
                const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

                Node& node = nodes[nodeIndex];

                //�u�����h�⊮����
                if (blendRate < 1.0f)
                {
                    //���݂̎p���Ǝ��̃L�[�t���[���Ƃ̎p���̕⊮
                    DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&node.scale);
                    DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
                    DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&node.rotate);
                    DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
                    DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&node.translate);
                    DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

                    DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, blendRate);
                    DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, blendRate);
                    DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, blendRate);

                    //�v�Z���ʂ��{�[���Ɋi�[
                    DirectX::XMStoreFloat3(&node.scale, S);
                    DirectX::XMStoreFloat4(&node.rotate, R);
                    DirectX::XMStoreFloat3(&node.translate, T);
                }

                else
                {
                    //�O�̃L�[�t���[���Ǝ��̃L�[�t���[���̎p����⊮
                    DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
                    DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
                    DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
                    DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
                    DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
                    DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

                    DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);
                    DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);
                    DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, rate);

                    //�v�Z���ʂ��{�[���Ɋi�[
                    DirectX::XMStoreFloat3(&node.scale, S);
                    DirectX::XMStoreFloat4(&node.rotate, R);
                    DirectX::XMStoreFloat3(&node.translate, T);
                }
            }
            break;
        }
    }

    //�ŏI�t���[������
    if (animationEndFlag)
    {
        animationEndFlag = false;
        currentAnimationIndex = -1;
        anim = true;
        return;
    }

    //���Ԍo��
    currentAnimationSeconds -= elapsedTime;

    //�Đ����Ԃ��T�Z���Ԃ𒴂�����
    if (currentAnimationSeconds <= 0)
    {
        //�Đ����Ԃ������߂�
        if (animationLoopFlag)
        {
            currentAnimationSeconds += animation.secondsLength;
        }
        //�Đ��I�����Ԃɂ���
        else
        {
            currentAnimationSeconds = 0;
            animationEndFlag = true;
        }
    }
}

//�A�j���[�V�����Đ�
void Model::PlayAnimation(int index, bool loop, float blendSeconds)
{
    currentAnimationIndex = index;
    currentAnimationSeconds = 0.0f;
    animationLoopFlag = loop;
    animationEndFlag = false;
    animationBlendTime = 0.0f;
    animationBlendSeconds = blendSeconds;
  
}

void Model::PlayReverseAnimation(int index, bool loop, float blendSeconds)
{
    currentAnimationIndex = index;
    currentAnimationSeconds = 0.0f;
    animationLoopFlag = loop;
    animationEndFlag = false;
    animationBlendTime = 0.0f;
    animationBlendSeconds = blendSeconds;
}

void Model::PlayUpeerBodyAnimation(int index, bool loop, float blendSeconds)
{
    currentAnimationIndexUpeer = index;
    currentAnimationSecondsUpeer = 0.0f;
    animationLoopFlagUpeer = loop;
    animationEndFlagUpeer = false;
    animationBlendTimeUpeer = 0.0f;
    animationBlendSecondsUpeer = blendSeconds;
}

void Model::PlayLowerBodyAnimation(int index, bool loop, float blendSeconds)
{
    currentAnimationIndex = index;
    currentAnimationSeconds = 0.0f;
    animationLoopFlag = loop;
    animationEndFlag = false;
    animationBlendTime = 0.0f;
    animationBlendSeconds = blendSeconds;
}

//�A�j���[�V�����Đ�����
bool Model::IsPlayAnimation() const
{
    if (currentAnimationIndex < 0) return false;
    if (currentAnimationIndex >= resource->GetAnimations().size()) return false;
    return true;
}

//�A�j���[�V�����Đ�����
bool Model::IsPlayUpeerBodyAnimation() const
{
    if (currentAnimationIndexUpeer < 0) return false;
    if (currentAnimationIndexUpeer >= resource->GetAnimations().size()) return false;
    return true;
}

//�m�[�h����
Model::Node* Model::FindNode(const char* name)
{
    bonenowsize = 0;

    //���ׂẴm�[�h�𑍓���Ŗ��O��r����
    for (Node& node : nodes)
    {

        if (::strcmp(node.name, name) == 0)
        {
            node.now = bonenowsize;
            return &node;
        }
        ++bonenowsize;
    }

    //������Ȃ�����
    return nullptr;
}