#include "Shader.hpp"


#include <memory>

namespace SetUniformDetailed
{
template <class ...Types>
void setUniformValue(int location, Types... values)
{
	std::cout << "Unknown function: " << __FUNCTION__ << std::endl;
}

template<>
void setUniformValue(int location, float f1)
{
	GLCall(glUniform1f(location, f1));
}

template<>
void setUniformValue(int location, float f1, float f2)
{
	GLCall(glUniform2f(location, f1, f2));
}

template<>
void setUniformValue(int location, float f1, float f2, float f3)
{
	GLCall(glUniform3f(location, f1, f2, f3));
}

template<>
void setUniformValue(int location, float f1, float f2, float f3, float f4)
{
	GLCall(glUniform4f(location, f1, f2, f3, f4));
}

template<>
void setUniformValue(int location, int32_t f1)
{
	GLCall(glUniform1i(location, f1));
}

template<>
void setUniformValue(int location,  int count, bool transpose, float *mat)
{
    GLCall(glUniformMatrix4fv(location, count, transpose, mat));
}
}

Shader::Shader(const std::string &vertexShader, const std::string &fragmentShader) : m_vertexShader(vertexShader),
                                                                       m_fragmentShader(fragmentShader),
                                                                       program(0)
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
}

void Shader::bind()
{
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
        std::cout << message.get();
        std::cout << std::endl;
    }
    return id;
}

int Shader::getUniformLocation(const std::string &uniformName)
{
    auto itr = locationCache.find(uniformName);
    if (itr != locationCache.end())
    {
        return itr->second;
    }

    GLCall(int location = glGetUniformLocation(program, uniformName.c_str()));
	if (location == -1)
	{
		std::cout << "Uniform: " << uniformName << " not found!" << std::endl;
		ASSERT(0);
	}

    locationCache[uniformName] = location;
    return location;
}