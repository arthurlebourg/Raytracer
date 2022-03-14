#include "raytracer.hh"

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
                const Vector3 &hit_point, const Vector3 &direction, int n)
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
}
