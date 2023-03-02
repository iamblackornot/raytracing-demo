#pragma once
#include "geometry.h"

class Viewport
{
public:
	Viewport(
		double unitWidth,
		double unitHeight,
		geometry::Vec3 viewPosition,
		geometry::Vec3 observerPosition,
		int pxWidth,
		int pxHeight);

	void SetResolution(int pxWidth, int pxHeight);

	geometry::Vec3 viewPosition;
	geometry::Vec3 observerPosition;
	int pxWidth;
	int pxHeight;
protected:
	double m_unitWidth;
	double m_unitHeight;
};

class StaticViewport : public Viewport
{
public:
	StaticViewport(int pxWidth, int pxHeight);
	geometry::Vec3 ViewportToWorld(int x, int y) const;
};