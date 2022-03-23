#pragma once

#include "hit_info.hh"
#include "object.hh"
#include "ray.hh"
#include "texture_material.hh"
#include "vector3.hh"

class Atmosphere : public Object
{
public:
    Atmosphere(Vector3 pos, double radius, double planet_radius,
               std::shared_ptr<Texture_Material> texture)
        : Object(texture)
        , pos_(pos)
        , radius_(radius)
        , planet_radius_(planet_radius)
    {
        scatteringCoef_ = Vector3(pow(400.0 / 700.0, 4.0) * scatterForce_,
                                  pow(400.0 / 530.0, 4.0) * scatterForce_,
                                  pow(400.0 / 440.0, 4.0) * scatterForce_);
    }

    Atmosphere(const Atmosphere &a)
        : Object(a.texture_)
        , scatteringCoef_(a.scatteringCoef_)
        , pos_(a.pos_)
        , radius_(a.radius_)
        , planet_radius_(a.planet_radius_)
    {}

    std::optional<Vector3> hit(Ray ray);

    Vector3 normal(Vector3 point);

    void move(Vector3 vec)
    {
        pos_ = pos_ + vec;
    }

    void set_position(Vector3 vec)
    {
        pos_ = vec;
    }

    std::shared_ptr<Object> clone()
    {
        return std::make_shared<Atmosphere>(Atmosphere(*this));
    }

    bool is_transparent()
    {
        return true;
    }

    Vector3 get_center()
    {
        return pos_;
    }

    double get_radius()
    {
        return radius_;
    }

    double densityAtPoint(Vector3 point)
    {
        double height_above_surface = (point - pos_).length() - planet_radius_;
        double height01 = height_above_surface / (radius_ - planet_radius_);
        double localDensity = exp(-height01 * densityFallOff) * (1 - height01);
        // if (localDensity < 0)
        //     return 0;
        // if (height01 > 1.000001)
        // std::cout << "couille dans paté: " << height01 << std::endl;
        return localDensity;
    }

    double opticalDepth(Vector3 origin, Vector3 dir, double length)
    {
        // if (length > 2* radius_)
        //     std::cout << length << std::endl;
        Vector3 densitySamplePoint = origin;
        double step_size = length / (numOpticalDepthPoints - 1);
        double opticalDepth = 0;

        for (size_t i = 0; i < numOpticalDepthPoints; i++)
        {
            double localDensity = densityAtPoint(densitySamplePoint);
            opticalDepth += localDensity;
            densitySamplePoint = densitySamplePoint + dir * step_size;
        }

        return opticalDepth;
        return opticalDepth / numOpticalDepthPoints;
    }

    Vector3 get_scattering_coef()
    {
        return scatteringCoef_;
    }

private:
    Vector3 scatteringCoef_;
    double densityFallOff = 8;
    double scatterForce_ = 20;
    size_t numOpticalDepthPoints = 10;
    Vector3 pos_;
    double radius_;
    double planet_radius_;
};
