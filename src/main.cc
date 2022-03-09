#include <iostream>
#include <limits>
#include <thread>
#include <tuple>

#include "earth_texture.hh"
#include "gif.hh"
#include "hit_info.hh"
#include "image.hh"
#include "obj_load.hh"
#include "plane.hh"
#include "point_light.hh"
#include "ray.hh"
#include "scene.hh"
#include "skybox_texture.hh"
#include "sphere.hh"
#include "triangle.hh"
#include "uniform_texture.hh"

const size_t img_width = 1280;
const size_t img_height = 960;
const int max_threads = std::thread::hardware_concurrency();

const int anti_aliasing = 4;
const int sqrt_anti_aliasing = sqrt(anti_aliasing);

Hit_Info find_closest_obj(const std::vector<std::shared_ptr<Object>> &objects,
                          Ray ray)
{
    float min_dist = std::numeric_limits<float>::max();
    std::optional<Vector3> hit = std::nullopt;
    std::shared_ptr<Object> object = nullptr;
    for (size_t i = 0; i < objects.size(); i++)
    {
        auto new_hit = objects[i]->hit(ray);
        if (new_hit.has_value())
        {
            float new_dist = (new_hit.value() - ray.origin()).squaredNorm();
            if (new_dist < min_dist)
            {
                min_dist = new_dist;
                object = objects[i];
                hit = new_hit;
            }
        }
    }
    return Hit_Info(hit, ray.direction(), object);
}

bool is_shadowed(const Scene &scene, const Ray &light_ray,
                 std::shared_ptr<Object> object)
{
    auto other_object = find_closest_obj(scene.objects_, light_ray).get_obj();

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
        auto hit_info = find_closest_obj(scene.objects_, ray);
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

Color skybox(Ray ray, const Scene &sc)
{
    Hit_Info hit = find_closest_obj(sc.skybox_, ray);

    if (hit.get_obj() == nullptr)
        return Color(255, 255, 0);

    Material material = hit.get_obj()->get_texture(hit.get_location());

    Color c = material.get_color();

    return c;
    // float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    // Color default_color = r > 0.98 ? Color(255,255,255) : Color(0, 0, 0);
}

void make_image_threads(Scene sc, double miny, double maxy, Image *img)
{
    for (double y = miny; y < maxy; y++)
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
                Ray ray = sc.camera_.get_ray(
                    x_pixel + x_distance * (n / sqrt_anti_aliasing),
                    y_pixel + y_distance * ((int)n % sqrt_anti_aliasing));
                auto hit_info = find_closest_obj(sc.objects_, ray);

                if (hit_info.get_obj() == nullptr)
                {
                    col = col + skybox(ray, sc) * (1.0 / anti_aliasing);
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
            img->set(col, x, y);
        }
        if (miny == 0)
            std::cout << y << "/" << maxy << std::endl;
    }
}

