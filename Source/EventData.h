#pragma once

#include <string>
#include <DirectXMath.h>

// �C�x���g�f�[�^
struct EventData
{
	bool				isStarted = false;
	std::string			identifier;
	DirectX::XMFLOAT3	position;
	float				radius;
	std::string			callScript;
};
