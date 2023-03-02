#pragma once


#include <functional>
#include <limits>

#include "geometry.h"
#include "RayTracer.h"
#include "Viewport.h"
#include "SingleDispatcher.h"
#include "Shader.h"
#include "SharedTypesAndConst.h"

typedef unsigned char FrameBuffer;

class RenderEngine
{
	SceneObjects m_sceneObjects;
	RayTracer m_rayTracer;
	PixelARGB m_backgroundColor = PixelARGB{ .Blue = 0, .Green = 0, .Red = 0, .Alpha = 0 };
	StaticViewport m_viewport;
	Shader m_shader;

	const double MIN_TRACE_CLOSEST_T_VALUE = 1;
	const double MAX_TRACE_CLOSEST_T_VALUE = DOUBLE_MAX_VALUE;

	struct TraceResult
	{
		std::shared_ptr<geometry::Geometry> object;
		geometry::Vec3 hitPoint;
	};
public:
	RenderEngine(int viewWidth, int viewHeight);

	void AddSphere(
		geometry::Vec3 const& position, 
		double radius, 
		PixelARGB color,
		std::optional<double> specularLevel = std::nullopt);

	void SetViewportResolution(int pxWidth, int pxHeight);

	void SetAmbientLightLevel(double value);
	void AddDirectionalLight(double level, geometry::Vec3 direction);
	void AddPointLight(double level, geometry::Vec3 position);

	std::shared_ptr<FrameBuffer> Render() const;

private:
	std::optional<TraceResult> TraceClosestToViewport(int viewportX, int viewportY) const;
};