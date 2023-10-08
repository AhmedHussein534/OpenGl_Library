#include "Elements/TextureAsset.hpp"
#include "Renderer.hpp"
#include <external/glew/include/GL/glew.h>

namespace GL_ENGINE
{
    TextureAsset::TextureAsset(uint8_t* localBuffer, int32_t width, int32_t height, int32_t BPP) :
            m_rendererId(0),
            activeSlot(0)
    {
        GLCall(glGenTextures(1, &m_rendererId));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererId));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer));
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    }

    bool TextureAsset::bind()
    {
        bool gotSlot = TexSlotManagement::getInterface().getSlot(shared_from_this(), activeSlot);
        if (!gotSlot)
        {
            std::cout << "Failed to get slot" << std::endl;
            return false;
        }

        GLCall(glActiveTexture(GL_TEXTURE0 + activeSlot));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererId));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));
        return true;
    }

    bool TextureAsset::unbind()
    {
        GLCall(glActiveTexture(GL_TEXTURE0 + activeSlot));
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
        activeSlot = 0;
        return true;
    }

    TextureAsset::~TextureAsset()
    {
        unbind();
        GLCall(glDeleteTextures(1, &m_rendererId));
        TexSlotManagement::getInterface().giveSlot(activeSlot);
    }


    #include "external/stb_image/stb_image.hpp"
    std::shared_ptr<TextureAsset> StbTexture::createTexAsset(const std::string& path)
    {
        int width = 0;
        int height = 0;
        int bpp = 0;
        stbi_set_flip_vertically_on_load(1);
        uint8_t* m_localBuffer = stbi_load(path.c_str(), &width, &height, &bpp, 4);
        std::shared_ptr<TextureAsset> texPtr;
        if (m_localBuffer)
        {
            texPtr = std::make_shared<TextureAsset>(m_localBuffer, width, height, bpp);
            stbi_image_free(m_localBuffer);
        }
        else
        {
            std::cout << "FAILED TO LOAD IMAGE" << std::endl;
        }

        return texPtr;
    }
}