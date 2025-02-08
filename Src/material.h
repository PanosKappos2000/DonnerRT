#pragma once

#include "donMath.h"

// This could be done with inheritance but I do not like inheritance. Also coming from real time graphics, materials should be one thing
enum class MaterialType : uint8_t
{
    albedo = 0,
    metallic = 1,
    dielectric = 2,

    Unknown = 3
};

struct Material
{
    DonMath::vec3 albedo;

    MaterialType type;

    float fuzz;

    float refractionIndex;
};

inline DonMath::vec3 Reflect(DonMath::vec3& v, DonMath::vec3& n)
{
    return v - n * 2 * DonMath::Dot(v, n);
}

inline uint8_t Refract(DonMath::vec3& v, DonMath::vec3& n, float niOverNt, DonMath::vec3& refracted)
{
    DonMath::vec3 uv = DonMath::UnitVector(v);
    float dt = DonMath::Dot(uv, n);

    float discriminant = 1.f - niOverNt * niOverNt * (1 - dt * dt);

    if(discriminant > 0)
    {
        refracted = (uv - n * dt) * niOverNt - n * sqrtf(discriminant);
        return 1;
    }

    return 0;
}

inline float schlick(float cosine, float refraction)
{
    float r0 = (1 - refraction) / (1 + refraction);
    r0 = r0 * r0;

    return r0 + (1 - r0) * pow(1 - cosine, 5);
}

inline uint8_t ScatterRay(DonMath::Ray& ray, DonMath::RayHitRecord& record, DonMath::vec3& attenuation, DonMath::Ray& scatteredRay)
{
    switch(record.pMat->type)
    {
    case MaterialType::albedo:
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
    case MaterialType::metallic:
    {
        if(record.pMat->fuzz > 1.f)
            record.pMat->fuzz = 1.f;

        DonMath::vec3 random;
        do{
            random = DonMath::vec3(rand() / 100.f) - DonMath::vec3(1.f);
        }while(DonMath::LengthSquared(random) >= 1.f);
        
        DonMath::vec3 reflected = Reflect(DonMath::UnitVector(ray.direction), record.normal);

        scatteredRay.origin = record.p;
        scatteredRay.direction = reflected + random * record.pMat->fuzz;

        attenuation = record.pMat->albedo;

        return DonMath::Dot(scatteredRay.direction, record.normal) > 0;
    }
    case MaterialType::dielectric:
    {
        DonMath::vec3 outwardNormal;
        DonMath::vec3 reflected = Reflect(ray.direction, record.normal);

        float niOverNt;
        attenuation = DonMath::vec3(1.f, 1.f, 0.f);
        float cosine;

        if(DonMath::Dot(ray.direction, record.normal) > 0.f)
        {
            outwardNormal = record.normal * (-1.f);
            niOverNt = record.pMat->refractionIndex;
            cosine = record.pMat->refractionIndex * DonMath::Dot(ray.direction, record.normal) / DonMath::Length(ray.direction);
        }
        else
        {
            outwardNormal = record.normal;
            niOverNt = 1.f / record.pMat->refractionIndex;
            cosine = -DonMath::Dot(ray.direction, record.normal) / DonMath::Length(ray.direction);
        }
        
        DonMath::vec3 refracted;
        float reflectProb;
        if(Refract(ray.direction, outwardNormal, niOverNt, refracted))
        {
            reflectProb = schlick(cosine, record.pMat->refractionIndex);
        }
        else
        {
            reflectProb = 1.f;
        }

        if(rand() / 100.f < reflectProb)
        {
            scatteredRay.origin = record.p;
            scatteredRay.direction = reflected;
        }
        else
        {
            scatteredRay.origin = record.p;
            scatteredRay.direction = refracted;
        }
        return 1;
    }
    default:
        return 0;
    }
}