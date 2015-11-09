#include "splines.h"

#include <cmath>

Splines::Splines(Pelicula peli, int c0, int f, int reset)
: CamaraLenta(peli, c0, f), reset(reset)
{
}

std::vector <double> tridiagonalSubstitution(std::vector <double> a, std::vector <double> b, std::vector <double> c, std::vector <double> f)
{
	c[0] /= b[0];
	f[0] /= b[0];

	for (int i = 1; i < f.size(); i++)
	{
		double m = 1. / (b[i] - a[i - 1] * c[i - 1]);
		f[i] = (f[i] - a[i - 1] * f[i - 1]) * m;

		if (i < f.size() - 1)
			c[i] *= m;
	}

	for (int i = f.size() - 2; i >= 0; i--)
		f[i] -= c[i] * f[i + 1];

	return f;
}

uint8_t topixel(double n)
{
	return std::max(0, std::min(255, (int) std::round(n)));
}

PixelTiempo Splines::alentarPixel(const PixelTiempo &P, int c0, int c1) const
{
	int n = P.size();
	double q = (c1 - 1) / (c0 - 1);

	const std::vector <double> y(P.begin(), P.end());

	// j[1..n) -> subdiagonal
	// k[0..n) -> diagonal
	// l[0..n-1) -> superdiagonal
	std::vector <double> j(n - 1, q), k(n, 4 * q), l(n - 1, q);
	j[n - 2] = l[0] = 0;
	k[0] = k[n - 1] = 1;

	std::vector <double> f(n);
	f[0] = f[n - 1] = 0;
	for (int i = 1; i < n - 1; i++)
		f[i] = (3 / q) * (y[i - 1] - 2 * y[i] + y[i + 1]);
	
	std::vector <double> a(y.begin(), y.end());
	std::vector <double> b(n, 0);
	std::vector <double> c = tridiagonalSubstitution(j, k, l, f);
	std::vector <double> d(n, 0);

	for (int i = 0; i < n - 1; i++)
	{
		b[i] = (y[i + 1] - y[i]) / q - (q / 3) * (2 * c[i] + c[i + 1]);
		d[i] = (c[i + 1] - c[i]) / (3 * q);
	}

	PixelTiempo r(c1);
	
	for (int i = 0; i < c0 - 1; i++)
	{
		for (int j = 0; j < q; j++)
		{
			double x = (i * q) + j;
			r[x] = topixel(a[i] + b[i] * j + c[i] * std::pow(j, 2) + d[i] * std::pow(j, 3));
			x += 1;
		}
	}
	r[c1 - 1] = P[c0 - 1];

	return r ;
}
