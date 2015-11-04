#ifndef SPLINES_H
#define SPLINES_H

#include "camaralenta.h"

class Splines : public CamaraLenta
{
	using CamaraLenta::CamaraLenta;
	PixelTiempo alentarPixel(const PixelTiempo&, int, int) const;
};

#endif
