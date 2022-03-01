#pragma once

#include "object.hh"
#include "ray.hh"
#include "vector3.hh"

class HitInfo
{
public:
    HitInfo(Vector3 hit_location, Vector3 direction,
            std::shared_ptr<Object> obj)
        : hit_location_(hit_location)
        , direction_(direction)
        , obj_(obj)
    {}

    Vector3 get_hit_loc()
    {
        return hit_location_;
    }

    Vector3 get_hit_dir()
    {
        return direction_;
    }

    Object get_hit_obj()
    {
        return obj_;
    }

private:
    Vector3 hit_location_;
    Vector3 direction_;
    Object obj_;
};
