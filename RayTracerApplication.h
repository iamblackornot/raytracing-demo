#pragma once

#include "Win32Application.h"
#include "Graphics.h"
#include "RenderEngine.h"

class RayTracerOnPaintEventHadler
{
public:
	RayTracerOnPaintEventHadler(RenderEngine& engine) : m_engine(engine) {}
	void operator()(HDC hdc);
private:
	std::shared_ptr<FrameBuffer> GenerateBackground(int width, int height);
	Gdiplus::Status DrawBuffer(Gdiplus::Graphics& graphics, FrameBuffer* buffer, int width, int height);

	RenderEngine& m_engine;
};

class RayTracerApplication : public Win32Application
{
public:
	RayTracerApplication(
		int width = 800,
		int height = 800);
private:
	RenderEngine m_engine;
};