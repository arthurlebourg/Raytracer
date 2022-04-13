#include <string>

#include "maker.hh"

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        Scene sc = sun_scene();
        make_image(sc);
        return 0;
    }

    std::string arg = std::string(argv[1]);
    if (arg == "planet")
    {
        Scene sc = planet();
        make_image(sc);
        return 0;
    }
    if (arg == "amogus")
    {
        Scene sc = amogus();
        make_image(sc);
        return 0;
    }
    if (arg == "atmosphere")
    {
        Scene sc = sample_atmosphere();
        make_image(sc);
        return 0;
    }
    if (arg == "alien")
    {
        Scene sc = alien_scene();
        make_image(sc);
        return 0;
    }
    if (arg == "video")
    {
        Scene sc = sun_scene();
        make_video(sc);
        return 0;
    }
    Scene sc = make_scene();
    make_image(sc);

    return 0;
}
