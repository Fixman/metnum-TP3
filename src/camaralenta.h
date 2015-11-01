#ifndef CAMARALENTA_H
#define CAMARALENTA_H

#include <vector>
#include <cstdint>

class CamaraLenta
{
	typedef uint8_t Pixel;
	typedef std::vector<Pixel> PixelTiempo;
	typedef std::vector <std::vector <PixelTiempo>> Pelicula;

	const Pelicula peli;
	const int c0;

	const int h, w;

	virtual PixelTiempo alentarPixel(PixelTiempo) const = 0;

	public:
	CamaraLenta(const Pelicula&, int);
	Pelicula alentar(int) const;
};

#endif
