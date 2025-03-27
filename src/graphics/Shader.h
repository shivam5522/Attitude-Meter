#ifndef SHADER_H
#define SHADER_H

#include "../config.h"

class Shader{
    public:
        unsigned int ID;

        Shader(const char* vertex_filepath, const char* fragment_filepath);
        void use();
        void setUniformMat4(const std::string& name, const float* value);
        void setUniformVec4(const std::string& name, const float* value);
        void setUniformBool(const std::string &name, bool value);
        void setUniformSampler2D(const std::string &name, int textureUnit);
        void setUniformVec3(const std::string& name, const float* value);
        ~Shader();
};

#endif