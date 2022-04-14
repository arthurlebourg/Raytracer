#pragma once

#include "atmosphere/atmosphere.hh"
#include "blob/asteroid.hh"
#include "blob/moon.hh"
#include "blob/planet.hh"
#include "maker/image.hh"
#include "object/sphere.hh"
#include "planet_texture/alien_texture.hh"
#include "planet_texture/earth_texture.hh"
#include "planet_texture/skybox_texture.hh"
#include "raytracer/hit_info.hh"
#include "raytracer/point_light.hh"
#include "raytracer/scene.hh"
#include "texture_material/sun_texture.hh"
#include "texture_material/uniform_texture.hh"

Scene make_scene();

Scene planet();

Scene sample_atmosphere();

Scene sun_scene();

Scene moon_scene();

Scene alien_scene();
