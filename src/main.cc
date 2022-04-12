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
    Scene sc = make_scene();
    if (arg == "planet")
    {
        Scene sc = planet();
    }
    if (arg == "amogus")
    {
        Scene sc = amogus();
    }
    if (arg == "atomsphere")
    {
        Scene sc = sample_atmosphere();
    }
    if (arg == "video")
    {
        Scene sc = sun_scene();
        make_video(sc);
        return 0;
    }
    make_image(sc);

    return 0;
}
