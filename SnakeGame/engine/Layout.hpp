#pragma once

#include <vector>

#include "VertexArray.hpp"
#include "IElement.hpp"
#include "ICamera.hpp"
#include <memory>

class Layout
{
public:
	Layout(std::shared_ptr<ICamera> camera);

	void bind();

	void unbind();

	void addElement(std::shared_ptr<IElement> e);

	~Layout();

	void draw();

private:
	std::vector<std::shared_ptr<IElement>> elements;
	VertexArray vArray;
	std::shared_ptr<ICamera> m_camera;

};