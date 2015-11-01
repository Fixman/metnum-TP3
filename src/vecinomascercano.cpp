#include "vecinomascercano.h"

#include <cmath>

PixelTiempo VecinoMasCercano::alentarPixel(const PixelTiempo &P, int c0, int c1) const
{
	PixelTiempo r(c1);

	double scale = (c1 - 1) / (c0 - 1);

	for (int i = 0; i < r.size(); i++)
		r[i] = P[std::round(i / scale)];

	return r;
}
