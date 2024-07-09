#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

// UV �X�N���[�����
struct UVScrollData
{
	DirectX::XMFLOAT2 uvScrollValue;	// UV �X�N���[���l
};

// �}�X�N�f�[�^
struct MaskData
{
	ID3D11ShaderResourceView*	maskTexture;
	float						dissolveThreshold;
	float						edgeThreshold;		// ����臒l
	DirectX::XMFLOAT4			edgeColor;			// ���̐F
};

// ���s�������
struct DirectionalLightData
{
	DirectX::XMFLOAT4 direction;	// ����
	DirectX::XMFLOAT4 color;		// �F
};

// �_�������
struct PointLightData
{
	DirectX::XMFLOAT4	position;	// ���W
	DirectX::XMFLOAT4	color;		// �F
	float				range;		// �͈�
	DirectX::XMFLOAT3	dummy;
};

// �_�����̍ő吔
static constexpr int PointLightMax = 8;

// �X�|�b�g���C�g
struct SpotLightData
{
	DirectX::XMFLOAT4	position;	// ���W
	DirectX::XMFLOAT4	direction;	// ����
	DirectX::XMFLOAT4	color;		// �F
	float				range;		// �͈�
	float				innerCorn;	// �C���i�[�p�x�͈�
	float				outerCorn;	// �A�E�^�[�p�x�͈�
	float				dummy;
};

// �X�|�b�g���C�g�̍ő吔
static constexpr int SpotLightMax = 8;

// �F���␳���
struct ColorGradingData
{
	float	hueShift = 0;	// �F������
	float	saturation = 1;	// �ʓx����
	float	brightness = 1;	// ���x����
	float	dummy;
};

// �K�E�X�t�B���^�[�v�Z���
struct GaussianFilterData
{
	int		kernelSize	= 8;		// �J�[�l���T�C�Y
	float	deviation	= 10.0f;	// �W���΍�
	DirectX::XMFLOAT2 textureSize;	// �ڂ����e�N�X�`���̃T�C�Y
};

// �K�E�X�t�B���^�[�̍ő�J�[�l���T�C�Y
static const int MaxKernelSize = 16;

// ���P�x���o�p���
struct LuminanceExtractionData
{
	float	threshold = 0.5f;	// 臒l
	float	intensity = 1.0f;	// �u���[���̋��x
	DirectX::XMFLOAT2 dummy2;
};

// �|�X�g�G�t�F�N�g�̍ŏI�p�X�p���
struct FinalpassData
{
	// �u���[���e�N�X�`��
	ID3D11ShaderResourceView* bloomTexture;
};


//	�V���h�E�}�b�v�̖���
static	const	int	ShadowmapCount = 4;

// �V���h�E�}�b�v�p���
struct ShadowMapData 
{
	//ID3D11ShaderResourceView* shadowMap;				//	�V���h�E�}�b�v�e�N�X�`��
	//DirectX::XMFLOAT4X4			lightViewProjection;	//	���C�g�r���[�v���W�F�N�V�����s��
	//DirectX::XMFLOAT3			shadowColor;			//	�e�̐F
	//float						shadowBias;				//	�[�x��r�p�̃I�t�Z�b�g�l

	//	�V���h�E�}�b�v�p�[�x�X�e���V���o�b�t�@
	ID3D11ShaderResourceView* shadowMap[ShadowmapCount];
	//	���C�g�r���[�v���W�F�N�V�����s��
	DirectX::XMFLOAT4X4		lightViewProjection[ShadowmapCount];
	//	�e�̐F
	DirectX::XMFLOAT3			shadowColor;
	//	�[�x��r�p�̃I�t�Z�b�g�l
	float					shadowBias[ShadowmapCount];

};

struct ToonData
{
	ID3D11ShaderResourceView* edgeTexture;
	DirectX::XMFLOAT4 edgeColor = { 0,0,0,1 };		// �F
	float			  edgeWidth = 0.001f;

};

//struct PrimitiveRender
//{
//
//};




// �����_�[�R���e�L�X�g
struct RenderContext
{
	ID3D11DeviceContext*	deviceContext;

	//	�J�������
	DirectX::XMFLOAT4		viewPosition;
	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;

	// �X�N���[���f�[�^
	UVScrollData			uvScrollData;

	// �}�X�N�f�[�^
	MaskData				maskData;

	// ���C�g���
	DirectX::XMFLOAT4		ambientLightColor;
	DirectionalLightData	directionalLightData;
	PointLightData			pointLightData[PointLightMax];	// �_�������
	SpotLightData			spotLightData[SpotLightMax];	// �X�|�b�g���C�g���
	int						pointLightCount = 0;			// �_������
	int						spotLightCount = 0;				// �X�|�b�g���C�g��

	// �F���␳���
	ColorGradingData		colorGradingData;

	// �K�E�X�t�B���^�[���
	GaussianFilterData		gaussianFilterData;

	// ���P�x���o�p���
	LuminanceExtractionData	luminanceExtractionData;

	// �ŏI�p�X���
	FinalpassData			finalpassData;

	// �V���h�E�}�b�v���
	ShadowMapData			shadowMapData;

	// �g�D�[���V�F�[�f�B���O
	ToonData				toonData;


};