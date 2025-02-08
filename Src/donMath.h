#pragma once

#include <iostream>
#include <math.h>
#include <stdlib.h>

// Declared here so that the RayHitRecord struct knows about it without causing circular dependencies
struct Material;

namespace DonMath
{

    //inline float Sqrt(float x) {return sqrtf(x);}

    struct vec3
    {
        union
        {
            float x, r, s, u;
        };
        union
        {
            float y, g, t, v;
        };
        union
        {
            float z, b, p, w;
        };

        inline vec3() : x{0.f}, y{0.f}, z{0.f} {}

        inline vec3(float f) : x{f}, y{f}, z{f} {}

        inline vec3(float first, float second, float third) : x{first}, y{second}, z{third} {}

        inline vec3(vec3& copy) : x{copy.x}, y{copy.y}, z{copy.z} {}
    };

    inline vec3 operator + (vec3 v1, vec3 v2) { return vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); }

    inline vec3 operator - (vec3& v1, vec3& v2){ return vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z); }

    inline vec3 operator * (vec3& v1, vec3& v2) { return vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z); }

    inline vec3 operator *(vec3 vec, float scalar) { return vec3(vec.x * scalar, vec.y * scalar, vec.z * scalar); }

    inline vec3 operator /(vec3 vec, float scalar) { return vec3(vec.x / scalar, vec.y / scalar, vec.z / scalar); }

    inline vec3 operator / (vec3& v1, vec3& v2) { return vec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z); }

    inline float LengthSquared(vec3& vec) { return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z; }

    inline float Length(vec3 vec) { return sqrtf(LengthSquared(vec)); }

    inline vec3 UnitVector(vec3& vec) {return vec / Length(vec);}

    inline std::istream& operator >> (std::istream& is, vec3& vec){
        is >> vec.x >> vec.y >> vec.z;
        return is; 
    }

    inline std::ostream& operator << (std::ostream& os, vec3& vec){
        os << vec.x << ' ' << vec.y << ' ' << vec.z;
        return os;
    }

    inline float Dot(vec3& v1, vec3& v2) 
    {
        float f = 0;
        f += v1.x * v2.x;
        f += v1.y * v2.y;
        f += v1.z * v2.z;
        return f;
    }

    inline vec3 Cross(vec3& v1, vec3& v2){
        return vec3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
    }

    struct Ray
    {
        vec3 origin;
        vec3 direction;
    };

    inline vec3 RayPointAt(Ray& ray, float t){ return ray.origin + ray.direction * t; }

    struct RayHitRecord
    {
        float t;

        DonMath::vec3 p;
        DonMath::vec3 normal;

        Material* pMat;
    };

    // Even though the objects are all sphere anyway, 
    // this bounding sphere could be used to check intersection with a ray for any object by generating it from their vertices.
    // This is why I call it a bounding sphere and not just sphere
    struct BoundingSphere
    {
        vec3 center;
        float radius;

        Material* pMat;

        uint8_t RayHitCheck(Ray& ray, float tMin, float tMax, RayHitRecord& hitRecord)
        {
            vec3 oc = ray.origin - center;
            float a = Dot(ray.direction, ray.direction);
            float b = Dot(oc, ray.direction);
            float c = Dot(oc, oc) - radius * radius;
            float discriminant = b * b - a * c;

            if(discriminant > 0)
            {
                float temp = (-b - sqrtf(discriminant)) / a;
                if(temp < tMax && temp > tMin)
                {
                    // Record the hit
                    hitRecord.t = temp;
                    hitRecord.p = RayPointAt(ray, hitRecord.t);
                    hitRecord.normal = (hitRecord.p - center) / radius;

                    // Return true
                    return 1;
                }

                temp = (-b + sqrtf(discriminant)) / a;
                if(temp < tMax && temp > tMin)
                {
                    // Record the hit
                    hitRecord.t = temp;
                    hitRecord.p = RayPointAt(ray, hitRecord.t);
                    hitRecord.normal = (hitRecord.p - center) / radius;

                    // Return true
                    return 1;
                }
            }

            return 0;
        }
    };
}