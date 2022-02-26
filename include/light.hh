#pragma once

#include <iostream>

#include "texture_material.hh"

class Light
{
public:
    virtual float get_intensity() = 0;

    virtual Vector3 get_pos() = 0;
};
