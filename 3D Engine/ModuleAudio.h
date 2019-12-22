#ifndef __ModuleAudio__H__
#define __ModuleAudio__H__

#include "Module.h"

class ModuleAudio :	public Module
{
public:
	ModuleAudio(bool start_enabled = true);
	virtual ~ModuleAudio();

private:
	
	bool InitWwise();
};

#endif __ModuleAudio__H__
