#pragma once

#include <iostream>

#include "texture_material.h"

class Light
{
    public:
        virtual float get_intensity() = 0; 
        
        Vector3 get_pos()
        {
            return pos_;
        }

    private:    
        Vector3 pos_;
        float intensity_;
};

