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
                        110.0, 90.0, 5.0);
    Scene sc = Scene(cam);

    Uniform_Texture green_tex = Uniform_Texture(Material(Color(0, 255, 0), 1));

    Sphere green_boulasse = Sphere(
        Vector3(0, 0, 10), 5, std::make_shared<Uniform_Texture>(green_tex));

    sc.objects_.push_back(std::make_shared<Sphere>(green_boulasse));

    return 0;
}
