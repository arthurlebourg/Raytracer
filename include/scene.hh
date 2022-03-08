#pragma once

#include <cstring>
#include <memory>
#include <vector>

#include "camera.hh"
#include "light.hh"
#include "object.hh"
#include "skybox_texture.hh"

class Scene
{
public:
    Scene(Camera camera, double ns, double skybox_dist)
        : camera_(camera)
        , ns_(ns)
        , skybox_dist_(skybox_dist)
    {
        Skybox_Texture tex = Skybox_Texture();

        Plane west =
            Plane(Vector3(-skybox_dist_, 0, 0), Vector3(1, 0, 0).normalized(),
                  std::make_shared<Skybox_Texture>(tex));

        Plane east =
            Plane(Vector3(skybox_dist_, 0, 0), Vector3(-1, 0, 0).normalized(),
                  std::make_shared<Skybox_Texture>(tex));

        Plane north =
            Plane(Vector3(0, 0, skybox_dist_), Vector3(0, 0, -1).normalized(),
                  std::make_shared<Skybox_Texture>(tex));

        Plane south =
            Plane(Vector3(0, 0, -skybox_dist_), Vector3(0, 0, 1).normalized(),
                  std::make_shared<Skybox_Texture>(tex));

        Plane top =
            Plane(Vector3(0, skybox_dist_, 0), Vector3(0, -1, 0).normalized(),
                  std::make_shared<Skybox_Texture>(tex));

        Plane bot =
            Plane(Vector3(0, -skybox_dist_, 0), Vector3(0, 1, 0).normalized(),
                  std::make_shared<Skybox_Texture>(tex));

        skybox_.push_back(std::make_shared<Plane>(west));
        skybox_.push_back(std::make_shared<Plane>(east));
        skybox_.push_back(std::make_shared<Plane>(north));
        skybox_.push_back(std::make_shared<Plane>(south));
        skybox_.push_back(std::make_shared<Plane>(top));
        skybox_.push_back(std::make_shared<Plane>(bot));
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
        Scene res(camera_, ns_, skybox_dist_);

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
};
