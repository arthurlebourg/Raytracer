#include <iostream>

#include "color.hh"
#include "image.hh"
#include "ray.hh"
#include "vector3.hh"

int main()
{
    Image img = Image("test.ppm", 10, 10);

    img.save();

    return 0;
}
