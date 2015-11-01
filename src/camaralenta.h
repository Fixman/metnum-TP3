#ifndef CAMARALENTA_H
#define CAMARALENTA_H

#include <vector>
#include <cstdint>

class CamaraLenta
{
	protected:
	typedef uint8_t Pixel;
	typedef std::vector<Pixel> PixelTiempo;
	typedef std::vector <std::vector <PixelTiempo>> Pelicula;

	virtual PixelTiempo alentarPixel(const PixelTiempo&, int, int) const = 0;

	private:
	const Pelicula peli;
	const int c0;

	const int h, w;

	public:
	CamaraLenta(const Pelicula&, int);
	Pelicula alentar(int) const;
};

#endif
