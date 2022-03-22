#pragma once

#include "color.hh"
#include "hit_info.hh"
#include "object.hh"
#include "scene.hh"
#include "vector3.hh"

Hit_Info find_closest_obj(const Scene &scene, Ray ray);

bool is_shadowed(const Scene &scene, const Ray &light_ray,
                 std::shared_ptr<Object> object);

Color get_color(std::shared_ptr<Object> object, const Scene &scene,
                const Vector3 &hit_point, const Vector3 &direction, int n);

Color skybox(Ray ray, const Scene &sc);
