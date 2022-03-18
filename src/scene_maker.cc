#include "scene_maker.hh"

Scene make_scene()
{
    std::srand(time(NULL));
    double seed = std::rand();
    std::cout << "seed: " << seed << std::endl;
    double fov_w = 90;
    double fov_h = 120;
    double dist_to_screen = 1;
    double dist_to_skybox = 5000;

    Vector3 camCenter(0, 0, 0);
    Vector3 camFocus(0, 0, 1);
    Vector3 camUp(0, 1, 0);

    Camera cam = Camera(camCenter, camFocus, camUp.normalized(), fov_w / 2,
                        fov_h / 2, dist_to_screen);
    Scene sc = Scene(cam, 5, dist_to_skybox, seed);

    Vector3 light_pos(0, 500, 5);
    double luminosty = 2;
    Point_Light light(luminosty, light_pos);
    sc.lights_.push_back(std::make_shared<Point_Light>(light));

    Earth_Texture planete_tex = Earth_Texture(seed, 200, 200);

    Sphere green_boulasse =
        // Sphere(Vector3(-50, 0, 1500), 600,
        Sphere(Vector3(0, -600, 800), 600,
               std::make_shared<Earth_Texture>(planete_tex));

    sc.objects_.push_back(std::make_shared<Sphere>(green_boulasse));
    return sc;
}

Scene amogus()
{
    std::srand(time(NULL));
    double seed = std::rand();
    std::cout << "seed: " << seed << std::endl;
    double fov_w = 90;
    double fov_h = 120;
    double dist_to_screen = 1;
    double dist_to_skybox = 5000;

    Vector3 camCenter(0, 0, 0);
    Vector3 camFocus(0, 0, 1);
    Vector3 camUp(0, 1, 0);

    Camera cam = Camera(camCenter, camFocus, camUp.normalized(), fov_w / 2,
                        fov_h / 2, dist_to_screen);
    Scene sc = Scene(cam, 5, dist_to_skybox, seed);

    Vector3 light_pos(0, 500, 5);
    double luminosty = 2;
    Point_Light light(luminosty, light_pos);
    sc.lights_.push_back(std::make_shared<Point_Light>(light));

    OBJLoad obj("models/amogus_hands.objet");

    for (Triangle t : obj.get_triangles())
    {
        sc.objects_.push_back(std::make_shared<Triangle>(t));
    }

    return sc;
}

Scene planet()
{
    std::srand(time(NULL));
    double seed = std::rand();
    std::cout << "seed: " << seed << std::endl;
    double fov_w = 90;
    double fov_h = 120;
    double dist_to_screen = 1;
    double dist_to_skybox = 5000;

    Vector3 camCenter(0, 0, 0);
    Vector3 camFocus(0, 0, 1);
    Vector3 camUp(0, 1, 0);

    Camera cam = Camera(camCenter, camFocus, camUp.normalized(), fov_w / 2,
                        fov_h / 2, dist_to_screen);
    Scene sc = Scene(cam, 5, dist_to_skybox, seed);

    Vector3 light_pos(0, 0, 5);
    double luminosty = 2;
    Point_Light light(luminosty, light_pos);
    sc.lights_.push_back(std::make_shared<Point_Light>(light));

    Uniform_Texture planete_tex =
        Uniform_Texture(Material(Color(0, 255, 0), 1, 1));

    Planet green_boulasse =
        Planet(Vector3(-100, -100, 100), 200, 25, 0.5, seed);

    auto triangles =
        green_boulasse.render(std::make_shared<Uniform_Texture>(planete_tex));

    std::cout << "size : " << triangles.size() << std::endl;

    sc.objects_.insert(sc.objects_.end(), triangles.begin(), triangles.end());
    return sc;
}

Scene sample_atmosphere()
{
    std::srand(time(NULL));
    double seed = std::rand();
    std::cout << "seed: " << seed << std::endl;
    double fov_w = 90;
    double fov_h = 120;
    double dist_to_screen = 1;
    double dist_to_skybox = 5000;

    Vector3 camCenter(0, 0, 0);
    Vector3 camFocus(0, 0, 1);
    Vector3 camUp(0, 1, 0);

    Camera cam = Camera(camCenter, camFocus, camUp.normalized(), fov_w / 2,
                        fov_h / 2, dist_to_screen);
    Scene sc = Scene(cam, 5, dist_to_skybox, seed);

    Vector3 light_pos(0, 0, 5);
    double luminosty = 2;
    Point_Light light(luminosty, light_pos);
    sc.lights_.push_back(std::make_shared<Point_Light>(light));

    Uniform_Texture tex = Uniform_Texture(Material(Color(255, 255, 255), 1, 1));

    Atmosphere atmos(Vector3(0, 0, 20), 10,
                     std::make_shared<Uniform_Texture>(tex));

    sc.objects_.push_back(std::make_shared<Atmosphere>(atmos));
    return sc;
}
