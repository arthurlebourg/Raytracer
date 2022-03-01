#include <iostream>
#include <limits>
#include <tuple>

#include "color.hh"
#include "gif.h"
#include "gif.hh"
#include "image.hh"
#include "plane.hh"
#include "point_light.hh"
#include "ray.hh"
#include "scene.hh"
#include "sphere.hh"
#include "uniform_texture.hh"
#include "vector3.hh"

const size_t img_width = 640;
const size_t img_height = 480;

bool is_shadowed(const Scene &scene, const Ray &light_ray,
                 std::shared_ptr<Object> object)
{
    bool is_shadowed = false;

    for (auto other_object : scene.objects_)
    {
        if (other_object == object)
            continue;
        if (other_object->hit(light_ray).has_value())
        {
            is_shadowed = true;
            break;
        }
    }
    return is_shadowed;
}

std::tuple<std::shared_ptr<Object>, std::optional<Vector3>>
find_closest_obj(const Scene &sc, Ray ray)
{
    float min_dist = std::numeric_limits<float>::max();
    std::optional<Vector3> hit = std::nullopt;
    std::shared_ptr<Object> object = nullptr;
    for (size_t i = 0; i < sc.objects_.size(); i++)
    {
        auto new_hit = sc.objects_[i]->hit(ray);
        if (new_hit.has_value())
        {
            float new_dist = (new_hit.value() - ray.origin()).squaredNorm();
            if (new_dist < min_dist)
            {
                min_dist = new_dist;
                object = sc.objects_[i];
                hit = new_hit;
            }
        }
    }
    return std::make_tuple(object, hit);
}

Color get_color(std::shared_ptr<Object> object, const Scene &scene,
                const Vector3 &hit_point, const Vector3 &direction, int n = 5)
{
    auto material = object->get_texture(hit_point);
    Color res;
    for (auto light : scene.lights_)
    {
        // ray cast from point to light
        Ray light_ray(hit_point, (light->get_pos() - hit_point).normalized());

        // checks if another object is in the way of the light
        bool shadowed = is_shadowed(scene, light_ray, object);
        if (shadowed)
            continue;

        Color diffused_color = material.get_color()
            * material.get_diffusion_coeff()
            * dot(object->normal(hit_point), light_ray.direction())
            * light->get_intensity();

        Vector3 S = direction
            - 2 * object->normal(hit_point)
                * dot(object->normal(hit_point), direction);

        Ray ray = Ray(hit_point, S);
        auto trace = find_closest_obj(scene, ray);
        if (n > 0 && std::get<0>(trace) != nullptr)
        {
            res = res + diffused_color * 0.5
                + get_color(std::get<0>(trace), scene,
                            std::get<1>(trace).value(), S, n - 1)
                    * 0.5;
        }
        else
        {
            res = res + diffused_color;
        }

        float dotp = dot(S, light_ray.direction());
        if (dotp < 0)
            continue;

        float spec =
            material.ks() * pow(dotp, scene.ns()) * light->get_intensity();

        res = res + spec;
    }
    return res;
}

int make_gif(Camera &cam, Scene &sc, int frames)
{
    Gif gif = Gif("raytrace.gif", img_width, img_height, frames);
    Color default_color(0, 125, 255);

    for (int frame = 0; frame < frames; ++frame)
    {
        for (double y = 0; y < img_height; y++)
        {
            for (double x = 0; x < img_width; x++)
            {
                Ray ray = cam.get_ray(x / img_width, y / img_height);
                auto trace = find_closest_obj(sc, ray);

                if (std::get<0>(trace) == nullptr)
                {
                    gif.set(default_color, x, y);
                }
                else
                {
                    Color c = get_color(
                        std::get<0>(trace), sc, std::get<1>(trace).value(),
                        cam.get_ray(x / img_width, y / img_height).direction(),
                        5);
                    gif.set(c, x, y);
                }
            }
        }
        // cam.change_pos(Vector3(0,frame < 50 ? 0.05 : -0.05,0));
        sc.objects_[1]->move(Vector3(0, frame < frames / 2 ? 0.05 : -0.05, 0));
        sc.objects_[0]->move(Vector3(frame < frames / 2 ? 0.05 : -0.05, 0, 0));
        std::cout << "frame: " << frame << std::endl;
        gif.write_frame();
    }
    gif.save();

    return 0;
}

int make_image(Camera &cam, Scene &sc)
{
    Image img = Image("bite.ppm", img_width, img_height);
    Color default_color(0, 125, 255);

    for (double y = 0; y < img_height; y++)
    {
        for (double x = 0; x < img_width; x++)
        {
            Ray ray = cam.get_ray(x / img_width, y / img_height);
            auto trace = find_closest_obj(sc, ray);

            if (std::get<0>(trace) == nullptr)
            {
                img.set(default_color, x, y);
            }
            else
            {
                Color c = get_color(
                    std::get<0>(trace), sc, std::get<1>(trace).value(),
                    cam.get_ray(x / img_width, y / img_height).direction(), 5);
                img.set(c, x, y);
            }
        }
    }
    img.save();

    return 0;
}

int main(int argc, char *argv[])
{
    double fov_w = 90.0;
    double fov_h = 110.0;
    double dist_to_screen = 1;

    Vector3 camCenter(0, 0, 0);
    Vector3 camFocus(0, 0, 1);
    Vector3 camUp(0, 1, 0);

    Camera cam = Camera(camCenter, camFocus, camUp, fov_w / 2, fov_h / 2,
                        dist_to_screen);
    std::cout << cam.get_horizontal() << std::endl
              << cam.get_vertical() << std::endl;
    Scene sc = Scene(cam, 5);

    Vector3 light_pos(5, 5, 5);
    float luminosty = 1;
    Point_Light light(luminosty, light_pos);
    sc.lights_.push_back(std::make_shared<Point_Light>(light));

    argv = argv;

    Uniform_Texture green_tex =
        Uniform_Texture(Material(Color(0, 255, 0), 1, 100));
    Uniform_Texture red_tex =
        Uniform_Texture(Material(Color(255, 0, 0), 1, 100));
    Uniform_Texture gray_tex =
        Uniform_Texture(Material(Color(125, 125, 125), 1, 10));

    Sphere green_boulasse = Sphere(
        Vector3(2, -1, 5), 2, std::make_shared<Uniform_Texture>(green_tex));

    Sphere red_boulasse = Sphere(Vector3(-4, 0, 8), 2,
                                 std::make_shared<Uniform_Texture>(red_tex));

    Plane plancher = Plane(Vector3(0, -1, 0), Vector3(0, 1, 0),
                           std::make_shared<Uniform_Texture>(gray_tex));

    Sphere gray_sphere = Sphere(Vector3(0, 0, 10), 0.5,
                                std::make_shared<Uniform_Texture>(gray_tex));

    sc.objects_.push_back(std::make_shared<Sphere>(green_boulasse));
    sc.objects_.push_back(std::make_shared<Sphere>(red_boulasse));
    sc.objects_.push_back(std::make_shared<Plane>(plancher));
    red_boulasse = red_boulasse;
    gray_sphere = gray_sphere;

    if (argc > 1)
    {
        return make_gif(cam, sc, 100);
    }
    return make_image(cam, sc);
}
