#pragma once

#include <iostream>

class Color
{
public:
    Color(char red, char green, char blue)
        : red_(red)
        , green_(green)
        , blue_(blue)
    {}

    char red()
    {
        return red_;
    }

    char green()
    {
        return green_;
    }

    char blue()
    {
        return blue_;
    }

private:
    char red_;
    char green_;
    char blue_;
};

inline std::ostream &operator<<(std::ostream &os, Color &col)
{
    return os << "(R: " << col.red() << ", G: " << col.green()
              << ", B: " << col.blue() << ")" << std::endl;
}
