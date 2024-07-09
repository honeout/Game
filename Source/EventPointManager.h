#pragma once

#include <string>
#include <vector>

#include "EventData.h"

// �C�x���g�|�C���g�Ǘ���
class EventPointManager
{
private:
	EventPointManager() {}
	~EventPointManager() {}

public:
	// �B��̃C���X�^���X�擾
	static EventPointManager& Instance()
	{
		static EventPointManager instance;
		return instance;
	}

	// �ǂݍ���
	void	LoadPoints(const char* filepath);

	// �N���A
	void	Clear();

	// �|�C���g�͈̓`�F�b�N
	void	CheckPoint(const DirectX::XMFLOAT3& position, float radius);

	// �|�C���g�͈͕`��
	void	DrawDebugPrimitive();

	inline	std::vector<EventData>& GetPoints() { return points; }

private:
	std::vector<EventData> points;
};
