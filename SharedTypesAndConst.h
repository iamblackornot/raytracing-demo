#pragma once

#include <list>
#include <memory>
#include <limits>

#include "geometry.h"

#define DOUBLE_MAX_VALUE (std::numeric_limits<double>::max)();

typedef std::list<std::shared_ptr<geometry::Geometry>> SceneObjects;
