#include "vecinomascercano.h"

PixelTiempo VecinoMasCercano::alentarPixel(const PixelTiempo &P, int c0, int c1) const
{
	PixelTiempo r;

	for (int i = 0; i < r.size(); i++)
		r[i] = P[c0 * i / c1];

	return r;
}
