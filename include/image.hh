#pragma once

#include <array>
#include <cstdio>
#include <fstream>
#include <string>

#include "color.hh"

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

    ~Image()
    {
        free(data_);
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
        data_[y * width_ + x] = c;
    }

    void save()
    {
        std::ofstream ofs(path_, std::ios_base::out | std::ios_base::binary);
        ofs << "P6" << std::endl
            << width_ << ' ' << height_ << std::endl
            << "255" << std::endl;

        // for (size_t j = height_ - 1; j < height_; j--)
        for (size_t j = height_ - 1; j < height_; j--)
            for (size_t i = 0; i < width_; i++)
                ofs << data_[width_ * j + i].red()
                    << data_[width_ * j + i].green()
                    << data_[width_ * j + i].blue();

        ofs.close();
    }

private:
    std::string path_;
    size_t width_;
    size_t height_;
    Color *data_;
};
