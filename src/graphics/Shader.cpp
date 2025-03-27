#include "Shader.h"
#include "../config.h"

unsigned int make_module(const std::string& filepath, unsigned int module_type){
    std::ifstream file;
    std::stringstream bufferedLines;
    std::string line;

    file.open(filepath);
    while(std::getline(file, line))
    {
        bufferedLines << line << "\n";
    }
    std::string shaderSource = bufferedLines.str();
    const char* shaderSrc = shaderSource.c_str();
    bufferedLines.str("");
    file.close();

    unsigned int shaderModule = glCreateShader(module_type);
    glShaderSource(shaderModule, 1, &shaderSrc, NULL);
    glCompileShader(shaderModule);

    int success;
    glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
    if(!success){
        char ErrorLog[1024];
        glGetShaderInfoLog(shaderModule, 1024, NULL, ErrorLog);
        std::cout << "Error compiling shader: " << ErrorLog << std::endl;
    }

    return shaderModule;
}

unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath){
    std::vector<unsigned int> shaderModules;
    shaderModules.push_back(make_module(vertex_filepath, GL_VERTEX_SHADER));
    shaderModules.push_back(make_module(fragment_filepath, GL_FRAGMENT_SHADER));

    unsigned int shader = glCreateProgram();
    for(auto i: shaderModules){
        glAttachShader(shader, i);
    }
    glLinkProgram(shader);

    int success;
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if(!success){
        char ErrorLog[1024];
        glGetProgramInfoLog(shader, 1024, NULL, ErrorLog);
        std::cout << "Error linking shader program: " << ErrorLog << std::endl;
    }
    for(auto i: shaderModules){
        glDeleteShader(i);
    }
    return shader;
}

Shader::Shader(const char* vertex_filepath, const char* fragment_filepath){
    ID = make_shader(vertex_filepath, fragment_filepath);
}

void Shader::use(){
    glUseProgram(ID);
}

void Shader::setUniformMat4(const std::string& name, const float* value){
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
}

void Shader::setUniformVec4(const std::string& name, const float* value){
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, value);
}

void Shader::setUniformBool(const std::string &name, bool value)
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setUniformSampler2D(const std::string &name, int textureUnit)
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), textureUnit);
}

void Shader::setUniformVec3(const std::string& name, const float* value) {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, value);
}

Shader::~Shader(){
    glDeleteProgram(ID);
}