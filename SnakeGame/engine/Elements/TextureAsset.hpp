#pragma once
#include <memory>
#include <list>
#include <unordered_map>
#include <stddef.h>
#include <iostream>

#include "external/stb_image/stb_image.hpp"

#define TEX_SLOTS 16

namespace GL_ENGINE
{
    class TextureAsset;

    struct TexSlotManagement
    {
        static TexSlotManagement& getInterface()
        {
            static TexSlotManagement s_texSlotManagement;
            return s_texSlotManagement;
        }

        void init()
        {
            for (uint32_t i = 1; i <= TEX_SLOTS; i++)
            {
                m_list.push_back(i);
            }
        }

        void flush()
        {
            m_list.clear();
            takenAssets.clear();
            init();
        }


        bool getSlot(std::shared_ptr<TextureAsset> m_ptr, uint32_t &slot)
        {
            bool ret = true;
            auto it = takenAssets.find(m_ptr);
            if (it != takenAssets.end())
            {
                slot = takenAssets[m_ptr];
            }
            else if (!m_list.empty())
            {
                slot = m_list.back();
                m_list.pop_back();
                takenAssets[m_ptr] = slot;
            }
            else
            {
                slot = 0;
                ret = false;
            }

            return ret;
        }

        bool giveSlot(std::shared_ptr<TextureAsset> m_ptr, uint32_t slot)
        {
            bool ret = true;
            auto it = takenAssets.find(m_ptr);
            if (it != takenAssets.end())
            {
                takenAssets.erase(m_ptr);
                m_list.push_back(slot);
            }
            else
            {
                std::cout << "Texture slot is not allocated: " << slot << std::endl;
            }

            return ret;
        }

        private:
        TexSlotManagement()
        {
            init();
        }

        std::list<uint32_t> m_list;
        std::unordered_map<std::shared_ptr<TextureAsset>, uint32_t> takenAssets;
    };


    class TextureAsset : public std::enable_shared_from_this<TextureAsset>
    {
        public:
        TextureAsset(uint8_t* localBuffer, int32_t width, int32_t height, int32_t BPP);

        bool bind();

        bool unbind();

        ~TextureAsset();

        private:
        uint32_t getTextureSlot();

        private:
        uint32_t m_rendererId;
        uint32_t activeSlot;
    };


    class StbTexture
    {
        public:

            static StbTexture& getInstance()
            {
                static StbTexture m_stb;
                return m_stb;
            }

            std::shared_ptr<TextureAsset> createTexAsset(const std::string& path)
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

        private:
            StbTexture();
    };
}