#pragma once

#include <array>
#include <cstdio>
#include <fstream>
#include <string>

#include "gif.h"

class Gif
{
public:
    Gif(const char *path, size_t width, size_t height, int frames)
        : path_(path)
        , width_(width)
        , height_(height)
        , frames_(frames)
    {
        data_ = static_cast<uint8_t *>(calloc(width * height, 4));
        writer_ = {};
        GifBegin(&writer_, path, width, height, 2, 8, true);
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
        uint8_t *pixel = &data_[((height_ - y) * width_ + x) * 4];
        pixel[0] = c.red();
        pixel[1] = c.green();
        pixel[2] = c.blue();
        pixel[3] = 255; // no alpha for this demo
    }

    void write_frame()
    {
        GifWriteFrame(&writer_, data_, width_, height_, 2, 8, true);
    }

    void save()
    {
        GifEnd(&writer_);
    }

private:
    const char *path_;
    size_t width_;
    size_t height_;
    int frames_;
    uint8_t *data_;
    GifWriter writer_;
};
