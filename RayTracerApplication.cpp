#include "RayTracerApplication.h"

void RayTracerOnPaintEventHadler::operator()(HDC hdc)
{
	RECT rect;
	GetClientRect(WindowFromDC(hdc), &rect);

	Gdiplus::Graphics graphics(hdc);

	int width = rect.right;
	int height = rect.bottom;

	DrawBuffer(graphics, GenerateBackground(width, height).get(), width, height);

	m_engine.SetViewportResolution(width, height);
	DrawBuffer(graphics, m_engine.Render().get(), width, height);
}
Gdiplus::Status RayTracerOnPaintEventHadler::DrawBuffer(Gdiplus::Graphics& graphics, FrameBuffer* buffer, int width, int height)
{
	Gdiplus::Bitmap bitmap(width, height, width * sizeof(PixelARGB), PixelFormat32bppARGB, buffer);

	//CLSID pngClsid;
	//CLSIDFromString(L"{557CF406-1A04-11D3-9A73-0000F81EF32E}", &pngClsid);
	//bitmap.Save(L"bitmap.png", &pngClsid, NULL);

	return graphics.DrawImage(&bitmap, Gdiplus::Rect(0, 0, width, height));
}

std::shared_ptr<FrameBuffer> RayTracerOnPaintEventHadler::GenerateBackground(int width, int height)
{
	int pixelsCount = width * height;

	FrameBuffer* buffer = new FrameBuffer[pixelsCount * sizeof(PixelARGB)];

	for (int i = 0; i < pixelsCount; ++i)
	{
		PixelARGB* allocated = new (buffer + i * sizeof(PixelARGB)) PixelARGB();

		allocated->Alpha = 255;

		int scale = 64;
		int col = i % width / scale;
		int row = i / width / scale;

		allocated->Blue = ((col + row) % 2 == 0 ? 100 : 225);
	}

	return std::shared_ptr<FrameBuffer>(buffer);
}

RayTracerApplication::RayTracerApplication(int width, int height)
	: m_engine(width, height), Win32Application(width, height, RayTracerOnPaintEventHadler(m_engine))
{
	m_engine.AddSphere({ 0, -1, 3 }, 1, PixelARGB{ .Red = 255, .Alpha = 255 }, 500);
	m_engine.AddSphere({ 2, 0, 4 }, 1, PixelARGB{ .Blue = 255, .Alpha = 255 }, 500);
	m_engine.AddSphere({ -2, 0, 4 }, 1, PixelARGB{ .Green = 255, .Alpha = 255 }, 10);
	m_engine.AddSphere({ 0, -5001, 0 }, 5000, PixelARGB{ .Green = 255, .Red = 255, .Alpha = 255 }, 1000);

	m_engine.SetAmbientLightLevel(0.2);
	m_engine.AddPointLight(0.6, { 2, 1, 0 });
	m_engine.AddDirectionalLight(0.2, { 1, 4, 4 });

	//m_engine.AddSphere({ 2, 1, 0 }, 0.1, PixelARGB{ .Alpha = 255 });

	//m_engine.AddPointLight(0.6, { 0, 0, -2 });
	//m_engine.AddDirectionalLight(0.6, { 0, 16, 16 });
};