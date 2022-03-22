#include <memory>
#include <vector>

#include "sphere.hh"
#include "triangle.hh"

class Globbing_Volume
{
public:
    Globbing_Volume(Sphere area)
        : area_(area)
    {}

    Globbing_Volume(const Globbing_Volume &v)
        : area_(v.area_)
    {
        for (auto obj : v.objects_)
            objects_.push_back(obj->clone());
    }

    std::vector<std::shared_ptr<Object>> objects_;
    Sphere area_;

    // the only case so far is when we wanna add all triangles rendered by a
    // blob adding a vector of abstract objects doesn not work
    void add_objects(std::vector<std::shared_ptr<Triangle>> objects)
    {
        objects_.insert(objects_.end(), objects.begin(), objects.end());
    }
};
