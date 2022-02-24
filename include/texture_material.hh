#pragma once

#include "color.hh"
#include "vector3.hh"

class Material
{
public:
    Material(Color kd, float ks)
        : kd_(kd)
        , ks_(ks)
    {}

private:
    Color kd_;
    float ks_;
};

class Texture_Material
{
public:
    virtual Material get_Material(Vector3 point);

private:
};