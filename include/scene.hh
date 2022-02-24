#pragma once

#include <vector>

#include "object.hh"

class Scene
{
public
    Scene(Camera camera)
        : camera_(camera)
    {}
    // will need to add more methods

private:
    std::vector<Object> objects_;
    std::vector<Light> lights_;
    Camera camera_;
}
