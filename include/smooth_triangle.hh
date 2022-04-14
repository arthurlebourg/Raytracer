#pragma once

#include "plane.hh"
#include "triangle.hh"

class Smooth_Triangle : public Triangle
{
public:
    Smooth_Triangle(Vector3 A, Vector3 B, Vector3 C, Vector3 normalA,
                    Vector3 normalB, Vector3 normalC,
                    std::shared_ptr<Texture_Material> texture)
        : Triangle(A, B, C, texture)
        , normalA_(normalA)
        , normalB_(normalB)
        , normalC_(normalC)
        , regular_normal_(Triangle::normal(A))
    {}

    Smooth_Triangle(Vector3 A, Vector3 B, Vector3 C,
                    std::shared_ptr<Texture_Material> texture)
        : Triangle(A, B, C, texture)
        , normalA_(Triangle::normal(A))
        , normalB_(Triangle::normal(A))
        , normalC_(Triangle::normal(A))
        , regular_normal_(Triangle::normal(A))
    {}

    Vector3 normal(Vector3 point);
    Vector3 get_regular_normal()
    {
        return regular_normal_;
    }

private:
    // normal at each vertex to be interpolated
    Vector3 normalA_;
    Vector3 normalB_;
    Vector3 normalC_;
    // normal before_interpolation
    Vector3 regular_normal_;
};
