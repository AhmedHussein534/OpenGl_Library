#pragma once


class IndexBuffer
{
public:
	IndexBuffer();

	IndexBuffer(void* data, int size);

	void bind();

	void unbind();

	void addIndexData(void* data, int size);

	~IndexBuffer();

private:
	unsigned int renderId;
};