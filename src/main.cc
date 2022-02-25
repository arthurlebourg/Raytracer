#include <iostream>
#include <limits>

#include "color.hh"
#include "image.hh"
#include "ray.hh"
#include "scene.hh"
#include "sphere.hh"
#include "uniform_texture.hh"
#include "vector3.hh"

Color diffused_color(std::shared_ptr<Object> object, Scene scene,
                     Vector3 hit_point)
{
    scene = scene;
    auto material = object->get_texture(hit_point);
    return material.get_color();
    // TODO :add proper diffusion computation
}

int main()
{
    double fov_w = 110;
    double fov_h = 110;
    double dist_to_screen = 50.0;

    Vector3 camCenter(0, 0, 0);
    Vector3 camFocus(0, 0, 1);
    Vector3 camUp(0, 1, 0);

    Camera cam = Camera(camCenter, camFocus, camUp, fov_w / 2, fov_h / 2,
                        dist_to_screen);
    Scene sc = Scene(cam);

    Uniform_Texture green_tex = Uniform_Texture(Material(Color(0, 255, 0), 1));

    Sphere green_boulasse = Sphere(
        Vector3(0, 0, 51), 50.95, std::make_shared<Uniform_Texture>(green_tex));

    sc.objects_.push_back(std::make_shared<Sphere>(green_boulasse));

    std::cout << cam.get_horizontal() << std::endl
              << cam.get_vertical() << std::endl;

    double img_width = 680;
    double img_height = 460;
    Color default_color(0, 0, 0);

    Image img = Image("bite.ppm", img_width, img_height);

    for (double y = 0; y < img_height; y++)
    {
        for (double x = 0; x < img_width; x++)
        {
            Ray ray = cam.get_ray(x / img_width, y / img_height);
            float min_dist = std::numeric_limits<float>::max();
            std::shared_ptr<Object> object = nullptr;
            std::optional<Vector3> hit = std::nullopt;
            // finds objects with closest point
            for (size_t i = 0; i < sc.objects_.size(); i++)
            {
                hit = sc.objects_[i]->hit(ray);
                if (hit.has_value())
                {
                    float new_dist =
                        (hit.value() - cam.get_center()).squaredNorm();
                    if (new_dist < min_dist)
                    {
                        min_dist = new_dist;
                        object = sc.objects_[i];
                    }
                }
            }

            if (object == nullptr)
            {
                img.set(default_color, x, y);
            }
            else
            {
                img.set(diffused_color(object, sc, hit.value()), x, y);
            }
        }
    }
    img.save();

    return 0;
}
