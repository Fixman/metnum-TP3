#ifndef VECINOMASCERCANO_H
#define VECINOMASCERCANO_H

#include "camaralenta.h"

class VecinoMasCercano : public CamaraLenta
{
	using CamaraLenta::CamaraLenta;

	PixelTiempo alentarPixel(const PixelTiempo&, int, int) const;
};

#endif
