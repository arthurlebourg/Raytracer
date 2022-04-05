#include <iostream>
#include <limits>
#include <thread>
#include <tuple>

#include "raytracer.hh"
#include "scene_maker.hh"

const size_t img_width = 1440;
const size_t img_height = 1080;
const int max_threads = std::thread::hardware_concurrency() + 1;

const int anti_aliasing = 4;
const int sqrt_anti_aliasing = sqrt(anti_aliasing);

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
                auto hit_info = find_closest_obj(sc.objects_, ray, true);

                if (hit_info.get_obj() == nullptr)
                {
                    col = col + Color(255, 255, 0) * (1.0 / anti_aliasing);
                }
                else
                {
                    col = col
                        + get_color(hit_info.get_obj(), sc,
                                    hit_info.get_location(),
                                    hit_info.get_dir().normalized(), 1)
                            * (1.0 / anti_aliasing);
                    /*if (hit_info.get_obj()->is_skybox())
                    {
                        col = col
                            + hit_info.get_obj()
                                    ->get_texture(hit_info.get_location())
                                    .get_color()
                                * (1.0 / anti_aliasing);
                    }
                    else
                    {
                        col = col
                            + get_color(hit_info.get_obj(), sc,
                                        hit_info.get_location(),
                                        hit_info.get_dir().normalized(), 1)
                                * (1.0 / anti_aliasing);
                    }*/
                }
            }
            img->set(col, x, y);
        }
        if (miny == 0)
            std::cout << y << "/" << maxy << std::endl;
    }
}

void make_video(Scene sc, int frames_begin, int frames_end, Color *res)
{
    for (int frame = frames_begin; frame < frames_end; frame++)
    {
        // Sample atmosphere
        /*sc.objects_[1]->set_position(Vector3(-frame, 0, -frame / 4.0 + 200));
        sc.objects_[2]->set_position(Vector3(-frame, 0, -frame / 4.0 + 200));

        sc.lights_[0]->set_position(Vector3(300 - frame, 200, 300 - 3 * frame));
        */
        sc.camera_.set_rotation_y(-frame / 4.0);
        if (frames_begin == 0)
        {
            std::cout << frame << "/" << frames_end << std::endl;
        }

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
                    auto hit_info = find_closest_obj(sc.objects_, ray, true);

                    if (hit_info.get_obj() == nullptr)
                    {
                        col = col + Color(255, 255, 0) * (1.0 / anti_aliasing);
                    }
                    else
                    {
                        col = col
                            + get_color(hit_info.get_obj(), sc,
                                        hit_info.get_location(),
                                        hit_info.get_dir().normalized(), 1)
                                * (1.0 / anti_aliasing);
                    }
                }

                res[(int)(y * img_width + x) + frame * img_width * img_height] =
                    col;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    argv = argv;
    size_t frames = 360;

    // Scene sc = make_scene();
    // Scene sc = planet();
    // Scene sc = amogus();
    // Scene sc = sample_atmosphere();
    Scene sc = sun_scene();

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
        }

        make_video(sc, (max_threads - 1) * frames_per_thread,
                   max_threads * frames_per_thread, data);

        std::string ffmpeg_data =
            "ffmpeg -loglevel quiet -y -f rawvideo -vcodec rawvideo -pix_fmt "
            "rgb24 -s "
            + std::to_string(img_width) + std::string("x")
            + std::to_string(img_height)
            + std::string(
                //" -r 25 -i - -f webm -q:v 5 -an -vcodec libvpx-vp9
                // raytracer.webm");
                " -r 25 -i - -f mp4 -q:v 5 -an -vcodec mpeg4 "
                "img/raytracer_bad_encoding.mp4");

        FILE *pipeout = popen(ffmpeg_data.c_str(), "w");

        std::cout << "pipe opened" << std::endl;

        unsigned char frame[img_height][img_width][3] = { 0 };

        for (int i = 0; i < max_threads - 1; i++)
        {
            threads[i].join();
        }
        std::cout << "finished threads" << std::endl;

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
        std::string ffmpeg_encoding =
            "ffmpeg -loglevel quiet -y -i img/raytracer_bad_encoding.mp4 "
            "-strict very raytracer.mp4";

        FILE *pipe_encoding = popen(ffmpeg_encoding.c_str(), "w");
        fflush(pipe_encoding);
        pclose(pipe_encoding);
        std::string ffmpeg_sound =
            "ffmpeg -loglevel quiet -y -i raytracer.mp4 -i "
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
    }
    std::cout << "finished threads" << std::endl;
    img.save();
    std::cout << "image saved" << std::endl;

    return 0;
}
