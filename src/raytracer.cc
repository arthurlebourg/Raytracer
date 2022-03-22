#include "raytracer.hh"

Hit_Info find_closest_obj(const Scene &scene, Ray ray)
{
    double min_dist = std::numeric_limits<double>::max();
    std::optional<Vector3> hit = std::nullopt;
    std::shared_ptr<Object> object = nullptr;

    for (auto volume : scene.volumes_)
    {
        if (volume.area_.hit(ray).has_value())
        {
            for (auto obj : volume.objects_)
            {
                auto new_hit = obj->hit(ray);
                if (new_hit.has_value())
                {
                    double new_dist =
                        (new_hit.value() - ray.origin()).squaredNorm();
                    if (new_dist < min_dist)
                    {
                        min_dist = new_dist;
                        object = obj;
                        hit = new_hit;
                    }
                }
            }
            break;
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

        Vector3 normal = object->normal(hit_point);
        if (dot(normal, direction) > 0)
            normal = -normal;

        Color diffused_color = material.get_color()
            * material.get_diffusion_coeff()
            * dot(normal, light_ray.direction()) * light->get_intensity();

        Vector3 S =
            direction - 2 * normal * dot(object->normal(hit_point), direction);

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

        double dotp = dot(S, light_ray.direction());
        if (dotp < 0)
            continue;

        double spec =
            material.ks() * pow(dotp, scene.ns()) * light->get_intensity();

        res = res + spec;
    }
    return res;
}

Color skybox(Ray ray, const Scene &sc)
{
    auto hit_point = sc.skybox_[0]->hit(ray);
    Material material = sc.skybox_[0]->get_texture(hit_point.value());

    Color c = material.get_color();

    return c;
}
