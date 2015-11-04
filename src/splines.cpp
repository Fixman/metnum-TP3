#include "splines.h"

#include <cmath>

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
		double x1 = x0 + q;
		double y0 = P[i];
		double y1 = P[i + 1];

		double a = 1 / std::pow(q, 3) * (y1 - y0 - q * fp - 1. / 2 * std::pow(q, 2)  * fs);
		double b = 1. / 2 * (fs - 6 * a * x0);
		double c = fp - 2 * x0 * b - 3 * std::pow(x0, 2) * a;
		double d = y0 - c * c0 - b * std::pow(x0, 2) - a * std::pow(x0, 3);

		for (int x = x0; x <= x1; x++)
			r[x] = a * std::pow(x, 3) + b * std::pow(x, 2) + c * x + d;

		fp = 3 * a * std::pow(x1, 2) + 2 * b * x1 + c;
		fs = 6 * a * x1 + 2 * b;
	}

	return r;
}
