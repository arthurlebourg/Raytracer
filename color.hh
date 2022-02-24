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

    friend std::ostream &operator<<(std::ostream &os, const Color &col);

private:
    char red_;
    char green_;
    char blue_;
};

std::ostream &operator<<(std::ostream &os, const Color &col)
{
    return os << "(R: " << col.red_ << ", G: " << col.green_
              << ", B: " << col.blue_ << ")" << std::endl;
}
