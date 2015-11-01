#include "camaralenta.h"

CamaraLenta::CamaraLenta(Pelicula peli, int c0, int f)
: peli(peli), c0(c0), h(peli.size()), w(peli[0].size()), f(f)
{
}

Pelicula CamaraLenta::alentar(int dc) const
{
	Pelicula r(h, std::vector <PixelTiempo> (w));

	int c1 = c0 * (dc + 1);
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
			r[i][j] = alentarPixel(peli[i][j], c0, c1);
	}

	return r;
}
