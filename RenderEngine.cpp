#include "RenderEngine.h"

RenderEngine::RenderEngine(int viewWidth, int viewHeight) : m_viewport(viewWidth, viewHeight)
{

}

void RenderEngine::AddSphere(
	geometry::Vec3 const& position,
	double radius,
	PixelARGB color,
	std::optional<double> specularLevel)
{
	m_sceneObjects.push_back(std::make_shared<geometry::Sphere>(
		position,
		radius,
		color,
		std::move(specularLevel)));
}

void RenderEngine::SetViewportResolution(int pxWidth, int pxHeight)
{
	m_viewport.SetResolution(pxWidth, pxHeight);
}

void RenderEngine::SetAmbientLightLevel(double value)
{
	m_shader.SetAmbientLightLevel(value);
}

void RenderEngine::AddDirectionalLight(double level, geometry::Vec3 direction)
{
	m_shader.AddDirectionalLight(level, direction);
}

void RenderEngine::AddPointLight(double level, geometry::Vec3 position)
{
	m_shader.AddPointLight(level, position);
}

std::shared_ptr<FrameBuffer> RenderEngine::Render() const
{
	int pixelSize = sizeof(PixelARGB);

	FrameBuffer* buffer = new FrameBuffer[(long)m_viewport.pxWidth * m_viewport.pxHeight * pixelSize];

	for (int y = 0; y < m_viewport.pxHeight; ++y)
	{
		for (int x = 0; x < m_viewport.pxWidth; ++x)
		{
			PixelARGB pixelColor = m_backgroundColor;

			auto traceResult = TraceClosestToViewport(x, y);

			if (traceResult)
			{
				ShaderOptions shaderOptions(
					traceResult->hitPoint,
					m_viewport.observerPosition,
					traceResult->object,
					m_sceneObjects);

				pixelColor = m_shader.Apply(shaderOptions);
			}

			new (buffer + ((long)y * m_viewport.pxWidth + x) * pixelSize) PixelARGB(pixelColor);
		}
	}

	return std::shared_ptr<FrameBuffer>(buffer);
}

std::optional<RenderEngine::TraceResult> RenderEngine::TraceClosestToViewport(int viewportX, int viewportY) const
{
	using namespace geometry;

	Vec3 viewportPoint = m_viewport.ViewportToWorld(viewportX, viewportY);

	auto rayTracerCallable = std::bind(
		&RayTracer::Trace,
		&m_rayTracer,
		m_viewport.observerPosition,
		viewportPoint - m_viewport.observerPosition,
		MIN_TRACE_CLOSEST_T_VALUE,
		MAX_TRACE_CLOSEST_T_VALUE,
		std::placeholders::_1);

	SingleDispatcher<
		RayTracer::TraceResult,
		decltype(rayTracerCallable),
		Geometry,
		GEOMETRY_SET> dispatcher(rayTracerCallable);

	bool gotHit = false;
	double closestPointK = DOUBLE_MAX_VALUE;
	std::shared_ptr<Geometry> closestObject;

	for (std::shared_ptr<Geometry> const& obj : m_sceneObjects)
	{
		RayTracer::TraceResult result = dispatcher.Dispatch(obj.get());

		if (!result)
		{
			continue;
		}

		gotHit = true;

		if (result.value() < closestPointK)
		{
			closestPointK = result.value();
			closestObject = obj;
		}
	}

	if (!gotHit)
	{
		return std::nullopt;
	}

	Vec3 closestPoint =
		m_viewport.observerPosition +
		closestPointK * (viewportPoint - m_viewport.observerPosition);

	return TraceResult{ closestObject, closestPoint };
}