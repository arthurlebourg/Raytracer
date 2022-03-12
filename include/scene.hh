#pragma once

#include <cstring>
#include <memory>
#include <vector>

#include "camera.hh"
#include "light.hh"
#include "object.hh"
#include "skybox_sphere.hh"
#include "skybox_texture.hh"

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

        Skybox_Sphere skybox =
            Skybox_Sphere(Vector3(0, 0, 0), skybox_dist,
                          std::make_shared<Skybox_Texture>(tex));

        skybox_.push_back(std::make_shared<Skybox_Sphere>(skybox));
    }

    double ns() const
    {
        return ns_;
    }

    // since we are using abstract classes we need pointers to them
    std::vector<std::shared_ptr<Object>> objects_;
    std::vector<std::shared_ptr<Object>> skybox_;
    std::vector<std::shared_ptr<Light>> lights_;
    Camera camera_;

    Scene copy_for_thread()
    {
        Scene res(camera_, ns_, skybox_dist_, seed_);

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
};
