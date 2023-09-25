#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <typeindex>

#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "IElement.hpp"
#include "ICamera.hpp"


struct BatchData
{
	std::shared_ptr<VertexBuffer> batchVertexBuffer;
	std::shared_ptr<IndexBuffer> batchIndexBuffer;
	std::shared_ptr<IElement> batchVertexElement;
};

class Layout
{
public:
	Layout();

	void bind();

	void unbind();

	template <typename T>
	void addElement(std::shared_ptr<T> e)
	{
		auto key = std::type_index(typeid(T));
		if (elementMap.find(key) != elementMap.end())
		{
			elementMap[key].push_back(e);
		}
		else
		{
			elementMap[key] = std::vector<std::shared_ptr<IElement>>({e});
		}

		batchCached = false;
	}

	~Layout();

	void draw(std::shared_ptr<ICamera> camera);

	void drawBatched(std::shared_ptr<ICamera> camera);

private:
	bool batchCached;
	std::unordered_map<std::type_index, std::vector<std::shared_ptr<IElement>>> elementMap;
	std::unordered_map<std::type_index, BatchData> elementBatchBufferMap;
	VertexArray vArray;
};