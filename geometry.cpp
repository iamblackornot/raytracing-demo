#include "geometry.h"

namespace geometry
{
	Vec3 operator* (double k, Vec3 const& rhs)
	{
		return { k * rhs.x, k * rhs.y, k * rhs.z };
	}

	Vec3 operator/ (Vec3 const& rhs, double k)
	{
		return { rhs.x / k , rhs.y / k, rhs.z / k };
	}

	double DotProduct(Vec3 const& lhs, Vec3 const& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}

	Vec3 Vec3::operator+ (Vec3 const& rhs) const
	{
		return { x + rhs.x, y + rhs.y, z + rhs.z };
	}

	Vec3 Vec3::operator- (Vec3 const& rhs) const
	{
		return { x - rhs.x, y - rhs.y, z - rhs.z };
	}

	double Vec3::Length() const
	{
		return sqrt(DotProduct(*this, *this));
	}
	Vec3 Vec3::GetNormalized() const
	{
		return *this / Length();
	}

	Geometry::Geometry(
		Vec3 position, 
		PixelARGB color, 
		std::optional<double> specularLevel)
		: 
		position(position), 
		color(color), 
		specularLevel(std::move(specularLevel)) {}

	Sphere::Sphere(
		Vec3 position, 
		double radius, 
		PixelARGB color, 
		std::optional<double> specularLevel)
		: 
		Geometry(
			position, 
			color, 
			std::move(specularLevel)),
		radius(radius) {}

	Vec3 Sphere::GetNormalVector(Vec3 const& point) const
	{
		return (point - position).GetNormalized();
	}
}