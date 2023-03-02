#pragma once
#include "geometry.h"
#include "SharedTypesAndConst.h"
#include "RayTracer.h"
#include <vector>
#include <optional>

struct ShaderOptions;

class Shader
{
	struct LightObject
	{
		double level;
		geometry::Vec3 positionOrDirection;
	};

	double m_ambientLightLevel;
	std::vector<LightObject> m_directionalLights;
	std::vector<LightObject> m_pointLights;

	RayTracer m_rayTracer;

	const double MIN_TRACE_SHADOW_VALUE = 0.0001;
	const double MAX_TRACE_SHADOW_FROM_POINT_LIGHT_VALUE = 1;
	const double MAX_TRACE_SHADOW_FROM_DIR_LIGHT_VALUE = DOUBLE_MAX_VALUE;

public:
	Shader();

	void SetAmbientLightLevel(double value);
	void AddDirectionalLight(double level, geometry::Vec3 direction);
	void AddPointLight(double level, geometry::Vec3 position);

	PixelARGB Apply(ShaderOptions options) const;

private:
	double ComputeLightLevel(
		double lightLevel,
		geometry::Vec3 const& pointPosition,
		geometry::Vec3 const& normalVector,
		geometry::Vec3 const& reflectionVector,
		std::optional<double> specularLevel) const;

	bool TraceAny(
		geometry::Vec3 const& startPosition,
		geometry::Vec3 const& direction,
		double maxTraceValue,
		SceneObjects objects) const;
};

struct ShaderOptions
{
	geometry::Vec3 const& hitPoint;
	geometry::Vec3 const& observerPosition;
	std::shared_ptr<geometry::Geometry> hitObject;
	SceneObjects objects;

	ShaderOptions(
		geometry::Vec3 const& hitPoint,
		geometry::Vec3 const& observerPosition,
		std::shared_ptr<geometry::Geometry> hitObject,
		SceneObjects objects);
};