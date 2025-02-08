#pragma once

#include "donMath.h"

struct Material
{
    DonMath::vec3 albedo;

    uint8_t metallic;
};

inline DonMath::vec3 Reflect(DonMath::vec3& v, DonMath::vec3& n)
{
    return v - n * 2 * DonMath::Dot(v, n);
} 

inline uint8_t ScatterRay(DonMath::Ray& ray, DonMath::RayHitRecord& record, DonMath::vec3& attenuation, DonMath::Ray& scatteredRay)
{
    if(!record.pMat->metallic)
    {
        DonMath::vec3 random;
        do{
            random = DonMath::vec3(rand() / 100.f) - DonMath::vec3(1.f);
        }while(DonMath::LengthSquared(random) >= 1.f);

        DonMath::vec3 target = record.p + record.normal + random;

        scatteredRay.origin = record.p;
        scatteredRay.direction = target - record.p;

        attenuation = record.pMat->albedo;

        return 1;
    }
    else
    {
        DonMath::vec3 reflected = Reflect(DonMath::UnitVector(ray.direction), record.normal);

        scatteredRay.origin = record.p;
        scatteredRay.direction = reflected;

        attenuation = record.pMat->albedo;

        return DonMath::Dot(scatteredRay.direction, record.normal) > 0;
    }
}