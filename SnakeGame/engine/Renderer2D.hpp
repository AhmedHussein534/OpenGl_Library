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

		bool addElement(std::shared_ptr<IElement> e);

		bool drawScene();

        void endScene();

    private:
        Renderer2D();
	private:
		bool isShaderBinded;
        bool sceneExists;
		GL_ENGINE::ElementType currentShaderClass;
		std::unordered_map<GL_ENGINE::ElementType, std::vector<std::shared_ptr<IElement>>> elementMap;
		std::shared_ptr<Layout> m_layout;
        std::shared_ptr<OrthographicCamera> m_camera;
	};
}
