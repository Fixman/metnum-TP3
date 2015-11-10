#include "interpolacionlineal.h"

int ceili(int a, int b)
{
	return a / b + !!(a % b);
}

PixelTiempo InterpolacionLineal::alentarPixel(const PixelTiempo &P, int c0, int c1) const
{
	PixelTiempo r(c1);

	int scale = (c1 - 1) / (c0 - 1);
	
	for (int i = 0; i < r.size(); i++)
	{
		int q = int(i / scale);
		int w = ceili(i, scale);

		int x0 = q * scale;
		int y0 = P[q];

		int x1 = w * scale;
		int y1 = P[w];

		if (q == w)
			r[i] = y0;
		else
			r[i] = y0 + (i - x0) * (y0 - y1) / double(x0 - x1);
	}

	return r;
}
