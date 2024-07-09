#pragma once
#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "Effect.h"

//�v���C���[
class Player : public Character
{
public:
    Player() {};
    ~Player() override {};

    //�X�V����
    virtual void Update(float elapsedTime) = 0;

    //�`�揈��
    virtual void Render(RenderContext& rc, ModelShader* shader) = 0;

    //�j��
    void Destroy();

    //�f�o�b�O�v���~�e�B�u�`��
    virtual void DrawDebugPrimitive();

    //�f�o�b�O���`��
    virtual void DrawDebugGUI();

    //�C���X�^���X�擾
    //static Player& Instance();
};