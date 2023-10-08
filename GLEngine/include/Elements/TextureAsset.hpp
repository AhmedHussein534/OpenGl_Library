#pragma once
#include <memory>
#include <list>
#include <unordered_map>
#include <stddef.h>
#include <iostream>


#define TEX_SLOTS 20

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
            auto it = takenAssets.find(m_ptr.get());
            if (it != takenAssets.end())
            {
                slot = it->second;
            }
            else if (!m_list.empty())
            {
                slot = m_list.back();
                m_list.pop_back();
                takenAssets[m_ptr.get()] = slot;
            }
            else
            {
                slot = 0;
                ret = false;
            }

            return ret;
        }

        bool giveSlot(uint32_t slot)
        {
            bool ret = false;
            for (auto &it : takenAssets)
            {
                if (it.second == slot)
                {
                    std::cout << "give slot: found slot = " << slot <<  std::endl;
                    takenAssets.erase(it.first);
                    m_list.push_back(slot);
                    ret = true;
                    break;
                }
            }

            return ret;
        }

        private:
        TexSlotManagement()
        {
            init();
        }

        std::list<uint32_t> m_list;
        std::unordered_map<TextureAsset*, uint32_t> takenAssets;
    };


    class TextureAsset : public std::enable_shared_from_this<TextureAsset>
    {
        public:
        TextureAsset(uint8_t* localBuffer, int32_t width, int32_t height, int32_t BPP);

        bool bind();

        bool unbind();

        uint32_t getActiveSlot()
        {
            return activeSlot;
        }

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

            std::shared_ptr<TextureAsset> createTexAsset(const std::string& path);


        private:
            StbTexture() = default;
    };
}