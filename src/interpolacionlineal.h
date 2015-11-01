#ifndef INTERPOLACIONLINEAL_H
#define INTERPOLACIONLINEAL_H

#include "camaralenta.h"

class InterpolacionLineal : public CamaraLenta
{
	using CamaraLenta::CamaraLenta;
	PixelTiempo alentarPixel(const PixelTiempo&, int, int) const;
};

#endif
