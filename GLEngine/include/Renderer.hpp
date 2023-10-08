#pragma once
#include <assert.h>

namespace GL_ENGINE
{
    void GLClearError();

    bool GLLogCall(const char* function, const char* file, int line);

    #define ASSERT(x) if (!(x)) assert(0);
    #define GLCall(x) GLClearError(); \
                    x;\
                    ASSERT(GLLogCall(#x, __FILE__, __LINE__))
}


