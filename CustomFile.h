#ifndef __CUSTOMEFILE_H__
#define __CUSTOMEFILE_H__

#include "Globals.h"
#include <string>

class CustomFile
{
	public:
		CustomFile(const char* filePath);
		~CustomFile();

	public:
		const char*					name = nullptr;
		const char*					path = nullptr;
		const char*					extension = nullptr;

};

#endif