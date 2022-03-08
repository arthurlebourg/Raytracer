#include <iostream>
#include <limits>
#include <tuple>

#include "gif.hh"
#include "hit_info.hh"
#include "image.hh"
#include "metaball.hh"
#include "plane.hh"
#include "point_light.hh"
#include "ray.hh"
#include "scene.hh"
#include "sphere.hh"
#include "triangle.hh"
#include "uniform_texture.hh"

const size_t img_width = 640;
const size_t img_height = 480;

const int anti_aliasing = 2;
const int sqrt_anti_aliasing = sqrt(anti_aliasing);

Hit_Info find_closest_obj(const Scene &sc, Ray ray)
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
    return Hit_Info(hit, ray.direction(), object);
}

bool is_shadowed(const Scene &scene, const Ray &light_ray,
                 std::shared_ptr<Object> object)
{
    auto other_object = find_closest_obj(scene, light_ray).get_obj();

    return other_object.get() != object.get();
}

Color get_color(std::shared_ptr<Object> object, const Scene &scene,
                const Vector3 &hit_point, const Vector3 &direction, int n = 5)
{
    auto material = object->get_texture(hit_point);
    Color res;
    for (auto &light : scene.lights_)
    {
        // ray cast from point to light
        Ray light_ray(hit_point, (light->get_pos() - hit_point).normalized());
        Ray shadow_ray(light->get_pos(),
                       (hit_point - light->get_pos()).normalized());

        // checks if another object is in the way of the light
        bool shadowed = is_shadowed(scene, shadow_ray, object);
        if (shadowed)
        {
            // return object->get_texture(hit_point).get_color() * 0.5;
            continue;
        }

        Color diffused_color = material.get_color()
            * material.get_diffusion_coeff()
            * dot(object->normal(hit_point), light_ray.direction())
            * light->get_intensity();

        Vector3 S = direction
            - 2 * object->normal(hit_point)
                * dot(object->normal(hit_point), direction);

        // Reflection ray
        Ray ray = Ray(hit_point + S * 0.001, S);
        auto hit_info = find_closest_obj(scene, ray);
        if (n > 0 && hit_info.get_obj() != nullptr)
        {
            res = res + diffused_color * 0.5
                + get_color(hit_info.get_obj(), scene, hit_info.get_location(),
                            S, n - 1)
                    * 0.5;
        }
        else
        {
            res = res + diffused_color * 0.5;
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
                auto hit_info = find_closest_obj(sc, ray);

                if (hit_info.get_obj() == nullptr)
                {
                    gif.set(default_color, x, y);
                }
                else
                {
                    Color c = get_color(hit_info.get_obj(), sc,
                                        hit_info.get_location(),
                                        hit_info.get_dir(), 5);
                    gif.set(c, x, y);
                }
            }
        }
        cam.change_pos(Vector3(0, frame < 50 ? 0.05 : -0.05, 0));
        sc.objects_[1]->move(Vector3(0, frame < frames / 2 ? 0.1 : -0.1, 0));
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
            Color col;
            for (double n = -anti_aliasing / 2; n < anti_aliasing / 2; n++)
            {
                double x_pixel = x / img_width;
                double y_pixel = y / img_height;
                double x_distance = x_pixel - (x + 1) / img_width;
                double y_distance = y_pixel - (y + 1) / img_height;
                Ray ray = cam.get_ray(
                    x_pixel + x_distance * (n / sqrt_anti_aliasing),
                    y_pixel + y_distance * ((int)n % sqrt_anti_aliasing));
                auto hit_info = find_closest_obj(sc, ray);

                if (hit_info.get_obj() == nullptr)
                {
                    col = col + default_color * (1.0 / anti_aliasing);
                }
                else
                {
                    col = col
                        + get_color(hit_info.get_obj(), sc,
                                    hit_info.get_location(), hit_info.get_dir(),
                                    5)
                            * (1.0 / anti_aliasing);
                }
            }
            img.set(col, x, y);
        }
    }
    img.save();

    return 0;
}

int main(int argc, char *argv[])
{
    double fov_w = 90.0;
    double fov_h = 120.0;
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
    Uniform_Texture blue_tex =
        Uniform_Texture(Material(Color(0, 0, 255), 1, 10));

    Sphere green_boulasse = Sphere(
        Vector3(2, -1, 5), 2, std::make_shared<Uniform_Texture>(green_tex));

    Sphere red_boulasse = Sphere(Vector3(-4, 0, 8), 2,
                                 std::make_shared<Uniform_Texture>(red_tex));

    Plane plancher = Plane(Vector3(0, -5, 0), Vector3(0, 1, 0),
                           std::make_shared<Uniform_Texture>(gray_tex));

    Plane mur = Plane(Vector3(-5, 0, 0), Vector3(1, 0, 0).normalized(),
                      std::make_shared<Uniform_Texture>(blue_tex));

    Triangle illuminati =
        Triangle(Vector3(3, 0, 2), Vector3(3, 2, 2), Vector3(1, 2, 2),
                 std::make_shared<Uniform_Texture>(red_tex));

    // sc.objects_.push_back(std::make_shared<Sphere>(green_boulasse));
    // sc.objects_.push_back(std::make_shared<Sphere>(red_boulasse));
    sc.objects_.push_back(std::make_shared<Plane>(plancher));
    sc.objects_.push_back(std::make_shared<Plane>(mur));
    // sc.objects_.push_back(std::make_shared<Triangle>(illuminati));
    green_boulasse = green_boulasse;
    red_boulasse = red_boulasse;
    illuminati = illuminati;

    Vector3 blob_corner(-3, -3, 2);
    float blob_length = 6.0;
    int blob_step = 20;
    float blob_thresh = 1.0;

    Metaball metaball(blob_corner, blob_length, blob_step, blob_thresh);
    // TODO : can we handle multiple texture inside the same blob ?
    metaball.balls.push_back(Sphere(
        Vector3(0, 0, 3), 1, std::make_shared<Uniform_Texture>(green_tex)));
    metaball.balls.push_back(Sphere(
        Vector3(1.5, 1.5, 1.5), 1, std::make_shared<Uniform_Texture>(red_tex)));

    auto triangles =
        metaball.render(std::make_shared<Uniform_Texture>(red_tex));
    std::cout << "nb triangle : " << triangles.size() << "\n";
    sc.objects_.insert(sc.objects_.end(), triangles.begin(), triangles.end());

    if (argc > 1)
    {
        return make_gif(cam, sc, 100);
    }
    return make_image(cam, sc);
}
