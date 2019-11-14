#include "JSONLoader.h"
#include <fstream>
#include <iomanip>

#include "mmgr/mmgr.h"

json JSONLoader::Load(const char * File) const
{
	if (File == nullptr)
	{
		assert(File != nullptr);
	}

	json jsonFile;

	std::ifstream ifs;
	ifs.open(File);
	assert(ifs.is_open());

	try
	{
		jsonFile = json::parse(ifs);
	}
	catch (json::parse_error& e)
	{
		LOG("Parse Error in loading file: %c", e.what());
	}

	ifs.close();

	return jsonFile;
}

void JSONLoader::Save(const char * File, json jsonfile)
{
	std::ofstream ofs;
	ofs.open(File);
	assert(ofs.is_open());
	ofs << std::setw(4) << jsonfile << std::endl;
	ofs.close();
}
