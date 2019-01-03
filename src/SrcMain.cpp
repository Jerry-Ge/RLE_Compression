#include "SrcMain.h"
#include <string>
#include "RleFile.h"
#include <iostream>
#include <cstring>
void ProcessCommandArgs(int argc, const char* argv[])
{
	if (argc < 2)
	{
		return;
	}
	bool isCompression = false;
	const char* fileName = argv[1];
	unsigned fileNameLength = strlen(fileName);
	std::string fileNameString(fileName, fileNameLength);	
	std::string fileFormat = fileNameString.substr(fileNameString.size() - 4, 4);

	RleFile mRleFile;
	if (fileNameLength < 4) //must be compression
	{
		mRleFile.CreateArchive(fileNameString);
	}
	else if (fileFormat == ".rl1")
	{
		mRleFile.ExtractArchive(fileNameString);
	} 
	else
	{
		std::cerr << "unkown state" << std::endl;
	}

}
