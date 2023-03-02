#include "RayTracer.h"

TraceResult RayTracer::Trace(
	geometry::Vec3 observerPos, 
	geometry::Vec3 direction,
	double minValue,
	double maxValue,
	geometry::Sphere const* sphere) const
{
	using namespace geometry;

	Vec3 OV = direction;
	Vec3 CO = observerPos - sphere->position;

	double a = DotProduct(OV, OV);
	double b = 2 * DotProduct(CO, OV);
	double c = DotProduct(CO, CO) - sphere->radius * sphere->radius;

	double d = b * b - 4 * a * c;

	if (d < 0)
	{
		return std::nullopt;
	}

	double k1 = (-b - sqrt(d)) / (2 * a);
	double k2 = (-b + sqrt(d)) / (2 * a);

	TraceResult res;

	if (k1 >= minValue && k1 <= maxValue)
	{
		res = k1;
	}
	if (k2 >= minValue && k2 <= maxValue)
	{
		if (!res)
		{
			res = k2;
		}
		else
		{
			res = k1 < k2 ? res : k2;
		}
	}

	return res;
}
