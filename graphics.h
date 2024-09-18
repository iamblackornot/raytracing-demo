#pragma once

#include "windows.h"
#include <cmath>
#include <limits>

#define BYTE_MAX_VALUE (std::numeric_limits<BYTE>::max)()

struct PixelARGB
{
	BYTE Blue;
	BYTE Green;
	BYTE Red;
	BYTE Alpha;

	PixelARGB operator* (double multiplier)
	{
		if (multiplier < 0)
		{
			return *this;
		}

		double dBlue = std::round(this->Blue * multiplier);
		BYTE blue = dBlue > BYTE_MAX_VALUE
			? BYTE_MAX_VALUE
			: static_cast<BYTE>(dBlue);

		double dGreen = std::round(this->Green * multiplier);
		BYTE green = dGreen > BYTE_MAX_VALUE
			? BYTE_MAX_VALUE
			: static_cast<BYTE>(dGreen);

		double dRed = std::round(this->Red * multiplier);
		BYTE red = dRed > BYTE_MAX_VALUE
			? BYTE_MAX_VALUE
			: static_cast<BYTE>(dRed);


		return { blue, green, red, this->Alpha };
	}
};