#ifndef __JSONLoader_H__
#define __JSONLoader_H__

#include "Globals.h"
#include "json.hpp"

using json = nlohmann::json;

class JSONLoader
{
public:

	JSONLoader() {}
	~JSONLoader() {}

	json Load(const char* File) const;
	void Save(const char* File, json jsonfile);
};

#endif __JSONLoader_H__
