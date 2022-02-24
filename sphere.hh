#pragma once

#include <iostream>

#include "texture_material.h"

class Sphere : Object
{
public:
    Sphere(Vector3 pos, float radius, Texture_Material texture)
        : pos_(pos)
        , radius_(radius)
        , texture_(texture)
    {}

    Vector3 hit(Vector3 point, Vector3 direction);

    Vector3 normal(Vector3 point);

    Material get_texture(Vector3 point);

private:
    Vector3 pos_;
    float radius_
};
