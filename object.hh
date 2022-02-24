#pragma once

#include <iostream>

#include "texture_material.hh"

class Object
{
public:
    virtual Vector3 hit(Vector3 point, Vector3 direction) = 0;

    virtual Vector3 normal(Vector3 point, Vector3 direction) = 0;

    virtual Material get_texture(Vector3 point) = 0;

private:
    Texture_Material texture_;
};
