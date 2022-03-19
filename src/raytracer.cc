#include "raytracer.hh"

#include "atmosphere.hh"

size_t numInScatteringPoints = 5;

double calculate_light_atmosphere(Atmosphere *a, const Scene &sc, Ray ray,
                                  double length)
{
    Vector3 inScatterPoint = ray.origin();
    double step_size = length / (numInScatteringPoints - 1);
    double inScatteredLight = 0;
    for (size_t i = 0; i < numInScatteringPoints; i++)
    {
        Ray light_ray(inScatterPoint,
                      (inScatterPoint - sc.lights_[0]->get_pos()).normalized());
        auto hit_info =
            find_closest_obj(sc.objects_,
                             Ray(inScatterPoint + light_ray.direction() * 0.01,
                                 light_ray.direction()));
        double sunRayLength =
            (hit_info.get_location() - inScatterPoint).length();
        double sunRayOpticalDepth = a->opticalDepth(
            inScatterPoint, light_ray.direction(), sunRayLength);
        double viewRayOpticalDepth =
            a->opticalDepth(inScatterPoint, -ray.direction(), step_size * i);
        double transmittance = exp(-(sunRayOpticalDepth + viewRayOpticalDepth));
        double localDensity = a->densityAtPoint(inScatterPoint);

        inScatteredLight += localDensity * transmittance * step_size;
        inScatterPoint = inScatterPoint + ray.direction() * step_size;
    }
    return inScatteredLight;
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
        auto hit_info = find_closest_obj(
            scene.objects_, Ray(hit_point + direction * 0.01, direction));

        Color transparent =
            get_color(hit_info.get_obj(), scene, hit_info.get_location(),
                      direction, n - 1);

        Vector3 vect_through = hit_info.get_location() - hit_point;

        double dist_through = vect_through.length();

        double radius = (object->get_center() - hit_point).length();
        double ratio = dist_through / (radius * 2);

        if (ratio > 1)
            return transparent;

        double light = calculate_light_atmosphere(
            dynamic_cast<Atmosphere *>(object.get()), scene,
            Ray(hit_point, direction), dist_through);
        return transparent * (1 - light) + light;
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
            direction - 2 * normal * dot(object->normal(hit_point), direction);

        Ray ray = Ray(hit_point + S * 0.001, S);

        auto hit_info = find_closest_obj(scene.objects_, ray);
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
