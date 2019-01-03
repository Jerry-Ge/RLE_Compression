#include "RleData.h"
#include <iostream>

void RleData::Compress(const char* input, size_t inSize)
{
	// clean up prior data if any
	delete[] this->mData;
	this->mSize = 0;
	//sanity check
    if (inSize == 0)
    {
        return;
    }
	//allocate new memory for compression
	mData = new char[2 * inSize];

	//do the positive run first
	char currChar = input[0];
	int counter = 0;
	bool isNegative = true;
	std::string negString;
	for (unsigned i = 0; i < inSize; ++i)
	{
		//same char with previous one
		if (input[i] == currChar)
		{
			counter++;
			negString += currChar;
			if (counter > 1)
            {
                isNegative = false;
            }
		}
		else //different char with previous one
		{
			if (isNegative) {
				counter++;
				currChar = input[i];
				negString += currChar;

				if (i < inSize - 1) 
				{ //valid to check the next one
					if (input[i + 1] == currChar)
					{
						//save the prev negative run
						negString = negString.substr(0, negString.size() - 1);
						writeData(counter - 1, negString.c_str(), isNegative);

						//alter to positve run
						counter = 1;
						negString = "";
						isNegative = true;
					}
				}
			}
			else {
				//write the data
				writeData(counter, &currChar, isNegative);

				counter = 1;
				currChar = input[i];
				negString = currChar;
				isNegative = true;
			}
		}
	}

	//save the last run : either positive or negative
	if (isNegative)
	{
		writeData(counter, negString.c_str(), isNegative);
	}
	else
	{
		writeData(counter, &currChar, isNegative);
	}

}

void RleData::Decompress(const char* input, size_t inSize, size_t outSize)
{
	// clear prior data if any
    delete [] mData;
    mSize = 0;
    
    //allocate the memory for decompred string
    mData = new char[outSize];
    
    int inputCounter = 0;
    while (inputCounter < inSize)
    {
        char temp = input[inputCounter];
        inputCounter++;
        
        if (static_cast<int>(temp) < 0) // negative runs
        {
            int count = 0 - (static_cast<int>(temp) );
            
            for (unsigned i = 0; i < count; ++i)
            {
                char currChar = input[inputCounter];
                inputCounter++;
                mData[mSize] = currChar;
                mSize++;
            }
        }
        else //positive runs
        {
            int count = static_cast<int>(temp) ;

            char currChar = input[inputCounter];

            for (unsigned i = 0; i < count; ++i)
            {
                mData[mSize] = currChar;
                mSize++;
            }
            inputCounter++;
        }
    }
    
}

std::ostream& operator<< (std::ostream& stream, const RleData& rhs)
{
	for (int i = 0; i < rhs.mSize; i++)
	{
		stream << rhs.mData[i];
	}
	return stream;
}

//helper functions for debugging
void RleData::printData()
{
	std::cout << "Current the mData has size: ";
	std::cout << mSize << std::endl;
	for (unsigned i = 0; i < mSize; ++i)
    {
		int temp = mData[i];
		std::cout << "[index: " << i << "] ";
		std::cout << "0X" << std::hex << std::uppercase << temp << " ";
		std::cout << "||";
	}
	std::cout << std::endl;
	std::cout << "---------------------------------------------------" << std::endl;
}

void RleData::writeData(unsigned counter, const char* currChar, bool isNegative)
{	
	const int MAX_SIZE = 127;
	
	//write data for nagative run
	if (isNegative)
	{
		size_t writeSize = counter;
		int iteration = 0;
		while (counter > MAX_SIZE)
		{
			unsigned char size = (0 - 127);
			mData[mSize] = size;
			mSize++;
			for (unsigned i = 0; i < MAX_SIZE; ++i) {
				mData[mSize] = currChar[i + (iteration * MAX_SIZE)];
				mSize++;
			}
			counter -= MAX_SIZE;
			iteration++;
			
		}
		if (counter > 1)
		{
			unsigned char size =  (0 - counter);
			mData[mSize] = size;
			mSize++;
			int i = (MAX_SIZE * iteration);
			while (counter > 0)
			{
				mData[mSize] = currChar[i];
				counter--;
				mSize++;
				i++;
			}

		}
		else if (counter == 1)
		{
			mData[mSize] = 1;
			mSize++;
			mData[mSize] = currChar[writeSize - 1];
			mSize++;
		}

	}
	
	//write data for positive run
	else {
		while (counter > MAX_SIZE) //we can hold max 127 of int
		{
			mData[mSize] = MAX_SIZE;
			mData[mSize + 1] = currChar[0];
			mSize += 2;
			counter -= MAX_SIZE;
		} //the last write
		if (counter > 0)
		{
			mData[mSize] = counter;
			mData[mSize + 1] = currChar[0];
			mSize += 2;
		}
	}
}
