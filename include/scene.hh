#pragma once

#include <cstring>
#include <memory>
#include <vector>

#include "camera.hh"
#include "light.hh"
#include "object.hh"
#include "skybox_texture.hh"
#include "sphere.hh"

class Scene
{
public:
    Scene(Camera camera, double ns, double skybox_dist, double seed)
        : camera_(camera)
        , ns_(ns)
        , skybox_dist_(skybox_dist)
        , seed_(seed)
    {
        Skybox_Texture tex = Skybox_Texture(seed, 2000, 2000);

        Sphere skybox = Sphere(Vector3(0, 0, 0), skybox_dist,
                               std::make_shared<Skybox_Texture>(tex));

        std::shared_ptr<Sphere> skybox_ptr = std::make_shared<Sphere>(skybox);
        skybox_ptr->set_skybox(true);
        objects_.push_back(skybox_ptr);
    }

    Scene(Camera camera, double ns)
        : camera_(camera)
        , ns_(ns)
    {}

    double ns() const
    {
        return ns_;
    }

    // since we are using abstract classes we need pointers to them
    std::vector<std::shared_ptr<Object>> objects_;
    std::vector<std::shared_ptr<Light>> lights_;
    Camera camera_;

    Scene copy_for_thread()
    {
        Scene res(camera_, ns_);

        for (auto i : objects_)
        {
            auto clone = i->clone();
            if (i->is_skybox())
            {
                clone->set_skybox(true);
            }
            res.objects_.push_back(clone);
        }
        for (auto i : lights_)
        {
            res.lights_.push_back(i->clone());
        }

        return res;
    }

private:
    double ns_;
    double skybox_dist_;
    double seed_;
};
