#ifndef SPLINES_H
#define SPLINES_H

#include "camaralenta.h"

class Splines : public CamaraLenta
{
	struct Polynomial
	{
		double a, b, c, d;
	};

	PixelTiempo alentarPixel(const PixelTiempo&, int, int) const;

	int reset;

	public:
	Splines(Pelicula peli, int c0, int f, int reset = 4);
};

#endif
