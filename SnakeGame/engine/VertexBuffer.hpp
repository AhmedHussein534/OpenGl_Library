#pragma once


class VertexBuffer
{
public:
	VertexBuffer();

	VertexBuffer(void* data, int size);

	void bind();

	void unbind();

	void addVertexData(void* data, int size);

	~VertexBuffer();

private:
	unsigned int renderId;
};