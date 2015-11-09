#include "splines.h"

#include <cmath>

Splines::Splines(Pelicula peli, int c0, int f, int reset)
: CamaraLenta(peli, c0, f), reset(reset)
{
}

uint8_t Splines::toPixel(double n) const
{
	return std::max(0, std::min(255, (int) std::round(n)));
}

Vector Splines::tridiagonalSubstitution(const Vector &a, const Vector &b, Vector c, Vector f) const
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

std::vector <Splines::Polynomial> Splines::buildSpline(const Vector &y, double q) const
{
	int n = y.size();

	// j[1..n) -> subdiagonal
	// k[0..n) -> diagonal
	// l[0..n-1) -> superdiagonal
	Vector j(n - 1, q);
	Vector k(n, 4 * q);
	Vector l(n - 1, q);
	j[n - 2] = l[0] = 0;
	k[0] = k[n - 1] = 1;

	Vector f(n);
	f[0] = f[n - 1] = 0;
	for (int i = 1; i < n - 1; i++)
		f[i] = (3 / q) * (y[i - 1] - 2 * y[i] + y[i + 1]);
	
	Vector c = tridiagonalSubstitution(j, k, l, f);

	std::vector <Polynomial> p(n);
	for (int i = 0; i < n - 1; i++)
	{
		p[i].a = y[i];
		p[i].b = (y[i + 1] - y[i]) / q - (q / 3) * (2 * c[i] + c[i + 1]);
		p[i].c = c[i];
		p[i].d = (c[i + 1] - c[i]) / (3 * q);
	}

	p[n - 1].a = y[n - 1];
	p[n - 1].b = p[n - 1].c = p[n - 1].d = 0;

	return p;
}

PixelTiempo Splines::alentarPixel(const PixelTiempo &P, int c0, int c1) const
{
	int q = (c1 - 1) / (c0 - 1);
	std::vector <Polynomial> z = buildSpline(Vector(P.begin(), P.end()), q);

	PixelTiempo r(c1);
	for (int i = 0; i < c0 - 1; i++)
	{
		for (int j = 0; j < q; j++)
		{
			double x = (i * q) + j;
			r[x] = toPixel(z[i].a + z[i].b * j + z[i].c * std::pow(j, 2) + z[i].d * std::pow(j, 3));
		}
	}
	r[c1 - 1] = P[c0 - 1];

	return r ;
}
