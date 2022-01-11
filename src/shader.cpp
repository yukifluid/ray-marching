#include "shader.hpp"

Shader::Shader(const std::string &vertex_path, const std::string &fragment_path)
{
    std::string vertex_sourcecode;
    std::string fragment_sourcecode;

    std::ifstream vertex_shader_file;
    std::ifstream fragment_shader_file;

    vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vertex_shader_file.open(vertex_path);
        fragment_shader_file.open(fragment_path);
        std::stringstream vertex_shader_stream;
        std::stringstream fragment_shader_stream;
        vertex_shader_stream << vertex_shader_file.rdbuf();
        fragment_shader_stream << fragment_shader_file.rdbuf();
        vertex_shader_file.close();
        fragment_shader_file.close();
        vertex_sourcecode = vertex_shader_stream.str();
        fragment_sourcecode = fragment_shader_stream.str();
    }
    catch(const std::exception& e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vertex_shader_code = vertex_sourcecode.c_str();
    const char* fragment_shader_code = fragment_sourcecode.c_str();

    unsigned int vertex;
    unsigned int fragment;

    int success;
    char info_log[512];

    vertex = glCreateShader(GL_VERTEX_SHADER); 
    glShaderSource(vertex, 1, &vertex_shader_code, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, info_log);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FALIED\n" << info_log << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER); 
    glShaderSource(fragment, 1, &fragment_shader_code, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, info_log);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FALIED\n" << info_log << std::endl;
    }

    this->id = glCreateProgram();

    glAttachShader(this->id, vertex);
    glAttachShader(this->id, fragment);
    glLinkProgram(this->id);
    glGetProgramiv(this->id, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(this->id, 512, NULL, info_log);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FALIED\n" << info_log << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Use() const{ glUseProgram(this->id); }
void Shader::SetBool(const std::string &name, const bool &value) const{ glUniform1i(glGetUniformLocation(this->id, name.c_str()), (int)value); }
void Shader::SetInt(const std::string &name, const int &value) const { glUniform1i(glGetUniformLocation(this->id, name.c_str()), value); }
void Shader::SetFloat(const std::string &name, const float &value) const { glUniform1f(glGetUniformLocation(this->id, name.c_str()), value); }
void Shader::SetVec2(const std::string &name, const glm::vec2 &value) const { glUniform2fv(glGetUniformLocation(this->id, name.c_str()), 1, glm::value_ptr(value)); }
void Shader::SetVec3(const std::string &name, const glm::vec3 &value) const { glUniform3fv(glGetUniformLocation(this->id, name.c_str()), 1, glm::value_ptr(value)); }
void Shader::SetVec3(const std::string &name, const glm::dvec3 &value) const { glUniform3fv(glGetUniformLocation(this->id, name.c_str()), 1, glm::value_ptr(glm::vec3(value))); }
void Shader::SetMat4(const std::string &name, const glm::mat4 &value) const { glUniformMatrix4fv(glGetUniformLocation(this->id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value)); }