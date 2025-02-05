#include <stdio.h>
#include <cstdint>

#include "donMath.h"

uint8_t HitSphereTest(DonMath::vec3& center, float radius, DonMath::Ray& ray)
{
    DonMath::vec3 oc = ray.origin - center;
    float a = DonMath::Dot(ray.direction, ray.direction);
    float b = 2.0 * DonMath::Dot(oc, ray.direction);
    float c = DonMath::Dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;
    return discriminant > 0;
}

DonMath::vec3 TestColor(DonMath::Ray& ray)
{
    if(HitSphereTest(DonMath::vec3(0.f, 0.f, -1.f), 0.5f, ray))
        return DonMath::vec3(1.f, 0.f, 0.f);
    DonMath::vec3 unitDirection = DonMath::UnitVector(ray.direction);
    float t = 0.5 * unitDirection.y + 1.0;
    return DonMath::vec3(1.f) * (1.0 - t) + DonMath::vec3(0.5f, 0.7f, 1.f) * t;
}

int main()
{
	uint32_t nx = 200;
	uint32_t ny = 100;

    DonMath::vec3 lowerLeftCorner(-2.f, -1.f, -1.f);
    DonMath::vec3 horizontal(4.f, 0.f, 0.f);
    DonMath::vec3 vertical(0.f, 2.f, 0.f);
    DonMath::vec3 origin(0.f);

	std::cout << "P3\n" << nx << ' ' << ny << "\n255\n";
	for (int32_t j = ny - 1; j >= 0; j--)
	{
		for (uint32_t i = 0; i < nx; ++i)
		{
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);

            DonMath::Ray ray;
            ray.origin = origin;
            ray.direction = lowerLeftCorner + horizontal * u + vertical * v;
            DonMath::vec3 color = TestColor(ray);

			uint32_t ir = uint32_t(255.99 * color.r);
			uint32_t ig = uint32_t(255.99 * color.g);
			uint32_t ib = uint32_t(255.99 * color.b);

			std::cout << ir << ' ' << ig << ' ' << ib << '\n';
		}
	}
}