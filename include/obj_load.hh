#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "triangle.hh"

std::string *split_str(std::string &str, char delim)
{
    static std::string res[4];
    std::string word = "";
    int i = 0;
    for (auto x : str)
    {
        if (x == delim)
        {
            res[i] = word;
            word = "";
            i++;
        }
        else
        {
            word = word + x;
        }
    }
    res[i] = word;

    return res;
}

class OBJLoad
{
public:
    OBJLoad(std::string path)
        : path_(path)
    {
        std::vector<Vector3> vertex;
        std::ifstream input(path);
        Uniform_Texture green_tex =
            Uniform_Texture(Material(Color(0, 255, 0), 1, 100));

        for (std::string line; getline(input, line);)
        {
            // get vertices
            if (line[0] == 'v' && line[1] == ' ')
            {
                std::string *splitted = split_str(line, ' ');
                /*
                std::cout << line << std::endl;
                std::cout << splitted[1] << std::endl;
                std::cout << splitted[2] << std::endl;
                std::cout << splitted[3] << std::endl << std::endl;
                */
                vertex.push_back(Vector3(std::stof(splitted[1]),
                                         std::stof(splitted[2]),
                                         std::stof(splitted[3])));
            }

            // make Triangles
            if (line[0] == 'f')
            {
                std::string *splitted = split_str(line, ' ');
                Vector3 A =
                    vertex[std::stoi(split_str(splitted[1], '/')[0]) - 1];
                Vector3 B =
                    vertex[std::stoi(split_str(splitted[2], '/')[0]) - 1];
                Vector3 C =
                    vertex[std::stoi(split_str(splitted[3], '/')[0]) - 1];

                Triangle t = Triangle(
                    A, B, C, std::make_shared<Uniform_Texture>(green_tex));

                triangles_.push_back(t);
            }
        }
    }

    std::vector<Triangle> get_triangles()
    {
        return triangles_;
    }

private:
    std::string path_;
    std::vector<Triangle> triangles_;
};
