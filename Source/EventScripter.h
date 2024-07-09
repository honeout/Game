#pragma once

#include <string>
#include <vector>
#include <DirectXMath.h>

// �C�x���g�X�N���v�g���s��
// �ǂ��T���v���Ƃ��Ă͋g���g���Ƃ������Ǝv��(�g�������ƂȂ����ǁc�c)
class EventScripter
{
	// ���߃^�C�v
	enum class CommandType
	{
		WINDOW_MESSAGE = 0,		// �E�B���h�E���b�Z�[�W�\��
		INPUT_KEY_WAIT,			// �L�[���͑ҋ@
		TIME_WAIT,				// ���ԑҋ@

		DISPLAY_TALKPICTURE,	// �����G�\��
		HIDE_TALKPICTURE,		// �����G��\��
		LOAD_TALKPICTURE,		// �����G�ǂݍ���
		DISPOSE_TALKPICTURE,	// �����G�j��

		PLAYER_SETPOSITION,		// �v���C���[�̈ʒu�ݒ�
		PLAYER_MOVE,			// �v���C���[�̈ړ�
		PLAYER_MOTION,			// �v���C���[�̃��[�V����

		CAMERA_SETPOSITION,		// �v���C���[�̈ʒu�ݒ�
		CAMERA_SETTARGET,		// �v���C���[�̃^�[�Q�b�g�ʒu�ݒ�
	};

	// ���߃f�[�^
	struct CommandData
	{
		CommandType type; // ���߃^�C�v
		union Data
		{
			// ���b�Z�[�W
			struct
			{
				char text[1024];
			} message;

			// �o�ߎ��ԑҋ@
			struct
			{
				float second;
			} time_wait;

			///////////////////////////////////////////
			// �����G�\��
			struct
			{
				int		id;
				int		displayPictureId;
				int		position;
				bool	xflip;
			} displayTalkPicture;
			// �����G��\��
			struct
			{
				int		id;
			} hideTalkPicture;
			// �����G�ǂݍ���
			struct
			{
				int		id;
				char	filepath[1024];
			} loadTalkPicture;
			// �����G�j��
			struct
			{
				int			id;
			} disposeTalkPicture;

			///////////////////////////////////////////
			// �v���C���[�̈ʒu�ݒ�
			struct
			{
				DirectX::XMFLOAT3 position;
			} playerSetPosition;
			// �v���C���[�̈ړ�
			struct
			{
				DirectX::XMFLOAT3 moveVec;
				float moveSpeed;
				float turnSpeed;
			} playerMove;
			// �v���C���[�̃��[�V����
			struct
			{
				int index;
				bool loop;
				float blendSecond;
			} playerMotion;

			///////////////////////////////////////////
			// �J�����̈ʒu�ݒ�
			struct
			{
				DirectX::XMFLOAT3 position;
			} cameraSetPosition;
			// �J�����̃^�[�Q�b�g�ݒ�
			struct
			{
				DirectX::XMFLOAT3 position;
			} cameraTarget;
		} data;
	};

private:
	EventScripter() {}
	~EventScripter() {}

public:
	// �B��̃C���X�^���X�擾
	static EventScripter& Instance()
	{
		static EventScripter instance;
		return instance;
	}

	void Initialize();

	void Finalize();

	void Update(float elapsedTime);

	void LoadScript(const char* filePath);

private:
	void OnEventMode(void* data);

private:
	std::vector<CommandData>	commands;
	int							commandIndex;
	float						timeWaitTimer = 0;

	// ���b�Z���W���[���ʎq
	uint64_t	EVENTMODEINDEX = -1;
};

