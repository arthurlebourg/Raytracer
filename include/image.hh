#pragma once

#include <array>
#include <cstdio>
#include <fstream>
#include <string>

class Image
{
public:
    Image(std::string path, int width, int height)
        : path_(path)
        , width_(width)
        , height_(height)
    {
        data_ = std::array<Color>(Color(0, 0, 0));
        data_.fill(Color(0, 0, 0));
    }

    int width()
    {
        return width_;
    }

    int height()
    {
        return height_;
    }

    void set(Color c, int x, int y)
    {
        data_[y * height_ + x] = c;
    }

    void save()
    {
        std::ofstream ofs(path_, std::ios_base::out | std::ios_base::binary);
        ofs << "P6" << std::endl
            << width_ << ' ' << height_ << std::endl
            << "255" << std::endl;

        for (char j = 0; j < height_; ++j)
            for (char i = 0; i < width_; ++i)
                ofs << data_[height_ * j + i].red()
                    << data_[height_ * j + i].green()
                    << data_[height_ * j + i].blue();

        ofs.close();
    }

    friend std::ostream &operator<<(std::ostream &os, const Image &img);

private:
    std::string path_;
    int width_;
    int height_;
    std::array<Color> data_;
};

std::ostream &operator<<(std::ostream &os, const Image &img)
{
    return os << "path: " << img.path_ << ", width: " << img.width_
              << ", height: " << img.height_ << std::endl;
}
