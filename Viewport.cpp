#include "Viewport.h"

Viewport::Viewport(
	double unitWidth, 
	double unitHeight, 
	geometry::Vec3 viewPosition,
	geometry::Vec3 observerPosition,
	int pxWidth,
	int pxHeight)
	:
	m_unitWidth(unitWidth), 
	m_unitHeight(unitHeight), 
	viewPosition(viewPosition),
	observerPosition(observerPosition),
	pxWidth(pxWidth),
	pxHeight(pxHeight)
{

}

void Viewport::SetResolution(int pxWidth, int pxHeight)
{
	this->pxWidth = pxWidth;
	this->pxHeight = pxHeight;
}

StaticViewport::StaticViewport(int pxWidth, int pxHeight)
	: Viewport(
		1., 1.,
		geometry::Vec3{ 0, 0, 1 },
		geometry::Vec3{ 0, 0, 0 },
		pxWidth, pxHeight)
{

}

geometry::Vec3 StaticViewport::ViewportToWorld(int x, int y) const
{
	return {
		viewPosition.x - m_unitWidth / 2. + x * m_unitWidth / pxWidth,
		viewPosition.y + m_unitHeight / 2. - y * m_unitHeight / pxHeight,
		1
	};
}
