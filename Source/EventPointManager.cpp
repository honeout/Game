#include "EventPointManager.h"

#include <fstream>
#include <iostream>
#include <algorithm>

#include "Misc.h"
#include "Graphics/Graphics.h"
#include "Collision.h"
#include "Messenger.h"
#include "MessageData.h"

void EventPointManager::LoadPoints(const char* filepath)
{
	// �X�N���v�g�t�@�C�����[�h
	// ���̕ӂ̓X�e�[�W�E�V�[�����ɕς���Ƃ�낵��
	// �X�^���[���͂���ς肢����(�L�E�ցE�M)
	std::ifstream ifs(filepath);
	assert(!ifs.fail());
	if (ifs.fail())
		return;

	// �\�����
	std::string str;
	while (getline(ifs, str))
	{
		if (str.size() == 0)
			continue;

		// ������𕪊����č\�����
		// ���߂͑S��1�s�ōs���A�p�����[�^�[�̕����͑S��" "�ōs��
		// �p�����[�^�[�̊�{�I�ȕ��т͈ȉ��̒ʂ�
		// 0 = �\����, 1�` = �f�[�^
		std::vector<std::string> string_array = string_split(str, ' ');
		if (string_array.size() == 0)
			continue;

		if (string_array[0] == "EVENT_POINT")
		{
			points.emplace_back();
			EventData& data = points.back();

			data.identifier = string_array[1];
			data.position.x = static_cast<float>(std::atof(string_array[2].data()));
			data.position.y = static_cast<float>(std::atof(string_array[3].data()));
			data.position.z = static_cast<float>(std::atof(string_array[4].data()));
			data.radius = static_cast<float>(std::atof(string_array[5].data()));
			data.callScript = string_array[6];
		}
	}
}

void EventPointManager::Clear()
{
	points.clear();
}

void EventPointManager::CheckPoint(const DirectX::XMFLOAT3& position, float radius)
{
	// �C�x���g�͈͕`��
	DirectX::XMFLOAT3	dummy;
	for (auto& it : points)
	{
		if (it.isStarted)
			continue;

		if (Collision::IntersectSphereVsSphere(it.position, it.radius, position, radius, dummy, dummy))
		{
			it.isStarted = true;
			MessageData::EVENTMODEDATA	data;
			data.filepath = it.callScript;
			Messenger::Instance().SendData(MessageData::EVENTMODEEVENT, &data);
			break;
		}
	}
}

// �f�o�b�O�v���~�e�B�u�`��
void EventPointManager::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	// �C�x���g�͈͕`��
	for (auto& it : points)
	{
		debugRenderer->DrawSphere(it.position, it.radius, DirectX::XMFLOAT4(0, 0, 0, 1));
	}
}
