#pragma once

#include <iostream>

#include "vector3.h"

class Ray
{
public:
    Ray(const Vector3 &origin, const Vector3 &direction)
        : origin_(origin)
        , direction_(direction)
    {}

    const Vector3 &origin() const
    {
        return origin_;
    }

    const Vector3 &direction() const
    {
        return direction_;
    }

    Vector3 pointAtParameter(float t) const
    {
        return origin_ + (t * direction_);
    }

    friend std::ostream &operator<<(std::ostream &os, const Ray &ray);

private:
    Vector3 origin_;
    Vector3 direction_;
};

std::ostream &operator<<(std::ostream &os, const Ray &ray)
{
    return os << "origin: " << ray.origin_ << ", direction: " << ray.direction_
              << std::endl;
}
