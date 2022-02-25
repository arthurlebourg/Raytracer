#include <iostream>

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
            // Ray ray = cam.get_ray(50.0/100,50.0/100);//Ray(Vector3(0,0,0),
            // Vector3(0,0,1));//cam.get_ray(x, y);
            Ray ray = cam.get_ray(x / 680, y / 460);
            // std::cout << ray << std::endl;
            std::optional<Vector3> truc = green_boulasse.hit(ray);
            if (truc.has_value())
            {
                Material mat = green_boulasse.get_texture(ray.origin());
                img.set(mat.get_color(), x, y);
            }
        }
    }
    img.save();

    return 0;
}
