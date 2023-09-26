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
	std::shared_ptr<glm::mat4> batchVertexModel;
	std::shared_ptr<IElement> element;

	BatchData(std::shared_ptr<VertexBuffer> m_batchVertexBuffer,
			  std::shared_ptr<IndexBuffer> m_batchIndexBuffer,
			  std::shared_ptr<glm::mat4> m_batchVertexModel,
			  std::shared_ptr<IElement> m_element) : batchVertexBuffer(m_batchVertexBuffer),
													 batchIndexBuffer(m_batchIndexBuffer),
													 batchVertexModel(m_batchVertexModel),
													 element(m_element)
	{

	}

};

struct BatchDataContainer
{
	std::vector<std::shared_ptr<BatchData>> batchVec;

	bool isModelInContainer(std::shared_ptr<glm::mat4> batchVertexModel, std::shared_ptr<BatchData> &batchData)
	{
		bool ret = false;
		for (auto& b : batchVec)
		{
			auto &model1 = *(b->batchVertexModel);
			auto &model2 = *batchVertexModel;
			if (model1 ==  model2)
			{
				batchData = b;
				std::cout << "Found similarity" << std::endl;
				ret = true;
			}

		}

		return ret;
	}

	void addElementData(std::shared_ptr<IElement> element)
	{
		std::shared_ptr<BatchData> existingData;
		if (isModelInContainer(element->getModel(), existingData))
		{
			*existingData->batchVertexBuffer + *element->getVertexBuffer();
			*existingData->batchIndexBuffer + *element->getIndexBuffer();
		}
		else
		{
			auto v = std::make_shared<VertexBuffer>(*element->getVertexBuffer());
			auto i = std::make_shared<IndexBuffer>(*element->getIndexBuffer());
			auto m = std::make_shared<glm::mat4>(*(element->getModel()));
			* m = *(element->getModel());
			auto d = std::make_shared<BatchData>(v,
												 i,
												 m,
												 element);
			batchVec.push_back(d);
		}
	}
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
	std::vector<BatchDataContainer> batchDataContainers;
	VertexArray vArray;
};