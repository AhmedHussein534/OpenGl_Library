#include "Layout.hpp"
#include <glm/gtc/type_ptr.hpp>


namespace GL_ENGINE
{
    Layout::Layout()
    {
    }

    void Layout::bind()
    {
        vArray.bind();
    }

    void Layout::unbind()
    {
        vArray.unbind();
    }

}