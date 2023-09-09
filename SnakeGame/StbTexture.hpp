#include "engine/Elements/Texture.hpp"
#include <memory>
#include <string>

class StbTexture
{
public:
	StbTexture(const std::string& path);

	std::shared_ptr<Texture> getTex();

	~StbTexture();
private:
	std::shared_ptr<Texture> texPtr;
};