#include "splines.h"

double sq(double n) { return n * n ; }
double cb(double n) { return n * n * n ; }

PixelTiempo Splines::alentarPixel(const PixelTiempo &P, int c0, int c1) const
{
	PixelTiempo r(c1);

	double q = (c1 - 1) / (c0 - 1);

	double fp = 0;
	double fs = 0;
	for (int i = 0; i < P.size() - 1; i++)
	{
		double x0 = q * i;
		double y0 = P[i];
		double y1 = P[i + 1];

		double a = 1 / 
	}
}
