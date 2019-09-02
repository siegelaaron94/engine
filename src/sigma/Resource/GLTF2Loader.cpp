#include <sigma/Resource/GLTF2Loader.hpp>

#include <sigma/Resource/GLTF2.hpp>

#include <algorithm>
#include <cctype>
#include <fstream>

bool GLTF2Loader::supports(const std::filesystem::path &inPath)
{
	std::string ext = inPath.extension().string();
	std::transform(ext.begin(), ext.end(), ext.begin(), [](std::string::value_type c) { return std::tolower(c); });
	return ext == ".gltf";
}

void GLTF2Loader::load(std::shared_ptr<Context> inContext, const std::filesystem::path &inPath)
{
	GLTF2Document doc(inPath.parent_path());
	nlohmann::json description;
	std::ifstream file(inPath);
	file >> description;
	doc.deserialize(doc, description);
}
