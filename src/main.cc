#include <iostream>
#include <limits>

#include "color.hh"
#include "image.hh"
#include "plane.hh"
#include "ray.hh"
#include "scene.hh"
#include "sphere.hh"
#include "uniform_texture.hh"
#include "vector3.hh"

int main()
{
    double fov_w = 90;
    double fov_h = 110;
    double dist_to_screen = 50.0;
    Camera cam = Camera(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0),
                        fov_w / 2, fov_h / 2, dist_to_screen);
    Scene sc = Scene(cam);

    Uniform_Texture green_tex = Uniform_Texture(Material(Color(0, 255, 0), 1));
    Uniform_Texture red_tex = Uniform_Texture(Material(Color(255, 0, 0), 1));
    Uniform_Texture gray_tex =
        Uniform_Texture(Material(Color(125, 125, 125), 1));

    Sphere green_boulasse = Sphere(
        Vector3(0, 0, 51), 50.95, std::make_shared<Uniform_Texture>(green_tex));

    Sphere red_boulasse = Sphere(Vector3(0, -0.25, 51), 50.95,
                                 std::make_shared<Uniform_Texture>(red_tex));

    Plane plancher = Plane(Vector3(0, -1, 0), Vector3(0, 1, 0),
                           std::make_shared<Uniform_Texture>(gray_tex));

    sc.objects_.push_back(std::make_shared<Sphere>(green_boulasse));
    sc.objects_.push_back(std::make_shared<Sphere>(red_boulasse));
    sc.objects_.push_back(std::make_shared<Plane>(plancher));

    std::cout << cam.get_horizontal() << std::endl
              << cam.get_vertical() << std::endl;

    double img_width = 680;
    double img_height = 460;

    Image img = Image("bite.ppm", img_width, img_height);

    for (double y = 0; y < img_height; y++)
    {
        for (double x = 0; x < img_width; x++)
        {
            Ray ray = cam.get_ray(x / img_width, y / img_height);
            float min_dist = std::numeric_limits<float>::max();
            Material mat = Material(
                Color((255 - y * 100 / 255), (255 - y * 100 / 255), 255), 0);
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
