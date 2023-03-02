#pragma once
#include "graphics.h"

#include <math.h>
#include <optional>

#define GEOMETRY_SET Sphere

namespace geometry
{
	struct Vec3;

	double DotProduct(Vec3 const& lhs, Vec3 const& rhs);
	Vec3 operator* (double k, Vec3 const& rhs);
	Vec3 operator/ (Vec3 const& rhs, double k);

	struct Vec3
	{
		double x;
		double y;
		double z;

		Vec3 operator+ (Vec3 const& rhs) const;
		Vec3 operator- (Vec3 const& rhs) const;

		double Length() const;
		Vec3 GetNormalized() const;
	};

	struct Geometry
	{
		Vec3 position;
		PixelARGB color;
		std::optional<double> specularLevel;

		virtual Vec3 GetNormalVector(Vec3 const& point) const = 0;
	protected:
		Geometry(
			Vec3 position, 
			PixelARGB color, 
			std::optional<double> specularLevel);
	};
	struct Sphere : public Geometry
	{
		double radius;

		Sphere(
			Vec3 position, 
			double radius, 
			PixelARGB color,
			std::optional<double> specularLevel);

		Vec3 GetNormalVector(Vec3 const& point) const override;
	};
}