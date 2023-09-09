#include "Shader.hpp"


#include <memory>


Shader::Shader(std::string vertexShader, std::string fragmentShader) : m_vertexShader(vertexShader),
                                                                       m_fragmentShader(fragmentShader),
                                                                       program(0)
{

}

void Shader::bind()
{
    GLCall(program = glCreateProgram());
    GLCall(unsigned int vs = compileShader(GL_VERTEX_SHADER, m_vertexShader));
    GLCall(unsigned int fs = compileShader(GL_FRAGMENT_SHADER, m_fragmentShader));

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    glDeleteShader(fs);
    glDeleteShader(vs);

    GLCall(glUseProgram(program));
}

void Shader::unbind()
{
    GLCall(glUseProgram(0));
}

Shader::~Shader()
{
    if (program != 0)
    {
        GLCall(glDeleteProgram(program));
    }
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source)
{
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result = 0;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length = 0;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        std::unique_ptr<char []> message = std::make_unique<char []>(length);
        glGetShaderInfoLog(id, length, &length, message.get());
        std::cout << "Failed to compile shader: ";
        std::cout << message;
        std::cout << std::endl;
    }
    return id;
}