#include <iostream>

#include "color.h"
#include "image.h"
#include "ray.h"
#include "vector3.h"

int main(int argc, const char **argv)
{
    Image img = Image("test.ppm", 10, 10);

    img.save();

    return 0;
}
