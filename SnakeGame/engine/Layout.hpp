#pragma once

#include <vector>

#include "VertexArray.hpp"
#include "IElement.hpp"
#include <memory>

class Layout
{
public:
	Layout();

	void bind();

	void unbind();

	void addElement(std::shared_ptr<IElement> e);

	~Layout();

	void draw();

private:
	std::vector<std::shared_ptr<IElement>> elements;
	VertexArray vArray;

};