#ifndef SPLINES_H
#define SPLINES_H

#include "camaralenta.h"

class Splines : public CamaraLenta
{
	int reset;
	PixelTiempo alentarPixel(const PixelTiempo&, int, int) const;

	public:
	Splines(Pelicula peli, int c0, int f, int reset = 4);
};

#endif
