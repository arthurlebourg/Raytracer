#pragma once

#include <iostream>

class Color
{
public:
    Color(unsigned char red, unsigned char green, unsigned char blue)
        : red_(red)
        , green_(green)
        , blue_(blue)
    {}

    unsigned char red()
    {
        return red_;
    }

    unsigned char green()
    {
        return green_;
    }

    unsigned char blue()
    {
        return blue_;
    }

private:
    unsigned char red_;
    unsigned char green_;
    unsigned char blue_;
};

inline std::ostream &operator<<(std::ostream &os, Color &col)
{
    return os << "(R: " << col.red() << ", G: " << col.green()
              << ", B: " << col.blue() << ")" << std::endl;
}
