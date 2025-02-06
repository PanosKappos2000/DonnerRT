#pragma once

#include "donMath.h"

namespace Donner
{
    struct Camera
    {
    
        // Hardcoded camera to view the raytraced image
        inline Camera(){
            lowerLeftCorner = DonMath::vec3(-2.f, -1.f, -1.f);

            horizontal = DonMath::vec3(4.f, 0.f, 0.f);

            vertical = DonMath::vec3(0.f, 2.f, 0.f);

            origin = DonMath::vec3(0.f);
        }

        DonMath::Ray GetRay(float u, float v){
            DonMath::Ray res;
            res.direction = lowerLeftCorner + horizontal * u + vertical * v - origin;
            res.origin = origin;
            return res;
        }

        DonMath::vec3 origin;
        DonMath::vec3 lowerLeftCorner;
        DonMath::vec3 horizontal;
        DonMath::vec3 vertical;
    };
}