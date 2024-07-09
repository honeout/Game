#pragma once

#include "Graphics/Shader.h"
#include "Character.h"

//�G�l�~�[
class Enemy : public Character
{
public:
    Enemy() {}
    ~Enemy() override {}

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
};