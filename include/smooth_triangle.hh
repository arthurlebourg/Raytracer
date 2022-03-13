#pragma once

#include "plane.hh"
#include "triangle.hh"

class Smooth_Triangle : public Triangle
{
public:
    Smooth_Triangle(Vector3 A, Vector3 B, Vector3 C,
                    std::shared_ptr<Texture_Material> texture);

    Vector3 normal(Vector3 point);

private:
    // normal at each vertex to be interpolated
    Vector3 normalA_;
    Vector3 normalB_;
    Vector3 normalC_;
};
