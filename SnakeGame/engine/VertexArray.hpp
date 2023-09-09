
#pragma once

class VertexArray
{
public:
	VertexArray();

	void bind();

	void unbind();

	~VertexArray();

private:
	unsigned int renderId;
};