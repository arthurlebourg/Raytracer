#include "raytracer.hh"

#include "atmosphere.hh"

size_t numInScatteringPoints = 10;

Vector3 calculate_light_atmosphere(Atmosphere *a, const Scene &sc, Ray ray,
                                   double length, Vector3 originalColor)
{
    Vector3 inScatterPoint = ray.origin();
    double step_size = length / (numInScatteringPoints - 1);
    Vector3 inScatteredLight;
    double viewRayOpticalDepth = 0;

    for (size_t i = 0; i < numInScatteringPoints; i++)
    {
        inScatterPoint = ray.origin() + ray.direction() * step_size * i;
        Ray light_ray(inScatterPoint,
                      (sc.lights_[0]->get_pos() - inScatterPoint).normalized());
        auto hit_info = find_closest_obj(sc.objects_, light_ray, true);

        if (!hit_info.get_obj()->is_transparent())
            continue;

        double sunRayLength =
            (hit_info.get_location() - inScatterPoint).length();

        double sunRayOpticalDepth = a->opticalDepth(
            inScatterPoint, light_ray.direction(), sunRayLength);

        viewRayOpticalDepth =
            a->opticalDepth(inScatterPoint, -ray.direction(), step_size * i);

        Vector3 transmittance =
            Vector3(exp(-(sunRayOpticalDepth + viewRayOpticalDepth)
                        * a->scatteringCoef.x()),
                    exp(-(sunRayOpticalDepth + viewRayOpticalDepth)
                        * a->scatteringCoef.y()),
                    exp(-(sunRayOpticalDepth + viewRayOpticalDepth)
                        * a->scatteringCoef.z()));

        double localDensity = a->densityAtPoint(inScatterPoint);

        inScatteredLight = inScatteredLight
            + a->scatteringCoef * localDensity * transmittance * step_size;
    }
    double originalColorTransmittance = 1; // exp(-viewRayOpticalDepth);
    return originalColor * originalColorTransmittance
        + inScatteredLight / numInScatteringPoints;
}

Hit_Info find_closest_obj(const std::vector<std::shared_ptr<Object>> &objects,
                          Ray ray, bool transparent_allowed)
{
    double min_dist = std::numeric_limits<double>::max();
    std::optional<Vector3> hit = std::nullopt;
    std::shared_ptr<Object> object = nullptr;
    for (size_t i = 0; i < objects.size(); i++)
    {
        if (!transparent_allowed && objects[i]->is_transparent())
            continue;
        auto new_hit = objects[i]->hit(ray);
        if (new_hit.has_value())
        {
            double new_dist = (new_hit.value() - ray.origin()).squaredNorm();
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
    auto other_object =
        find_closest_obj(scene.objects_, light_ray, false).get_obj();

    return other_object.get() != object.get();
}

Color get_color(std::shared_ptr<Object> object, const Scene &scene,
                const Vector3 &hit_point, const Vector3 &direction, int n)
{
    auto material = object->get_texture(hit_point);
    Color res;
    if (object->is_transparent())
    {
        const double epsilon = 0.0001;
        auto behind_atmosphere = find_closest_obj(
            scene.objects_, Ray(hit_point + direction * epsilon, direction),
            false);

        Color base_color =
            get_color(behind_atmosphere.get_obj(), scene,
                      behind_atmosphere.get_location(), direction, n - 1);

        auto hit_info = find_closest_obj(
            scene.objects_, Ray(hit_point + direction * epsilon, direction),
            true);

        double dist_through = (hit_info.get_location() - hit_point).length();

        double r = base_color.red();
        double g = base_color.green();
        double b = base_color.blue();
        Vector3 base_color_vec = Vector3(r / 255, g / 255, b / 255);

        Vector3 light = calculate_light_atmosphere(
            dynamic_cast<Atmosphere *>(object.get()), scene,
            Ray(hit_point + direction * epsilon, direction),
            dist_through - epsilon * 2, base_color_vec);

        return Color(light.x() * 255, light.y() * 255, light.z() * 255);
    }

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

        // Reflection ray
        Vector3 S =
            (direction - 2 * normal * dot(object->normal(hit_point), direction))
                .normalized();

        Ray ray = Ray(hit_point + S * 0.001, S);

        auto hit_info = find_closest_obj(scene.objects_, ray, true);
        if (n > 0 && hit_info.get_obj() != nullptr)
        {
            Color reflected = get_color(hit_info.get_obj(), scene,
                                        hit_info.get_location(), S, n - 1)
                * 0.5;

            res = res + diffused_color * 0.5 + reflected * 0.5;
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
