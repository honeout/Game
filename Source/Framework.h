#pragma once

#include <windows.h>
#include "HighResolutionTimer.h"
#include "Graphics/Graphics.h"
#include "Input/input.h"

#include "SceneGame.h"

#include "CameraController.h"
#include "Audio/Audio.h"

class Framework
{
public:
	Framework(HWND hWnd);
	~Framework();

private:
	void Update(float elapsedTime/*Elapsed seconds from last frame*/);
	void Render(float elapsedTime/*Elapsed seconds from last frame*/);

	void CalculateFrameStats();

public:
	int Run();
	LRESULT CALLBACK HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	const HWND				hWnd;
	HighResolutionTimer		timer;
	Graphics				graphics;
	Input					input;
	SceneGame*				scene;
	CameraController		cameraController;
	Audio* audio; 
};

