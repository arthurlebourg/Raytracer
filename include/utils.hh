#pragma once

#include <cmath>
#include <functional>
#include <map>

#include "color.hh"

inline const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180.0;
}

inline Color interpolate(Color color1, Color color2, double fraction)
{
    return Color((color2.red() - color1.red()) * fraction + color1.red(),
                 (color2.green() - color1.green()) * fraction + color1.green(),
                 (color2.blue() - color1.blue()) * fraction + color1.blue());
}

template <typename R, typename... A>
class CacheDecorator
{
public:
    CacheDecorator(std::function<R(A...)> f)
        : f_(f)
    {}

    R operator()(A... a)
    {
        std::tuple<A...> key(a...);
        auto search = map_.find(key);
        if (search != map_.end())
        {
            return search->second;
        }

        auto result = f_(a...);
        map_[key] = result;
        return result;
    }

private:
    std::function<R(A...)> f_;
    std::map<std::tuple<A...>, R> map_;
};
