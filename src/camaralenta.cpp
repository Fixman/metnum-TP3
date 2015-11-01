#include "camaralenta.h"

CamaraLenta::CamaraLenta(const Pelicula &peli, int c0)
: peli(peli), c0(c0), h(peli.size()), w(peli.size())
{
}

Pelicula CamaraLenta::alentar(int dc) const
{
	Pelicula r(h, std::vector <PixelTiempo> (w));

	int c1 = c0 * dc;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
			r[i][j] = alentarPixel(peli[i][j], c0, c1);
	}

	return r;
}
