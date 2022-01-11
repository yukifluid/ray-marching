#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int id;

    Shader(const std::string &vertex_path, const std::string &fragment_path);

    void Use() const;

    void SetBool(const std::string &name, const bool &value) const;
    void SetInt(const std::string &name, const int &value) const;
    void SetFloat(const std::string &name, const float &value) const;
    void SetVec2(const std::string &name, const glm::vec2 &value) const;
    void SetVec3(const std::string &name, const glm::vec3 &value) const;
    void SetVec3(const std::string &name, const glm::dvec3 &value) const;
    void SetMat4(const std::string &name, const glm::mat4 &value) const;
};