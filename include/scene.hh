#pragma once

#include <memory>
#include <vector>

#include "camera.hh"
#include "light.hh"
#include "object.hh"

class Scene
{
public:
    Scene(Camera camera, float ns)
        : camera_(camera)
        , ns_(ns)
    {}

    float ns() const
    {
        return ns_;
    }

    // since we are using abstract classes we need pointers to them
    std::vector<std::shared_ptr<Object>> objects_;
    std::vector<std::shared_ptr<Light>> lights_;

private:
    Camera camera_;
    float ns_;
};
