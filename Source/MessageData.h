#pragma once

#include <DirectXMath.h>
#include <vector>
#include <string>

// �����G�ʒu
enum class TalkPicturePosition
{
	Left,
	Center,
	Right,
};

namespace	MessageData
{
	// �C�x���g���[�h�Ɉړ�
	static	constexpr	char* EVENTMODEEVENT = "EVENT MODE EVENT";
	struct EVENTMODEDATA
	{
		std::string	filepath;
	};

	// �Q�[�����[�h�Ɉړ�
	static	constexpr	char* GAMEMODEEVENT = "GAME MODE EVENT";
	struct GAMEMODEDATA
	{
		int dummy;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// ���b�Z�[�W�E�B���h�E���
	static	constexpr	char* MESSAGEEVENT = "MESSAGE EVENT";
	struct MESSAGEEVENTDATA
	{
		std::string message;	// ���b�Z�[�W
	};

	// �����G�\�����
	static	constexpr	char* DISPLAYTALKPICTUREEVENT = "DISPLAY TALK PICTURE EVENT";
	struct DISPLAYTALKPICTUREDATA
	{
		int						id;
		int						displayPictureId;
		TalkPicturePosition		position;
		bool					xflip;
	};

	// �����G��\�����
	static	constexpr	char* HIDETALKPICTUREEVENT = "HIDE TALK PICTURE EVENT";
	struct HIDETALKPICTUREDATA
	{
		int	id;
	};

	// �����G�ǂݍ��ݏ��
	static	constexpr	char* LOADTALKPICTUREEVENT = "LOAD TALK PICTURE EVENT";
	struct LOADTALKPICTUREDATA
	{
		int			id;
		std::string	filepath;
	};

	// �����G�j�����
	static	constexpr	char* DISPOSETALKPICTUREEVENT = "DISPOSE TALK PICTURE EVENT";
	struct DISPOSETALKPICTUREDATA
	{
		int			id;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// �v���C���[�̈ʒu�ݒ�
	static	constexpr	char* PLAYERSETPOSITIONEVENT = "PLAYER SETPOSITION EVENT";
	struct PLAYERSETPOSITIONDATA
	{
		DirectX::XMFLOAT3 position;
	};

	// �v���C���[�̈ړ�
	static	constexpr	char* PLAYERMOVEEVENT = "PLAYER MOVE EVENT";
	struct PLAYERMOVEDATA
	{
		DirectX::XMFLOAT3 moveVec;
		float moveSpeed;
		float turnSpeed;
	};

	// �v���C���[�̈ړ�
	static	constexpr	char* PLAYERMOTIONEVENT = "PLAYER MOTION EVENT";
	struct PLAYERMOTIONDATA
	{
		int index;
		bool loop;
		float blendSecond = 0.2f;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// �J�����̈ʒu�ݒ�
	static	constexpr	char* CAMERASETPOSITIONEVENT = "CAMERA SETPOSITION EVENT";
	struct CAMERASETPOSITIONDATA
	{
		DirectX::XMFLOAT3 position;
	};

	// �J�����̃^�[�Q�b�g�ʒu�̐ݒ�
	static	constexpr	char* CAMERASETTARGETEVENT = "CAMERA TARGET EVENT";
	struct CAMERASETTARGETDATA
	{
		DirectX::XMFLOAT3 position;
	};

};