int make_gif(Scene &sc, int frames)
{
    Gif gif = Gif("raytrace.gif", img_width, img_height, frames);
    Color default_color(0, 125, 255);

    for (int frame = 0; frame < frames; ++frame)
    {
        for (double y = 0; y < img_height; y++)
        {
            for (double x = 0; x < img_width; x++)
            {
                Ray ray = sc.camera_.get_ray(x / img_width, y / img_height);
                auto hit_info = find_closest_obj(sc.objects_, ray);

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
        sc.camera_.change_pos(Vector3(0, frame < 50 ? 0.05 : -0.05, 0));
        sc.objects_[1]->move(Vector3(0, frame < frames / 2 ? 0.1 : -0.1, 0));
        sc.objects_[0]->move(Vector3(frame < frames / 2 ? 0.05 : -0.05, 0, 0));
        std::cout << "frame: " << frame << std::endl;
        gif.write_frame();
    }
    gif.save();

    return 0;
}

void make_video(Scene sc, int frames_begin, int frames_end, Color *res)
{
    for (int frame = frames_begin; frame < frames_end; frame++)
    {
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
                    Ray ray = sc.camera_.get_ray(
                        x_pixel + x_distance * (n / sqrt_anti_aliasing),
                        y_pixel + y_distance * ((int)n % sqrt_anti_aliasing));
                    auto hit_info = find_closest_obj(sc.objects_, ray);

                    if (hit_info.get_obj() == nullptr)
                    {
                        col = col + skybox(ray, sc) * (1.0 / anti_aliasing);
                    }
                    else
                    {
                        col = col
                            + get_color(hit_info.get_obj(), sc,
                                        hit_info.get_location(),
                                        hit_info.get_dir(), 5)
                                * (1.0 / anti_aliasing);
                    }
                }
                sc.objects_[0]->set_position(Vector3(-50, -625, 100));

                sc.camera_.set_position(Vector3(0, 0, -frame));

                res[(int)(y * img_width + x) + frame * img_width * img_height] =
                    col;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    std::srand(time(NULL));
    double seed = std::rand();
    double fov_w = 90;
    double fov_h = 120.0;
    double dist_to_screen = 1;
    double dist_to_skybox = 5000;

    size_t frames = 220;

    Vector3 camCenter(0, 0, 0);
    Vector3 camFocus(0, 0, 1);
    Vector3 camUp(0, 1, 0.2);

    Camera cam = Camera(camCenter, camFocus, camUp.normalized(), fov_w / 2,
                        fov_h / 2, dist_to_screen);
    Scene sc = Scene(cam, 5, dist_to_skybox, seed);

    Vector3 light_pos(4, 5, 5);
    float luminosty = 1;
    Point_Light light(luminosty, light_pos);
    sc.lights_.push_back(std::make_shared<Point_Light>(light));

    argv = argv;

    Earth_Texture planete_tex = Earth_Texture(seed);

    Sphere green_boulasse = Sphere(
        Vector3(60, 0, 300), 600, std::make_shared<Earth_Texture>(planete_tex));

    sc.objects_.push_back(std::make_shared<Sphere>(green_boulasse));

    // OBJLoad obj("models/amogus_hands.objet");

    /*for (Triangle t : obj.get_triangles())
    {
        sc.objects_.push_back(std::make_shared<Triangle>(t));
    }*/

    if (max_threads == 0)
    {
        std::cerr << "error threads" << std::endl;
        return 1;
    }
    std::cout << "Engaging on " << max_threads << " threads" << std::endl;

    if (argc > 1)
    {
        size_t frames_per_thread = frames / max_threads;
        std::vector<std::thread> threads;
        Color *data = static_cast<Color *>(
            calloc(img_width * img_height * frames, sizeof(Color)));
        for (int i = 0; i < max_threads - 1; i++)
        {
            threads.push_back(std::thread(make_video, sc.copy_for_thread(),
                                          i * frames_per_thread,
                                          (i + 1) * frames_per_thread, data));
            std::cout << i + 1 << std::endl;
        }

        make_video(sc, (max_threads - 1) * frames_per_thread,
                   max_threads * frames_per_thread, data);

        std::string ffmpeg_data =
            "ffmpeg -loglevel quiet -y -f rawvideo -vcodec rawvideo -pix_fmt "
            "rgb24 -s "
            + std::to_string(img_width) + std::string("x")
            + std::to_string(img_height)
            + std::string(
                " -r 25 -i - -f mp4 -q:v 5 -an -vcodec mpeg4 raytracer.mp4");

        FILE *pipeout = popen(ffmpeg_data.c_str(), "w");

        std::cout << "pipe opened" << std::endl;

        unsigned char frame[img_height][img_width][3] = { 0 };

        for (int i = 0; i < max_threads - 1; i++)
        {
            threads[i].join();
            std::cout << "finished thread: " << i << std::endl;
        }

        for (size_t f = 0; f < frames; f++)
        {
            for (size_t y = 0; y < img_height; y++)
            {
                for (size_t x = 0; x < img_width; x++)
                {
                    Color c = data[((img_height - y - 1) * img_width + x)
                                   + img_width * img_height * f];
                    frame[y][x][0] = c.red(); // red
                    frame[y][x][1] = c.green(); // green
                    frame[y][x][2] = c.blue(); // blue
                }
            }
            fwrite(frame, 1, img_height * img_width * 3, pipeout);
        }
        free(data);
        fflush(pipeout);
        pclose(pipeout);
        std::string ffmpeg_sound = "ffmpeg -loglevel quiet -i raytracer.mp4 -i "
                                   "sound/universal.wav -map 0:v -map 1:a "
                                   "-c:v copy "
                                   "-shortest sound/raytracer_sound.mp4";

        FILE *pipesound = popen(ffmpeg_sound.c_str(), "w");
        fflush(pipesound);
        pclose(pipesound);
        return 0;
    }

    double y_num = img_height / max_threads;
    std::vector<std::thread> threads;
    Image img = Image("bite.ppm", img_width, img_height);
    for (int i = 0; i < max_threads - 1; i++)
    {
        threads.push_back(std::thread(make_image_threads, sc, i * y_num,
                                      (i + 1) * y_num, &img));
    }
    make_image_threads(sc, (max_threads - 1) * y_num, max_threads * y_num,
                       &img);
    for (int i = 0; i < max_threads - 1; i++)
    {
        threads[i].join();
        std::cout << "finished thread: " << i << std::endl;
    }
    img.save();
    std::cout << "image saved" << std::endl;

    return 0;
}
