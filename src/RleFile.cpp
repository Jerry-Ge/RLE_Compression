#include "RleFile.h"
#include "RleData.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>


void RleFile::CreateArchive(const std::string& source)
{
	
	
	std::ifstream::pos_type size;
	char* memblock;
	std::ifstream file(source, std::ios::in | std::ios::binary | std::ios::ate);
	
	
	if (file.is_open())
	{
		
		size = file.tellg(); //gives the size of source file
		memblock = new char[static_cast<unsigned int>(size)];
		file.seekg(0, std::ios::beg); //set to beginning
		file.read(memblock, size);
		file.close();
	
		mRleData.Compress(memblock, static_cast<size_t>(size)); //compress the source file
		
		//open file for writing in binary mode
		const std::string filename = source + ".rl1";
		std::ofstream arc(filename, std::ios::out | std::ios::binary | std::ios::trunc);
		
		if (arc.is_open())
		{
			mHeader.mFileSize = size;
			mHeader.mFileNameLength = source.size();
			mHeader.mFileName = source;


			//start writing stuff into buffer
			//write signature
			arc.write(mHeader.mSig, 4);

			//original filesize
			arc.write(reinterpret_cast<char*>(&(mHeader.mFileSize)), 4);
			
			//fileName: length and content
			arc.write(reinterpret_cast<char*>(&mHeader.mFileNameLength), 1);
			
			for (unsigned i = 0; i < mHeader.mFileNameLength; ++i)
			{
				arc.write(mHeader.mFileName.c_str() + i, 1); //convert to c-style string and write one byte each time
			}

			//write the main content
			arc.write(mRleData.mData, mRleData.mSize);

		}
		arc.close();
		
		delete[] memblock;
	}

}


void RleFile::ExtractArchive(const std::string& source)
{
	std::ifstream::pos_type size;
	char* memblock;
	std::ifstream file(source, std::ios::in | std::ios::binary | std::ios::ate);

	if (file.is_open())
	{
		//load the compressed file into memblock
		size = file.tellg(); //gives the size of source file
		memblock = new char[static_cast<unsigned int>(size)];
		file.seekg(0, std::ios::beg); //set to beginning
		file.read(memblock, size);
		file.close();

		//get the original uncompressed filesize
		mHeader.mFileSize = *(reinterpret_cast<int*>(&memblock[4]));
		
		//get the original file name length
		mHeader.mFileNameLength = *(reinterpret_cast<unsigned char*>(&memblock[8]));

		
		std::string originalFileName;
		for (unsigned i = 0; i < static_cast<int>(mHeader.mFileNameLength); ++i)
		{
			originalFileName += (memblock[9+i]); //append the char
		}
		

		std::string compressedContent;
		
		for (unsigned i = 9 + static_cast<int>(mHeader.mFileNameLength); i < size; ++i)
		{
			compressedContent += memblock[i];
		}

		
		//decompress the data
		mRleData.Decompress(compressedContent.c_str(), compressedContent.size(), mHeader.mFileSize);
		
		//save the decompressed data into file
		const std::string filename = originalFileName;
		std::ofstream exArc(filename, std::ios::out | std::ios::binary | std::ios::trunc);
		if (exArc.is_open())
		{
			exArc.write(mRleData.mData, mRleData.mSize);
		}
		exArc.close();
		
		delete[] memblock;
	}
}
