#include "catch.hpp"
#include "RleData.h"
#include "RleFile.h"
#include "SrcMain.h"
#include <iostream>
// Helper function declarations (don't change these)
extern bool BuffersAreSame(const char* expected, const char* actual, size_t size);
extern bool RunCompressionTest(char* test, size_t testSize,
							   char* expected, size_t expectedSize);
extern bool RunDecompressionTest(char* test, size_t testSize,
								 char* expected, size_t expectedSize);
extern bool CheckFileMD5(const std::string& fileName, const std::string& expected);


TEST_CASE("RLE Compression", "[student]")
{
	SECTION("Basic positive run")
	{
		char test[] = "aaabbbcccdddaaabbbcccdddaaabbbcccdddaaabbbc"
		"ccdddaaabbbcccdddaaabbbcccdddaaabbbcccdddaaabbbcccddd";
		char expected[] = "\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d";
		
		bool result = RunCompressionTest(test, sizeof(test) - 1,
										 expected, sizeof(expected) - 1);
		REQUIRE(result);
	}
	
	 //TODO: Add more test case sections!!
     // -127 <-> 127 the range
    SECTION("Varying positive runs")
    {
        char test[] = "aaabbbbccccc";
        char expected[] = "\x03" "a" "\x04" "b" "\x05" "c";
        
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    
    SECTION("Long positive runs")
    {
        char test[] = "aaaaaaaaaaaaaaaaaaaa"
                    "aaaaaaaaaaaaaaaaaaaa"
                    "aaaaaaaaaaaaaaaaaaaa"
                    "aaaaaaaaaaaaaaaaaaaa"
                    "aaaaaaaaaaaaaaaaaaaa"
                    "aaaaaaaaaaaaaaaaaaaa"
                    "aaaaaaaaaaaaaaaaaaaa"
                    "aaaaaaaaaaaaaaaaaaaa"
                    "aaaaaaaaaaaaaaaaaaaa"
                    "aaaaaaaaaaaaaaaaaaaa";
        char expected[] = "\x7f" "a" "\x49" "a";
        
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    
    SECTION("Simple negative runs")
    {
        char test[] = "abcdefghijklmnopqrstuvwxyz";
        char expected[] = "\xe6" "abcdefghijklmnopqrstuvwxyz";
        
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }


	SECTION("Simple negative run plus positive run")
	{
		char test[] = "abcdeaaa";
		char expected[] = "\xfb" "abcde"
							"\x03" "a";

		bool result = RunCompressionTest(test, sizeof(test) - 1,
			expected, sizeof(expected) - 1);
		REQUIRE(result);
	}


	SECTION("Long Negative Run")
	{
		char test[] = "abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz";
		
		char expected[] = "\x81" "abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvw"

			"\x81" "xyzabcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrst"

			"\xfa" "uvwxyz";

		bool result = RunCompressionTest(test, sizeof(test) - 1,
			expected, sizeof(expected) - 1);
		REQUIRE(result);
	}

	SECTION("Long Negative Run with additional one positve run")
	{
		char test[] = "abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwx";

		char expected[] = "\x81" "abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvw"
			"\x01" "x";


		bool result = RunCompressionTest(test, sizeof(test) - 1,
			expected, sizeof(expected) - 1);
		REQUIRE(result);
	}


	SECTION("Alternating vary long negative and positive run")
	{
		char test[] = "abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwx"

			"bbbbbbbbbbbbbbbbbbbb" //200 b's
			"bbbbbbbbbbbbbbbbbbbb"
			"bbbbbbbbbbbbbbbbbbbb"
			"bbbbbbbbbbbbbbbbbbbb"
			"bbbbbbbbbbbbbbbbbbbb"
			"bbbbbbbbbbbbbbbbbbbb"
			"bbbbbbbbbbbbbbbbbbbb"
			"bbbbbbbbbbbbbbbbbbbb"
			"bbbbbbbbbbbbbbbbbbbb"
			"bbbbbbbbbbbbbbbbbbbb"

			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwx";
			

		char expected[] = "\x81" "abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvw"
			"\x01" "x"
			"\x7f" "b" "\x49" "b"
			"\x81" "abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvw"
			"\x01" "x";


		bool result = RunCompressionTest(test, sizeof(test) - 1,
			expected, sizeof(expected) - 1);
		REQUIRE(result);
	}


	SECTION("Alternating vary long positive and nagative run")
	{
		char test[] =
			"bbbbbbbbbbbbbbbbbbbb" //200 b's
			"bbbbbbbbbbbbbbbbbbbb"
			"bbbbbbbbbbbbbbbbbbbb"
			"bbbbbbbbbbbbbbbbbbbb"
			"bbbbbbbbbbbbbbbbbbbb"
			"bbbbbbbbbbbbbbbbbbbb"
			"bbbbbbbbbbbbbbbbbbbb"
			"bbbbbbbbbbbbbbbbbbbb"
			"bbbbbbbbbbbbbbbbbbbb"
			"bbbbbbbbbbbbbbbbbbbb"

			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwx"


			"bbbbbbbbbbbbbbbbbbbb" //200 b's
			"bbbbbbbbbbbbbbbbbbbb"
			"bbbbbbbbbbbbbbbbbbbb"
			"bbbbbbbbbbbbbbbbbbbb"
			"bbbbbbbbbbbbbbbbbbbb"
			"bbbbbbbbbbbbbbbbbbbb"
			"bbbbbbbbbbbbbbbbbbbb"
			"bbbbbbbbbbbbbbbbbbbb"
			"bbbbbbbbbbbbbbbbbbbb"
			"bbbbbbbbbbbbbbbbbbbb";


		char expected[] =
			"\x7f" "b" "\x49" "b"
			"\x81" "abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvwxyz"
			"abcdefghijklmnopqrstuvw"
			"\x01" "x"
			"\x7f" "b" "\x49" "b";


		bool result = RunCompressionTest(test, sizeof(test) - 1,
			expected, sizeof(expected) - 1);
		REQUIRE(result);
	}
}



TEST_CASE("RLE Decompression", "[student]")
{
    SECTION("Basic positive run")
    {
        char test[] = "\x28" "x";
        char expected[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"; //40 x's

        bool result = RunDecompressionTest(test, sizeof(test) - 1,
                                           expected, sizeof(expected) - 1);
        REQUIRE(result);
    }

    SECTION("Basic negative run")
    {
        char test[] = "\xe6" "abcdefghijklmnopqrstuvwxyz";
        char expected[] = "abcdefghijklmnopqrstuvwxyz"; //alphabate table

        bool result = RunDecompressionTest(test, sizeof(test) - 1,
                                           expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    
    SECTION("Long positive run")
    {
        char test[] = "\x7f" "a" "\x49" "a";
        char expected[] = "aaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaa";

        bool result = RunDecompressionTest(test, sizeof(test) - 1,
                                           expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    
    
    SECTION("Basic positive plus negative")
    {
        char test[] = "\x05" "b" "\xfb" "abcde";
        char expected[] = "bbbbbabcde";
        
        bool result = RunDecompressionTest(test, sizeof(test) - 1,
                                           expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    
    SECTION("Basic negative plus positive")
    {
        char test[] = "\xfb" "abcde" "\x05" "b" ;
        char expected[] = "abcdebbbbb";
        
        bool result = RunDecompressionTest(test, sizeof(test) - 1,
                                           expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    
    SECTION("Alternating very long positive, negative, positive")
    {
        char test[] =   "\x7f" "b" "\x49" "b"
                        "\x81" "abcdefghijklmnopqrstuvwxyz"
                        "abcdefghijklmnopqrstuvwxyz"
                        "abcdefghijklmnopqrstuvwxyz"
                        "abcdefghijklmnopqrstuvwxyz"
                        "abcdefghijklmnopqrstuvw"
                        "\x01" "x"
                        "\x7f" "b" "\x49" "b";
        
        
        char expected[] = "bbbbbbbbbbbbbbbbbbbb" //200 b's
        "bbbbbbbbbbbbbbbbbbbb"
        "bbbbbbbbbbbbbbbbbbbb"
        "bbbbbbbbbbbbbbbbbbbb"
        "bbbbbbbbbbbbbbbbbbbb"
        "bbbbbbbbbbbbbbbbbbbb"
        "bbbbbbbbbbbbbbbbbbbb"
        "bbbbbbbbbbbbbbbbbbbb"
        "bbbbbbbbbbbbbbbbbbbb"
        "bbbbbbbbbbbbbbbbbbbb"
        
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwxyz"
        "abcdefghijklmnopqrstuvwx"
        
        
        "bbbbbbbbbbbbbbbbbbbb" //200 b's
        "bbbbbbbbbbbbbbbbbbbb"
        "bbbbbbbbbbbbbbbbbbbb"
        "bbbbbbbbbbbbbbbbbbbb"
        "bbbbbbbbbbbbbbbbbbbb"
        "bbbbbbbbbbbbbbbbbbbb"
        "bbbbbbbbbbbbbbbbbbbb"
        "bbbbbbbbbbbbbbbbbbbb"
        "bbbbbbbbbbbbbbbbbbbb"
        "bbbbbbbbbbbbbbbbbbbb";
        
        bool result = RunDecompressionTest(test, sizeof(test) - 1,
                                           expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    
    
    SECTION("Alternating very long negative, positive, negative")
    {
        char test[] =   "\x81" "abcdefghijklmnopqrstuvwxyz"
                        "abcdefghijklmnopqrstuvwxyz"
                        "abcdefghijklmnopqrstuvwxyz"
                        "abcdefghijklmnopqrstuvwxyz"
                        "abcdefghijklmnopqrstuvw"
                        "\x01" "x"
                        "\x7f" "b" "\x49" "b"
                        "\x81" "abcdefghijklmnopqrstuvwxyz"
                        "abcdefghijklmnopqrstuvwxyz"
                        "abcdefghijklmnopqrstuvwxyz"
                        "abcdefghijklmnopqrstuvwxyz"
                        "abcdefghijklmnopqrstuvw"
                        "\x01" "x";
        
        char expected[] =   "abcdefghijklmnopqrstuvwxyz"
                            "abcdefghijklmnopqrstuvwxyz"
                            "abcdefghijklmnopqrstuvwxyz"
                            "abcdefghijklmnopqrstuvwxyz"
                            "abcdefghijklmnopqrstuvwx"
        
                            "bbbbbbbbbbbbbbbbbbbb" //200 b's
                            "bbbbbbbbbbbbbbbbbbbb"
                            "bbbbbbbbbbbbbbbbbbbb"
                            "bbbbbbbbbbbbbbbbbbbb"
                            "bbbbbbbbbbbbbbbbbbbb"
                            "bbbbbbbbbbbbbbbbbbbb"
                            "bbbbbbbbbbbbbbbbbbbb"
                            "bbbbbbbbbbbbbbbbbbbb"
                            "bbbbbbbbbbbbbbbbbbbb"
                            "bbbbbbbbbbbbbbbbbbbb"
        
                            "abcdefghijklmnopqrstuvwxyz"
                            "abcdefghijklmnopqrstuvwxyz"
                            "abcdefghijklmnopqrstuvwxyz"
                            "abcdefghijklmnopqrstuvwxyz"
                            "abcdefghijklmnopqrstuvwx";
        
        bool result = RunDecompressionTest(test, sizeof(test) - 1,
                                           expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
	
}


TEST_CASE("Command line arguments", "[student]")
{
	SECTION("instructions.txt.rl1")
	{
		const char* argv[] = {
			"tests/tests",
			"data/instructions.txt.rl1"
		};
		ProcessCommandArgs(2, argv);
		bool result = CheckFileMD5("data/instructions.txt", "5e042d2e4bf014e714dd7cfc4dc25aab");
		REQUIRE(result);
	}
}

TEST_CASE("File compression", "[student]")
{
	RleFile r;
	SECTION("rle.bmp")
	{
		r.CreateArchive("data/rle.bmp");
		bool result = CheckFileMD5("data/rle.bmp.rl1", "f2a9d8425d53c664e45d9eb1b53137b9");
		REQUIRE(result);
	}
	SECTION("pic.jpg")
	{
		r.CreateArchive("data/pic.jpg");
		bool result = CheckFileMD5("data/pic.jpg.rl1", "0bbf2a5109b30d79939d2061ea8c74aa");
		REQUIRE(result);
	}
	SECTION("Conquest.ogg")
	{
		r.CreateArchive("data/Conquest.ogg");
		bool result = CheckFileMD5("data/Conquest.ogg.rl1", "ec29ff368ec5100bfba22635ddc5ba5c");
		REQUIRE(result);
	}
}


TEST_CASE("File decompression", "[student]")
{
	RleFile r;
	SECTION("works.bmp.rl1")
	{
		r.ExtractArchive("data/works.bmp.rl1");
		bool result = CheckFileMD5("data/works.bmp", "8baad647acefae2f8c36ee111582a875");
		REQUIRE(result);
	}
	SECTION("xkcd.bmp.rl1")
	{
		r.ExtractArchive("data/xkcd.bmp.rl1");
		bool result = CheckFileMD5("data/xkcd.bmp", "a4d7efa89d47a94a76f8050dd8d60cd0");
		REQUIRE(result);
	}
	SECTION("logo.png.rl1")
	{
		r.ExtractArchive("data/logo.png.rl1");
		bool result = CheckFileMD5("data/logo.png", "95403309460632f60efa647ef59b78fc");
		REQUIRE(result);
	}
}

