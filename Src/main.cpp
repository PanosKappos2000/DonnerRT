#include <stdio.h>
#include <cstdint>

#include "donMath.h"

int main()
{
	uint32_t nx = 200;
	uint32_t ny = 100;

	std::cout << "P3\n" << nx << ' ' << ny << "\n255\n";
	for (int32_t j = ny - 1; j >= 0; j--)
	{
		for (uint32_t i = 0; i < nx; ++i)
		{
            DonMath::vec3 vec(float(i) / float(nx), float(j) / float(ny), 0.2f);

			uint32_t ir = uint32_t(255.99 * vec.r);
			uint32_t ig = uint32_t(255.99 * vec.g);
			uint32_t ib = uint32_t(255.99 * vec.b);

			std::cout << ir << ' ' << ig << ' ' << ib << '\n';
		}
	}
}