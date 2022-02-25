#include <iostream>
#include <limits>

#include "color.hh"
#include "image.hh"
#include "ray.hh"
#include "scene.hh"
#include "sphere.hh"
#include "uniform_texture.hh"
#include "vector3.hh"

int main()
{
    Camera cam = Camera(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0),
                        55.0, 55.0, 50.0);
    Scene sc = Scene(cam);

    Uniform_Texture green_tex = Uniform_Texture(Material(Color(0, 255, 0), 1));

    Sphere green_boulasse = Sphere(
        Vector3(0, 0, 51), 50.95, std::make_shared<Uniform_Texture>(green_tex));

    sc.objects_.push_back(std::make_shared<Sphere>(green_boulasse));

    std::cout << cam.get_horizontal() << std::endl
              << cam.get_vertical() << std::endl;

    Image img = Image("bite.ppm", 680, 460);

    for (double y = 0; y < 460; y++)
    {
        for (double x = 0; x < 680; x++)
        {
            Ray ray = cam.get_ray(x / 680, y / 460);
            float min_dist = std::numeric_limits<float>::max();
            Material mat = Material(Color(0, 0, 0), 0);
            for (size_t i = 0; i < sc.objects_.size(); i++)
            {
                std::optional<Vector3> hit = sc.objects_[i]->hit(ray);
                if (hit.has_value())
                {
                    float new_dist =
                        (hit.value() - cam.get_center()).squaredNorm();
                    if (new_dist < min_dist)
                    {
                        min_dist = new_dist;
                        mat = sc.objects_[i]->get_texture(hit.value());
                    }
                }
            }
            img.set(mat.get_color(), x, y);
        }
    }
    img.save();

    return 0;
}
