#pragma once

#include "alien_texture.hh"
#include "atmosphere.hh"
#include "earth_texture.hh"
#include "hit_info.hh"
#include "image.hh"
#include "obj_load.hh"
#include "plane.hh"
#include "planet.hh"
#include "point_light.hh"
#include "ray.hh"
#include "scene.hh"
#include "skybox_texture.hh"
#include "sphere.hh"
#include "sun_texture.hh"
#include "triangle.hh"
#include "uniform_texture.hh"

Scene make_scene();

Scene amogus();

Scene planet();

Scene sample_atmosphere();

Scene sun_scene();

Scene alien_scene();
