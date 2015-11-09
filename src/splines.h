#ifndef SPLINES_H
#define SPLINES_H

#include "camaralenta.h"

typedef std::vector <double> Vector;

class Splines : public CamaraLenta
{
	struct Polynomial
	{
		double a, b, c, d;
	};

	Pixel toPixel(double) const;

	Vector tridiagonalSubstitution(const Vector&, const Vector&, Vector, Vector) const;
	std::vector <Polynomial> buildSpline(const Vector &, double) const;

	PixelTiempo alentarPixel(const PixelTiempo&, int, int) const;

	const int reset;

	public:
	Splines(Pelicula peli, int c0, int f, int reset = 4);
};

#endif
