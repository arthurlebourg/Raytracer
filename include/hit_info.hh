#pragma once

#include "object.hh"
#include "ray.hh"
#include "vector3.hh"

class Hit_Info
{
public:
    Hit_Info(std::optional<Vector3> hit_location, Vector3 direction,
             std::shared_ptr<Object> obj)
        : hit_location_(hit_location)
        , direction_(direction)
        , obj_(obj)
    {}

    Vector3 get_location()
    {
        /* WARNING : this should always be checked with obj_ != nullptr
        beforehand*/
        return hit_location_.value();
    }

    Vector3 get_dir()
    {
        return direction_;
    }

    std::shared_ptr<Object> get_obj()
    {
        return obj_;
    }

private:
    std::optional<Vector3> hit_location_;
    Vector3 direction_;
    std::shared_ptr<Object> obj_;
};
