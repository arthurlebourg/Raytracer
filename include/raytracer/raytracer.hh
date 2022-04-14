#pragma once

#include "raytracer/hit_info.hh"
#include "raytracer/scene.hh"
#include "texture_material/color.hh"

Hit_Info find_closest_obj(const std::vector<std::shared_ptr<Object>> &objects,
                          Ray ray, bool transparent_allowed);

bool is_shadowed(const Scene &scene, const Ray &light_ray,
                 std::shared_ptr<Object> object);

Color get_color(std::shared_ptr<Object> object, const Scene &scene,
                const Vector3 &hit_point, const Vector3 &direction, int n);
