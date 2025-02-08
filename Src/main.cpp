#include <stdio.h>
#include <cstdint>

#include "camera.h"
#include "material.h"

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
            record.pMat = pSpheres[i].pMat;
        }
    }

    return hitAnything;
}

DonMath::vec3 TestColor(DonMath::Ray& ray, DonMath::BoundingSphere* pSpheres, uint32_t sphereCount, uint32_t depth)
{
    DonMath::RayHitRecord record;
    // First checks if the ray hit anything
    if(HitAnything(ray, 0.001f, FLT_MAX, record, pSpheres, sphereCount))
    {
        DonMath::Ray scatteredRay;
        DonMath::vec3 attenuation;
        if(depth < 50 && ScatterRay(ray, record, attenuation, scatteredRay))
            return attenuation * TestColor(scatteredRay, pSpheres, sphereCount, depth + 1);
        else
            return DonMath::vec3(0, 0, 0);
    }

    // If it didn't, linear interpolation is done
    DonMath::vec3 unitDirection = DonMath::UnitVector(ray.direction);
    float t = 0.5 * (unitDirection.y + 1.0);
    return DonMath::vec3(1.f) * (1.f - t) + DonMath::vec3(0.5f, 0.7f, 1.f) * t;
}

int main()
{
	uint32_t nx = 200;
	uint32_t ny = 100;
    uint32_t ns = 100;

    Material albedo1;
    albedo1.type = MaterialType::albedo;
    albedo1.albedo = DonMath::vec3(0.8f, 0.8f, 0.3f);

    Material albedo2;
    albedo2.type = MaterialType::albedo;
    albedo2.albedo = DonMath::vec3(0.8f, 0.8f, 0.0f);

    Material metallic1;
    metallic1.type = MaterialType::metallic;
    metallic1.albedo = DonMath::vec3(0.8f, 0.6f, 0.2f);
    metallic1.fuzz = 0.3f;

    Material metallic2;
    metallic2.type = MaterialType::dielectric;
    metallic2.refractionIndex = 1.5f;

    DonMath::BoundingSphere spheres[5];
    // One normal sphere in the middle to reflect on the left and right metallic spheres
    spheres[0].center = DonMath::vec3(0.f, 0.f, -1.f);
    spheres[0].radius = 0.5f;
    spheres[0].pMat = &albedo1;
    // One "sphere" underneath acting as the ground
    spheres[1].center = DonMath::vec3(0, -100.5f, -1.f);
    spheres[1].radius = 100.f;
    spheres[1].pMat = &albedo2;
    // One metallic sphere to the left
    spheres[2].center = DonMath::vec3(1.f, 0.f, -1.f);
    spheres[2].radius = 0.5f;
    spheres[2].pMat = &metallic1;
    // One metallic sphere to the right
    spheres[3].center = DonMath::vec3(-1.f, 0.f, -1.f);
    spheres[3].radius = 0.5f;
    spheres[3].pMat = &metallic2;

    spheres[4].center = DonMath::vec3(-1.f, 0.f, -1.f);
    spheres[4].radius = -0.45f;
    spheres[4].pMat = &metallic2;

    Donner::Camera camera;

	std::cout << "P3\n" << nx << ' ' << ny << "\n255\n";
	for (int32_t j = ny - 1; j >= 0; j--)
	{
		for (uint32_t i = 0; i < nx; ++i)
		{
            DonMath::vec3 color(0.f);
            for(uint32_t s = 0; s < ns; ++s)
            {
                float u = float(i) / float(nx);
                float v = float(j) / float(ny);

                DonMath::Ray ray = camera.GetRay(u, v);
                DonMath::vec3 p = DonMath::RayPointAt(ray, 2.f);

                color = color + TestColor(ray, spheres, 4, 0);
            }
            
            color = color / float(ns);
            color = DonMath::vec3(sqrtf(color.x), sqrtf(color.y), sqrtf(color.z));

			uint32_t ir = uint32_t(255.99 * color.r);
			uint32_t ig = uint32_t(255.99 * color.g);
			uint32_t ib = uint32_t(255.99 * color.b);

			std::cout << ir << ' ' << ig << ' ' << ib << '\n';
		}
	}

    return 1;
}