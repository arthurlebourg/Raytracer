#pragma once

#include <iostream>

#include "texture_material.hh"

class Light
{
public:
    virtual double get_intensity() = 0;

    virtual Vector3 get_pos() = 0;

    virtual std::shared_ptr<Light> clone() = 0;

    virtual void set_position(Vector3 vec) = 0;
};
