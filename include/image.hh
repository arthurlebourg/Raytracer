#pragma once

#include <array>
#include <cstdio>
#include <fstream>
#include <string>

class Image
{
public:
    Image(std::string path, size_t width, size_t height)
        : path_(path)
        , width_(width)
        , height_(height)
    {
        data_ = static_cast<Color *>(calloc(width * height, sizeof(Color)));
    }

    size_t width()
    {
        return width_;
    }

    size_t height()
    {
        return height_;
    }

    void set(Color c, size_t x, size_t y)
    {
        data_[y * height_ + x] = c;
    }

    void save()
    {
        std::ofstream ofs(path_, std::ios_base::out | std::ios_base::binary);
        ofs << "P6" << std::endl
            << width_ << ' ' << height_ << std::endl
            << "255" << std::endl;

        for (size_t j = 0; j < height_; ++j)
            for (size_t i = 0; i < width_; ++i)
                ofs << data_[height_ * j + i].red()
                    << data_[height_ * j + i].green()
                    << data_[height_ * j + i].blue();

        ofs.close();
    }

    friend std::ostream &operator<<(std::ostream &os, const Image &img);

private:
    std::string path_;
    size_t width_;
    size_t height_;
    Color *data_;
};

std::ostream &operator<<(std::ostream &os, const Image &img)
{
    return os << "path: " << img.path_ << ", width: " << img.width_
              << ", height: " << img.height_ << std::endl;
}
