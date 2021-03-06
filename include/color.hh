#pragma once

#include <iostream>

class Color
{
public:
    // default black color
    Color()
        : red_(0)
        , green_(0)
        , blue_(0)
    {}

    Color(unsigned char red, unsigned char green, unsigned char blue)
        : red_(red)
        , green_(green)
        , blue_(blue)
    {}

    unsigned char red() const
    {
        return red_;
    }

    unsigned char green() const
    {
        return green_;
    }

    unsigned char blue() const
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
    return os << "(R: " << (int)col.red() << ", G: " << (int)col.green()
              << ", B: " << (int)col.blue() << ")" << std::endl;
}

inline Color operator+(const Color &c, double t)
{
    int red = t + c.red();
    int green = t + c.green();
    int blue = t + c.blue();
    // prevents signed overflow
    return Color(red > 255 ? 255 : red, green > 255 ? 255 : green,
                 blue > 255 ? 255 : blue);
}

inline Color operator+(const Color &lhs, const Color &rhs)
{
    unsigned int red = lhs.red() + rhs.red();
    unsigned int green = lhs.green() + rhs.green();
    unsigned int blue = lhs.blue() + rhs.blue();
    // prevents signed overflow
    return Color(red > 255 ? 255 : red, green > 255 ? 255 : green,
                 blue > 255 ? 255 : blue);
}

inline Color operator*(const Color &c, double t)
{
    if (t <= 0)
        return Color(0, 0, 0);

    int red = t * c.red();
    int green = t * c.green();
    int blue = t * c.blue();
    // prevents signed overflow
    return Color(red > 255 ? 255 : red, green > 255 ? 255 : green,
                 blue > 255 ? 255 : blue);
}
