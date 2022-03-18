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

        skybox_ = std::make_shared<Sphere>(skybox);
        skybox_->set_skybox(true);
        objects_.push_back(skybox_);
    }

    Scene(Camera camera, double ns, std::shared_ptr<Object> skybox)
        : camera_(camera)
        , ns_(ns)
    {
        objects_.push_back(skybox);
    }

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
        Scene res(camera_, ns_, skybox_);

        for (auto i : objects_)
        {
            res.objects_.push_back(i->clone());
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
    std::shared_ptr<Object> skybox_;
};
