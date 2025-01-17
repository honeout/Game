#pragma once

#include <DirectXMath.h>
#include <vector>
#include <string>

// 立ち絵位置
enum class TalkPicturePosition
{
	Left,
	Center,
	Right,
};

namespace	MessageData
{
	// イベントモードに移動
	static	constexpr	char* EVENTMODEEVENT = "EVENT MODE EVENT";
	struct EVENTMODEDATA
	{
		std::string	filepath;
	};

	// ゲームモードに移動
	static	constexpr	char* GAMEMODEEVENT = "GAME MODE EVENT";
	struct GAMEMODEDATA
	{
		int dummy;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// メッセージウィンドウ情報
	static	constexpr	char* MESSAGEEVENT = "MESSAGE EVENT";
	struct MESSAGEEVENTDATA
	{
		std::string message;	// メッセージ
	};

	// 立ち絵表示情報
	static	constexpr	char* DISPLAYTALKPICTUREEVENT = "DISPLAY TALK PICTURE EVENT";
	struct DISPLAYTALKPICTUREDATA
	{
		int						id;
		int						displayPictureId;
		TalkPicturePosition		position;
		bool					xflip;
	};

	// 立ち絵非表示情報
	static	constexpr	char* HIDETALKPICTUREEVENT = "HIDE TALK PICTURE EVENT";
	struct HIDETALKPICTUREDATA
	{
		int	id;
	};

	// 立ち絵読み込み情報
	static	constexpr	char* LOADTALKPICTUREEVENT = "LOAD TALK PICTURE EVENT";
	struct LOADTALKPICTUREDATA
	{
		int			id;
		std::string	filepath;
	};

	// 立ち絵破棄情報
	static	constexpr	char* DISPOSETALKPICTUREEVENT = "DISPOSE TALK PICTURE EVENT";
	struct DISPOSETALKPICTUREDATA
	{
		int			id;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// プレイヤーの位置設定
	static	constexpr	char* PLAYERSETPOSITIONEVENT = "PLAYER SETPOSITION EVENT";
	struct PLAYERSETPOSITIONDATA
	{
		DirectX::XMFLOAT3 position;
	};

	// プレイヤーの移動
	static	constexpr	char* PLAYERMOVEEVENT = "PLAYER MOVE EVENT";
	struct PLAYERMOVEDATA
	{
		DirectX::XMFLOAT3 moveVec;
		float moveSpeed;
		float turnSpeed;
	};

	// プレイヤーの移動
	static	constexpr	char* PLAYERMOTIONEVENT = "PLAYER MOTION EVENT";
	struct PLAYERMOTIONDATA
	{
		int index;
		bool loop;
		float blendSecond = 0.2f;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// カメラの位置設定
	static	constexpr	char* CAMERASETPOSITIONEVENT = "CAMERA SETPOSITION EVENT";
	struct CAMERASETPOSITIONDATA
	{
		DirectX::XMFLOAT3 position;
	};

	// カメラのターゲット位置の設定
	static	constexpr	char* CAMERASETTARGETEVENT = "CAMERA TARGET EVENT";
	struct CAMERASETTARGETDATA
	{
		DirectX::XMFLOAT3 position;
	};

};
