#include <stdio.h>
#include <cstdint>

#include "camera.h"

uint8_t HitAnything(DonMath::Ray& ray, float tMin, float tMax, DonMath::RayHitRecord& record, 
DonMath::BoundingSphere* pSpheres, uint32_t sphereCount)
{
    DonMath::RayHitRecord temp;
    uint8_t hitAnything = 0;
    double closestSoFar = tMax;

    for(uint32_t i = 0; i < sphereCount; ++i)
    {
        if(pSpheres[i].RayHitCheck(ray, tMin, closestSoFar, temp))
        {
            hitAnything = 1;
            closestSoFar = temp.t;
            record = temp;
        }
    }

    return hitAnything;
}

DonMath::vec3 TestColor(DonMath::Ray& ray, DonMath::BoundingSphere* pSpheres, uint32_t sphereCount)
{
    DonMath::RayHitRecord record;
    if(HitAnything(ray, 0.f, FLT_MAX, record, pSpheres, sphereCount))
        return DonMath::vec3(record.normal.x + 1, record.normal.y + 1, record.normal.z + 1) * 0.5;

    DonMath::vec3 unitDirection = DonMath::UnitVector(ray.direction);
    float t = 0.5 * (unitDirection.y + 1.0);
    return DonMath::vec3(1.f) * (1.f - t) + DonMath::vec3(0.5f, 0.7f, 1.f) * t;
}

int main()
{
	uint32_t nx = 200;
	uint32_t ny = 100;
    uint32_t ns = 100;

    DonMath::vec3 lowerLeftCorner(-2.f, -1.f, -1.f);
    DonMath::vec3 horizontal(4.f, 0.f, 0.f);
    DonMath::vec3 vertical(0.f, 2.f, 0.f);
    DonMath::vec3 origin(0.f);

    DonMath::BoundingSphere spheres[2];
    spheres[0].center = DonMath::vec3(0.f, 0.f, -1.f);
    spheres[0].radius = 0.5f;
    spheres[1].center = DonMath::vec3(0, -100.5f, -1);
    spheres[1].radius = 100;

    Donner::Camera camera;

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

            DonMath::RayPointAt(ray, 2.f);
            DonMath::vec3 color = TestColor(ray, spheres, 2);

			uint32_t ir = uint32_t(255.99 * color.r);
			uint32_t ig = uint32_t(255.99 * color.g);
			uint32_t ib = uint32_t(255.99 * color.b);

			std::cout << ir << ' ' << ig << ' ' << ib << '\n';
		}
	}
}