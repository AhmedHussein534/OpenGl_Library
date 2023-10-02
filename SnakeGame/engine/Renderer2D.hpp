#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

#include "Layout.hpp"
#include "Cameras/OrthographicCamera.hpp"

namespace GL_ENGINE
{
	class Renderer2D
	{
	public:

        static Renderer2D& getRenderer()
        {
            static Renderer2D renderer;
            return renderer;
        }


        std::shared_ptr<OrthographicCamera> beginScene(const float &left, const float &right, const float &bot, const float &top);

        uint32_t createLayout()
        {
            m_layouts.push_back(std::make_shared<Layout>());
            elementMap.push_back({});
            elementShaderMap.push_back({});
            return static_cast<uint32_t>(m_layouts.size() - 1);
        }

		bool addElement(uint32_t layoutKey, std::shared_ptr<IElement> e);

		bool drawScene();

        void endScene();

    private:
        Renderer2D();
        void createAndBindShader(uint32_t layoutKey, std::shared_ptr<IElement> e, const std::string &vertexShaderText, const std::string &indexShaderText);
        void drawElementsIndexed(const std::vector<VertexElement>& vertexElements, uint32_t indexCount);
        void drawIndexedAndFlush(const std::vector<VertexElement>& vertexElements);
	private:
        bool sceneExists;
		GL_ENGINE::ElementType currentShaderClass;
		std::vector<std::unordered_map<GL_ENGINE::ElementType, std::vector<std::shared_ptr<IElement>>>> elementMap;
        std::vector<std::unordered_map<GL_ENGINE::ElementType, std::shared_ptr<Shader>>> elementShaderMap;
		std::vector<std::shared_ptr<Layout>> m_layouts;
        std::shared_ptr<OrthographicCamera> m_camera;

        std::unique_ptr<uint8_t[]> vertexCpuBuffer;
        std::unique_ptr<uint8_t[]> indexCpuBuffer;
        std::unique_ptr<VertexBuffer> m_vertexBuffer;
        std::unique_ptr<IndexBuffer> m_indexBuffer;

        int vOffset = 0;
        int iOffset = 0;
        int maxIndex = 0;
        size_t vertexDataSize = 0;
        size_t indexDataSize = 0;

	};
}
