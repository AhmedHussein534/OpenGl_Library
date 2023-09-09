#include "StbTexture.hpp"
#include "engine\Elements\Texture.hpp"

#include "external/stb_image/stb_image.hpp"

StbTexture::StbTexture(const std::string &path)
{
	int width = 0;
	int height = 0;
	int bpp = 0;
	stbi_set_flip_vertically_on_load(1);
	uint8_t* m_localBuffer = stbi_load(path.c_str(), &width, &height, &bpp, 4);

	std::cout << std::endl;
	std::cout << "Width = " << width << std::endl;
	std::cout << "HEIGHT = " << height << std::endl;
	std::cout << "BPP = " << bpp << std::endl;
	if (m_localBuffer)
	{
		texPtr = std::make_shared<Texture>(m_localBuffer, width, height, bpp, 0, -1.0f, 1.0f, 2.0f, 0.0f, -1.0f, 1.0f);
		stbi_image_free(m_localBuffer);
	}
	else
	{
		std::cout << "FAILED TO LOAD IMAGE" << std::endl;
	}
}

std::shared_ptr<Texture> StbTexture::getTex()
{
	return texPtr;
}

StbTexture::~StbTexture()
{
}