#pragma once
#include <optional>
#include "geometry.h"
#include <math.h>

typedef std::optional<double> TraceResult;

class RayTracer
{
public:
	typedef std::optional<double> TraceResult;

	TraceResult
	Trace(
		geometry::Vec3 observerPos,
		geometry::Vec3 direction,
		double minValue,
		double maxValue,
		geometry::Sphere const* sphere) const;
};